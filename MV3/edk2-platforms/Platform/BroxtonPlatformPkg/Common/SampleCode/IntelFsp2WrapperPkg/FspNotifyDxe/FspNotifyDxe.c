/** @file
  This driver will register two callbacks to call fsp's notifies.

  Copyright (c) 2014 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>

#include <Protocol/PciEnumerationComplete.h>

#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/PerformanceLib.h>
#include <Library/HobLib.h>
#include <FspStatusCode.h>

typedef
EFI_STATUS
(EFIAPI * ADD_PERFORMANCE_RECORDS)(
  IN CONST VOID *HobStart
  );

struct _ADD_PERFORMANCE_RECORD_PROTOCOL {
  ADD_PERFORMANCE_RECORDS          AddPerformanceRecords;
};

typedef struct _ADD_PERFORMANCE_RECORD_PROTOCOL ADD_PERFORMANCE_RECORD_PROTOCOL;


extern EFI_GUID gAddPerfRecordProtocolGuid;
extern EFI_GUID gFspHobGuid;
extern EFI_GUID gFspApiPerformanceGuid;

EFI_EVENT mExitBootServicesEvent     = NULL;

/**
  Perform platform related reset in FSP wrapper.

  @param[in] ResetType  The type of reset the platform has to perform.

  @return Will reset the system based on Reset status provided.

**/
VOID
EFIAPI
CallFspWrapperResetSystem (
  IN UINT32    ResetType
  );

/**
  Relocate this image under 4G memory.

  @param  ImageHandle  Handle of driver image.
  @param  SystemTable  Pointer to system table.

  @retval EFI_SUCCESS  Image successfully relocated.
  @retval EFI_ABORTED  Failed to relocate image.

**/
EFI_STATUS
RelocateImageUnder4GIfNeeded (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  );

FSP_INFO_HEADER *mFspHeader = NULL;

/**
  PciEnumerationComplete Protocol notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.

**/
VOID
EFIAPI
OnPciEnumerationComplete (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  return;
}

/**
  Notification function of EVT_GROUP_READY_TO_BOOT event group.

  This is a notification function registered on EVT_GROUP_READY_TO_BOOT event group.
  When the Boot Manager is about to load and execute a boot option, it reclaims variable
  storage if free size is below the threshold.

  @param[in] Event        Event whose notification function is being invoked.
  @param[in] Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
OnReadyToBoot (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  NOTIFY_PHASE_PARAMS               NotifyPhaseParams;
  EFI_STATUS                        Status;
  ADD_PERFORMANCE_RECORD_PROTOCOL   *AddPerfRecordInterface;
  EFI_PEI_HOB_POINTERS              Hob;
  VOID                              **FspHobListPtr;

  gBS->CloseEvent (Event);
  NotifyPhaseParams.Phase = EnumInitPhaseReadyToBoot;
  PERF_START_EX (&gFspApiPerformanceGuid, "EventRec", NULL, 0, FSP_STATUS_CODE_READY_TO_BOOT_NOTIFICATION | FSP_STATUS_CODE_API_ENTRY);
  Status = CallFspNotifyPhase (&NotifyPhaseParams);
  PERF_END_EX (&gFspApiPerformanceGuid, "EventRec", NULL, 0, FSP_STATUS_CODE_READY_TO_BOOT_NOTIFICATION | FSP_STATUS_CODE_API_EXIT);

  //
  // Reset the system if FSP API returned FSP_STATUS_RESET_REQUIRED status
  //
  if ((Status & ~(0xF)) == (FSP_STATUS_RESET_REQUIRED_COLD & ~(0xF))) {
    DEBUG ((DEBUG_INFO, "FSP NotifyPhase ReadyToBoot requested reset 0x%x\n", Status));
    CallFspWrapperResetSystem ((UINT32) Status);
  }
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "FSP NotifyPhase ReadyToBoot failed, status: 0x%x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "FSP NotifyPhase ReadyToBoot Success.\n"));
  }

  Status = gBS->LocateProtocol (
                  &gAddPerfRecordProtocolGuid,
                  NULL,
                  (VOID **) &AddPerfRecordInterface
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "gAddPerfRecordProtocolGuid - Locate protocol failed\n"));
    return;
  } else {
    Hob.Raw = GetHobList ();
    if (Hob.Raw != NULL) {
      Hob.Raw = GetNextGuidHob (&gFspHobGuid, Hob.Raw);
      FspHobListPtr = GET_GUID_HOB_DATA (Hob.Raw);
      AddPerfRecordInterface->AddPerformanceRecords ((VOID *) (UINTN) (((UINT32) (UINTN) *FspHobListPtr) & 0xFFFFFFFF));
    }
  }
}

/**
  This stage is notified just before the firmware/Preboot environment transfers
  management of all system resources to the OS or next level execution environment.

  @param  Event         Event whose notification function is being invoked.
  @param  Context       Pointer to the notification function's context, which is
                        always zero in current implementation.

**/
VOID
EFIAPI
OnEndOfFirmware (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  NOTIFY_PHASE_PARAMS NotifyPhaseParams;
  EFI_STATUS          Status;

  gBS->CloseEvent (Event);

  NotifyPhaseParams.Phase = EnumInitPhaseEndOfFirmware;
  PERF_START_EX (&gFspApiPerformanceGuid, "EventRec", NULL, 0, FSP_STATUS_CODE_END_OF_FIRMWARE_NOTIFICATION | FSP_STATUS_CODE_API_ENTRY);
  Status = CallFspNotifyPhase (&NotifyPhaseParams);
  PERF_END_EX (&gFspApiPerformanceGuid, "EventRec", NULL, 0, FSP_STATUS_CODE_END_OF_FIRMWARE_NOTIFICATION | FSP_STATUS_CODE_API_EXIT);

  //
  // Reset the system if FSP API returned FSP_STATUS_RESET_REQUIRED status
  //
  if ((Status & ~(0xF)) == (FSP_STATUS_RESET_REQUIRED_COLD & ~(0xF))) {
    DEBUG ((DEBUG_INFO, "FSP NotifyPhase EndOfFirmware requested reset 0x%x\n", Status));
    CallFspWrapperResetSystem ((UINT32) Status);
  }
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "FSP NotifyPhase EndOfFirmware failed, status: 0x%x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "FSP NotifyPhase EndOfFirmware Success.\n"));
  }
}


/**
  Main entry for the FSP DXE module.

  This routine registers two callbacks to call fsp's notifies.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
FspDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  EFI_EVENT   ReadyToBootEvent;
  VOID        *Registration;
  EFI_EVENT   ProtocolNotifyEvent;

  //
  // Load this driver's image to memory
  //
  Status = RelocateImageUnder4GIfNeeded (ImageHandle, SystemTable);
  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  ProtocolNotifyEvent = EfiCreateProtocolNotifyEvent (
                          &gEfiPciEnumerationCompleteProtocolGuid,
                          TPL_CALLBACK,
                          OnPciEnumerationComplete,
                          NULL,
                          &Registration
                          );
  ASSERT (ProtocolNotifyEvent != NULL);

  Status = EfiCreateEventReadyToBootEx (
             TPL_CALLBACK,
             OnReadyToBoot,
             NULL,
             &ReadyToBootEvent
             );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  OnEndOfFirmware,
                  NULL,
                  &gEfiEventExitBootServicesGuid,
                  &mExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

