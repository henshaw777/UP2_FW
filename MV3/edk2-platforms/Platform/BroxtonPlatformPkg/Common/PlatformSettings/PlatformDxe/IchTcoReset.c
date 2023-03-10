/** @file
  Implements the programming of events in TCO Reset.

  Copyright (c) 1999 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformDxe.h"
#include <Protocol/TcoReset.h>
#include <Protocol/HwWatchdogTimer.h>

EFI_STATUS
EFIAPI
EnableTcoReset (
  IN      UINT32            *RcrbGcsSaveValue
  );

EFI_STATUS
EFIAPI
DisableTcoReset (
  OUT     UINT32    RcrbGcsRestoreValue
  );

EFI_TCO_RESET_PROTOCOL  mTcoResetProtocol = {
  EnableTcoReset,
  DisableTcoReset
};


/**
  Enables the TCO timer to reset the system in case of a system hang.  This is
  used when writing the clock registers.

  @param[in] RcrbGcsSaveValue      This is the value of the RCRB GCS register before it is
                                   changed by this procedure.  This will be used to restore
                                   the settings of this register in PpiDisableTcoReset.

  @retval    EFI_STATUS

**/
EFI_STATUS
EFIAPI
EnableTcoReset (
  IN      UINT32            *RcrbGcsSaveValue
  )
{
  UINT16                              TmpWord;
  UINT16                              AcpiBaseAddr;
  EFI_WATCHDOG_TIMER_DRIVER_PROTOCOL  *WatchdogTimerProtocol;
  EFI_STATUS                          Status;
  UINTN                               PbtnDisableInterval = 4;  //Default value

  //
  // Get Watchdog Timer protocol.
  //
  Status = gBS->LocateProtocol (&gEfiWatchdogTimerDriverProtocolGuid, NULL, (VOID **) &WatchdogTimerProtocol);

  //
  // If the protocol is present, shut off the Timer as we enter BDS
  //
  if (!EFI_ERROR (Status)) {
    WatchdogTimerProtocol->RestartWatchdogTimer ();
    WatchdogTimerProtocol->AllowKnownReset (TRUE);
  }

  if (*RcrbGcsSaveValue == 0) {
    PbtnDisableInterval = PcdGet32 (PcdPBTNDisableInterval);
  } else {
    PbtnDisableInterval = *RcrbGcsSaveValue * 10 / 6;
  }

  //
  // Read ACPI Base Address
  //
  AcpiBaseAddr = (UINT16) PcdGet16 (PcdScAcpiIoPortBaseAddress);

  //
  // Stop TCO if not already stopped
  //
  TmpWord = IoRead16 (AcpiBaseAddr + R_TCO_CNT);
  TmpWord |= B_TCO_CNT_TMR_HLT;
  IoWrite16 (AcpiBaseAddr + R_TCO_CNT, TmpWord);

  //
  // Clear second TCO status
  //
  IoWrite32 (AcpiBaseAddr + R_TCO_STS, B_TCO_STS_SECOND_TO);

  //
  // Enable reboot on TCO timeout
  //
  *RcrbGcsSaveValue = MmioRead32 (PMC_BASE_ADDRESS + R_PMC_PM_CFG);
  MmioAnd8 (PMC_BASE_ADDRESS + R_PMC_PM_CFG, (UINT8) ~B_PMC_PM_CFG_NO_REBOOT);

  //
  // Set TCO reload value (interval *.6s)
  //
  IoWrite32 (AcpiBaseAddr + R_TCO_TMR, (UINT32) (PbtnDisableInterval << 16));

  //
  // Force TCO to load new value
  //
  IoWrite8 (AcpiBaseAddr + R_TCO_RLD, 4);

  //
  // Clear second TCO status
  //
  IoWrite32 (AcpiBaseAddr + R_TCO_STS, B_TCO_STS_SECOND_TO);

  //
  // Start TCO timer running
  //
  TmpWord = IoRead16 (AcpiBaseAddr + R_TCO_CNT);
  TmpWord &= ~(B_TCO_CNT_TMR_HLT);
  IoWrite16 (AcpiBaseAddr + R_TCO_CNT, TmpWord);

  //
  // Clear Sleep Type Enable
  //
  IoWrite16 (AcpiBaseAddr + R_SMI_EN,
    (UINT16) (IoRead16 (AcpiBaseAddr + R_SMI_EN) & (~B_SMI_EN_ON_SLP_EN)));

  return EFI_SUCCESS;
}


/**
  Disables the TCO timer.
  This is used after writing the clock registers.

  @param[out]  RcrbGcsRestoreValue    Value saved in PpiEnableTcoReset so that it can restored.

  @retval      EFI_STATUS

**/
EFI_STATUS
EFIAPI
DisableTcoReset (
  OUT     UINT32    RcrbGcsRestoreValue
  )
{
  UINT16                              TmpWord;
  UINT16                              AcpiBaseAddr;
  EFI_WATCHDOG_TIMER_DRIVER_PROTOCOL  *WatchdogTimerProtocol;
  EFI_STATUS                          Status;

  //
  // Read ACPI Base Address
  //
  AcpiBaseAddr = (UINT16) PcdGet16 (PcdScAcpiIoPortBaseAddress);

  //
  // Stop the TCO timer
  //
  TmpWord = IoRead16 (AcpiBaseAddr + R_TCO_CNT);
  TmpWord |= B_TCO_CNT_TMR_HLT;
  IoWrite16 (AcpiBaseAddr + R_TCO_CNT, TmpWord);

  //
  // Get Watchdog Timer protocol.
  //
  Status = gBS->LocateProtocol (&gEfiWatchdogTimerDriverProtocolGuid, NULL, (VOID **) &WatchdogTimerProtocol);

  //
  // If the protocol is present, shut off the Timer as we enter BDS
  //
  if (!EFI_ERROR (Status)) {
    WatchdogTimerProtocol->AllowKnownReset (FALSE);
  }

  return EFI_SUCCESS;
}


VOID
InitTcoReset (
  )
{
  EFI_HANDLE        Handle;
  EFI_STATUS        Status;

  Handle = NULL;
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiTcoResetProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mTcoResetProtocol
                  );

  ASSERT_EFI_ERROR (Status);
}

