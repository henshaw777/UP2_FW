/** @file
  Board Init driver.

  Copyright (c) 2010 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "BoardInit.h"

EFI_STATUS
EFIAPI
AuroraGlacierPostMemInitCallback (
  IN EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );


static EFI_PEI_NOTIFY_DESCRIPTOR mAuroraGlacierPostMemNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardPostMemInitStartGuid,
  AuroraGlacierPostMemInitCallback
};

static EFI_PEI_PPI_DESCRIPTOR mAuroraGlacierPostMemDonePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gBoardPostMemInitDoneGuid,
  NULL
};

EFI_STATUS
EFIAPI
AuroraGlacierPostMemInitCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                       Status;
  VOID                             *Instance;
  UINT8                            BoardId;
  UINT8                            FabId;
  UINT8                            ResetType;
  UINTN                            BufferSize;
  UINT8                            MaxPkgCState;
  UINTN                            VariableSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  SYSTEM_CONFIGURATION             SystemConfiguration;

  VariableSize = sizeof (SYSTEM_CONFIGURATION);
  ZeroMem (&SystemConfiguration, sizeof (SYSTEM_CONFIGURATION));

  (*PeiServices)->LocatePpi (
                    (CONST EFI_PEI_SERVICES **)PeiServices,
                    &gEfiPeiReadOnlyVariable2PpiGuid,
                    0,
                    NULL,
                    (VOID **) &VariableServices
                    );

  VariableServices->GetVariable (
                      VariableServices,
                      PLATFORM_SETUP_VARIABLE_NAME,
                      &gEfiSetupVariableGuid,
                      NULL,
                      &VariableSize,
                      &SystemConfiguration
                      );

  Status = PeiServicesLocatePpi (
             &gBoardPostMemInitDoneGuid,
             0,
             NULL,
             &Instance
             );
  if (!EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }

  BoardId = (UINT8) PcdGet8 (PcdBoardId);
  FabId = (UINT8) PcdGet8 (PcdFabId);
  DEBUG ((EFI_D_INFO,  "PostMemInit: BoardId == 0x%X, FabId == 0x%X\n", BoardId, FabId));
  if (BoardId != (UINT8) BOARD_ID_AURORA) {
    return EFI_SUCCESS;
  }

  DEBUG ((EFI_D_INFO,  "Aurora Glacier Post Mem Init callback\n"));

  //
  // Set init function PCD
  //
  PcdSet64 (PcdBoardPostMemInitFunc, (UINT64) (UINTN) AuroraMultiPlatformInfoInit);

  //
  // Set Reset Type according to different Board
  //
  ResetType = V_RST_CNT_HARDRESET;
  PcdSet8 (PcdResetType, (UINT8) ResetType);

  //
  // Board specific VBT table.
  //
  BufferSize = sizeof (EFI_GUID);
  PcdSetPtr(PcdBoardVbtFileGuid, &BufferSize, (UINT8 *)&gPeiAuroraGlacierVbtGuid);
  PcdSetPtr(PcdOemLogoFileGuid, &BufferSize, (UINT8 *)&gPeiLogoGuid);

  //
  // Set PcdSueCreek
  //
  if (SystemConfiguration.SueCreekBypass) {
    PcdSetBool (PcdSueCreek, FALSE);
    PcdSetBool (PcdTi3100AudioCodecEnable, TRUE);
    DEBUG ((EFI_D_INFO,  "Bypass SueCreek \n"));
  } else {
    PcdSetBool (PcdSueCreek, TRUE);
    PcdSetBool (PcdTi3100AudioCodecEnable, FALSE); 
    DEBUG ((EFI_D_INFO,  "Use SueCreek \n"));
  }

  //
  // Set PcdMaxPkgCState
  //
  MaxPkgCState = MAX_PKG_CSTATE_C2;
  PcdSet8 (PcdMaxPkgCState, (UINT8) MaxPkgCState);
  
  //
  // Set PcdeMMCHostMaxSpeed
  //
  PcdSet8 (PcdeMMCHostMaxSpeed, (UINT8) (SystemConfiguration.ScceMMCHostMaxSpeed));

  //
  // I2S NHLT Virtual Bus ID
  //
  PcdSet8(HdaEndpointBtRenderVirtualBusId, 0x0F);  // N/A
  PcdSet8(HdaEndpointBtCaptureVirtualBusId, 0x0F); // N/A
  PcdSet8(HdaEndpointI2sRenderSKPVirtualBusId, 1); // I2S2
  PcdSet8(HdaEndpointI2sRenderHPVirtualBusId, 1);  // I2S2
  PcdSet8(HdaEndpointI2sCaptureVirtualBusId, 1);   // I2S2

  //
  // Add init steps here
  //
  //
  // Install a flag signalling a board's post mem init is done
  //
  Status = PeiServicesInstallPpi (&mAuroraGlacierPostMemDonePpi);

  return EFI_SUCCESS;
}


/**
  This function performs Board initialization in Pre-Memory.

  @retval     EFI_SUCCESS           The PPI is installed and initialized.
  @retval     EFI ERRORS            The PPI is not successfully installed.
  @retval     EFI_OUT_OF_RESOURCES  No enough resoruces (such as out of memory).

**/
EFI_STATUS
EFIAPI
AuroraGlacierInitConstructor (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                        Status;

  DEBUG ((EFI_D_INFO,  "AuroraGlacier Post Mem Init Constructor \n"));

  DEBUG ((EFI_D_INFO,  "Notify on Post Mem Init Start PPI \n"));
  Status = PeiServicesNotifyPpi (&mAuroraGlacierPostMemNotifyList);

  return Status;
}

