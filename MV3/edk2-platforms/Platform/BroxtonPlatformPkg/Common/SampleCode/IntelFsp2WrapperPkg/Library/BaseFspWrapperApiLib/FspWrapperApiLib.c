/** @file
  Provide FSP API related function.

  Copyright (c) 2014 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiPei.h>
#include <Guid/FspHeaderFile.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

/**
  Wrapper for a thunk  to transition from long mode to compatibility mode to execute 32-bit code and then transit back to
  long mode.

  @param[in] Function     The 32bit code entry to be executed.
  @param[in] Param1       The first parameter to pass to 32bit code.

  @return    EFI_STATUS

**/
EFI_STATUS
Execute32BitCode (
  IN UINT64      Function,
  IN UINT64      Param1,
  IN UINT64      Param2
  );

/**
  Find FSP header pointer.

  @param[in] FlashFvFspBase Flash address of FSP FV.

  @return FSP header pointer.

**/
FSP_INFO_HEADER *
EFIAPI
FspFindFspHeader (
  IN EFI_PHYSICAL_ADDRESS  FlashFvFspBase
  )
{
  UINT8   *CheckPointer;

  DEBUG ((DEBUG_INFO, "FspFindFspHeader\n"));

  CheckPointer = (UINT8 *) (UINTN) FlashFvFspBase;

  if (((EFI_FIRMWARE_VOLUME_HEADER *) CheckPointer)->Signature != EFI_FVH_SIGNATURE) {
    DEBUG ((DEBUG_INFO, "return 1\n"));
    return NULL;
  }

  if (((EFI_FIRMWARE_VOLUME_HEADER *) CheckPointer)->ExtHeaderOffset != 0) {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *) CheckPointer)->ExtHeaderOffset;
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_EXT_HEADER *) CheckPointer)->ExtHeaderSize;
    CheckPointer = (UINT8 *) ALIGN_POINTER (CheckPointer, 8);
  } else {
    CheckPointer = CheckPointer + ((EFI_FIRMWARE_VOLUME_HEADER *)CheckPointer)->HeaderLength;
  }

  CheckPointer = CheckPointer + sizeof (EFI_FFS_FILE_HEADER);

  if (((EFI_RAW_SECTION *) CheckPointer)->Type != EFI_SECTION_RAW) {
    DEBUG ((DEBUG_INFO, "return 2\n"));
    return NULL;
  }

  CheckPointer = CheckPointer + sizeof (EFI_RAW_SECTION);

  return (FSP_INFO_HEADER *) CheckPointer;
}

/**
  Call FSP API - FspNotifyPhase.

  @param[in] NotifyPhaseParams Address pointer to the NOTIFY_PHASE_PARAMS structure.

  @return EFI status returned by FspNotifyPhase API.

**/
EFI_STATUS
EFIAPI
CallFspNotifyPhase (
  IN NOTIFY_PHASE_PARAMS *NotifyPhaseParams
  )
{
  FSP_INFO_HEADER     *FspHeader;
  FSP_NOTIFY_PHASE    NotifyPhaseApi;
  EFI_STATUS          Status;
  BOOLEAN             InterruptState;

  FspHeader = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspsBaseAddress));
  if (FspHeader == NULL) {
    return EFI_DEVICE_ERROR;
  }

  NotifyPhaseApi = (FSP_NOTIFY_PHASE)((UINTN)FspHeader->ImageBase + (UINTN)FspHeader->NotifyPhaseEntryOffset);
  InterruptState = SaveAndDisableInterrupts ();
  Status = Execute32BitCode ((UINTN) NotifyPhaseApi, (UINTN) NotifyPhaseParams, (UINTN) NULL);
  SetInterruptState (InterruptState);

  return Status;
}

/**
  Call FSP API - FspMemoryInit.

  @param[in,out] FspMemoryInitParams Address pointer to the FSP_MEMORY_INIT_PARAMS structure.

  @return EFI status returned by FspMemoryInit API.

**/
EFI_STATUS
EFIAPI
CallFspMemoryInit (
  IN VOID                       *FspmUpdDataPtr,
  OUT VOID                      **HobListPtr
  )
{
  FSP_INFO_HEADER     *FspHeader;
  FSP_MEMORY_INIT     FspMemoryInitApi;
  EFI_STATUS          Status;
  BOOLEAN             InterruptState;

  FspHeader = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspmBaseAddress));
  if (FspHeader == NULL) {
    return EFI_DEVICE_ERROR;
  }

  FspMemoryInitApi = (FSP_MEMORY_INIT)((UINTN)FspHeader->ImageBase + (UINTN)FspHeader->FspMemoryInitEntryOffset);
  InterruptState = SaveAndDisableInterrupts ();
  Status = Execute32BitCode ((UINTN) FspMemoryInitApi, (UINTN) FspmUpdDataPtr, (UINTN) HobListPtr);
  SetInterruptState (InterruptState);

  return Status;
}

/**
  Call FSP API - TempRamExit.

  @param[in,out] TempRamExitParam    Address pointer to the TempRamExit parameters structure.

  @return EFI status returned by TempRamExit API.

**/
EFI_STATUS
EFIAPI
CallTempRamExit (
  IN VOID                       *TempRamExitParam
  )
{
  FSP_INFO_HEADER     *FspHeader;
  FSP_TEMP_RAM_EXIT   TempRamExitApi;
  EFI_STATUS          Status;
  BOOLEAN             InterruptState;

  FspHeader = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspmBaseAddress));
  if (FspHeader == NULL) {
    return EFI_DEVICE_ERROR;
  }

  TempRamExitApi = (FSP_TEMP_RAM_EXIT)((UINTN)FspHeader->ImageBase + (UINTN)FspHeader->TempRamExitEntryOffset);
  InterruptState = SaveAndDisableInterrupts ();

  DEBUG ((DEBUG_INFO, "** calling TempRamExitApi\n"));

  Status = Execute32BitCode ((UINTN) TempRamExitApi, (UINTN) TempRamExitParam, (UINTN) NULL);
  DEBUG ((DEBUG_INFO, "** TempRamExitApi returns\n"));
  SetInterruptState (InterruptState);
  DEBUG ((DEBUG_INFO, "** return\n"));
  return Status;
}

/**
  Call FSP API - FspSiliconInit.

  @param[in,out] FspSiliconInitParam Address pointer to the Silicon Init parameters structure.

  @return EFI status returned by FspSiliconInit API.

**/
EFI_STATUS
EFIAPI
CallFspSiliconInit (
  IN VOID                       *FspsUpdDataPtr
  )
{
  FSP_INFO_HEADER     *FspHeader;
  FSP_SILICON_INIT    FspSiliconInitApi;
  EFI_STATUS          Status;
  BOOLEAN             InterruptState;

  DEBUG ((DEBUG_INFO, "PcdGet32 (PcdFspsBaseAddress) = 0x%X\n", PcdGet32 (PcdFspsBaseAddress)));

  FspHeader = (FSP_INFO_HEADER *) FspFindFspHeader (PcdGet32 (PcdFspsBaseAddress));
  if (FspHeader == NULL) {
    return EFI_DEVICE_ERROR;
  }
  DEBUG ((DEBUG_INFO, "FspHeader = 0x%X\n", FspHeader));

  FspSiliconInitApi = (FSP_SILICON_INIT)((UINTN)FspHeader->ImageBase + (UINTN)FspHeader->FspSiliconInitEntryOffset);
  DEBUG ((DEBUG_INFO, "FspSiliconInitApi = 0x%X\n", FspSiliconInitApi));

  InterruptState = SaveAndDisableInterrupts ();
  Status = Execute32BitCode ((UINTN) FspSiliconInitApi, (UINTN) FspsUpdDataPtr, (UINTN) NULL);
  SetInterruptState (InterruptState);

  return Status;
}

