/** @file
  Program SC SBI register.

  Copyright (c) 2013 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "ScPlatformLibrary.h"

EFI_STATUS
EFIAPI
PchSbiExecution (
  IN     SC_SBI_PID                     Pid,
  IN     UINT64                         Offset,
  IN     PCH_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{
  //
  // Check address valid
  //
  if (((UINT32) Offset & 0x3) != 0) {
    //
    // Warning message for the address not DWORD alignment.
    //
    DEBUG ((DEBUG_INFO, "PchSbiExecution: Be careful that the address is not DWORD alignment.\n"));
  }

  return PchSbiExecutionEx (
           Pid,
           Offset,
           Opcode,
           Posted,
           0x000F,
           0x0000,
           0x0000,
           Data32,
           Response
           );
}

EFI_STATUS
EFIAPI
PchSbiExecutionEx (
  IN     SC_SBI_PID                     Pid,
  IN     UINT64                         Offset,
  IN     PCH_SBI_OPCODE                 Opcode,
  IN     BOOLEAN                        Posted,
  IN     UINT16                         Fbe,
  IN     UINT16                         Bar,
  IN     UINT16                         Fid,
  IN OUT UINT32                         *Data32,
  OUT    UINT8                          *Response
  )
{
  EFI_STATUS                            Status;
  UINTN                                 P2sbBase;
  BOOLEAN                               P2sbOrgStatus;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  //
  // Check opcode valid
  //
  switch (Opcode) {
    case PciConfigRead:
    case PciConfigWrite:
    case PrivateControlRead:
    case PrivateControlWrite:
    case GpioLockUnlock:
      break;
    default:
      return EFI_INVALID_PARAMETER;
      break;
  }

  P2sbOrgStatus = FALSE;
  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_SC,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  PchRevealP2sb (P2sbBase, &P2sbOrgStatus);

  Timeout = 0xFFFFFFF;
  while (Timeout > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    Status = EFI_DEVICE_ERROR;
    goto ExitPchSbiExecutionEx;
  }
  //
  // Initial Response status
  //
  *Response = SBI_INVALID_RESPONSE;
  Status    = EFI_SUCCESS;
  SbiStat   = 0;

  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIADDR, (UINT32) ((Pid << 24) | (UINT16)Offset));

  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR, (UINT32) RShiftU64 (Offset, 16));

  MmioAndThenOr16 (
    (P2sbBase + R_PCH_P2SB_SBISTAT),
    (UINT16) ~(B_PCH_P2SB_SBISTAT_OPCODE | B_PCH_P2SB_SBISTAT_POSTED),
    (UINT16) ((Opcode << 8) | (Posted << 7))
    );

  MmioWrite16 (
    (P2sbBase + R_PCH_P2SB_SBIRID),
    (((Fbe & 0x000F) << 12) | ((Bar & 0x0007) << 8) | (Fid & 0x00FF))
    );

  switch (Opcode) {
    case PciConfigWrite:
    case PrivateControlWrite:
    case GpioLockUnlock:

      MmioWrite32 ((P2sbBase + R_PCH_P2SB_SBIDATA), *Data32);
      break;
    default:

      MmioWrite32 ((P2sbBase + R_PCH_P2SB_SBIDATA), 0);
      break;
  }

  MmioOr16 (P2sbBase + R_PCH_P2SB_SBISTAT, (UINT16) B_PCH_P2SB_SBISTAT_INITRDY);
  //
  // Poll SBISTAT[0] = 0b, Polling for Busy bit
  //
  Timeout = 0xFFFFFFF;
  while (Timeout > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
    Timeout--;
  }
  if (Timeout == 0) {
    //
    // If timeout, it's fatal error.
    //
    Status = EFI_DEVICE_ERROR;
  } else {
    *Response = (UINT8) ((SbiStat & B_PCH_P2SB_SBISTAT_RESPONSE) >> N_PCH_P2SB_SBISTAT_RESPONSE);
    if (*Response == SBI_SUCCESSFUL) {
      switch (Opcode) {
      case PciConfigRead:
      case PrivateControlRead:
        *Data32 = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIDATA);
        break;
      default:
        break;
      }
      Status = EFI_SUCCESS;
    } else {
      Status = EFI_DEVICE_ERROR;
    }
  }

ExitPchSbiExecutionEx:
  if (!P2sbOrgStatus) {
    PchHideP2sb (P2sbBase);
  }

  return Status;
}


/**
  This function saves all PCH SBI registers.
  The save and restore operations must be done while using the PchSbiExecution inside SMM.
  It prevents the racing condition of PchSbiExecution re-entry between POST and SMI.
  Before using this function, make sure the P2SB is not hidden.

  @param[in, out] PchSbiRegister        Structure for saving the registers

  @retval         EFI_SUCCESS           Successfully completed.
  @retval         EFI_DEVICE_ERROR      Device is hidden.

**/
EFI_STATUS
EFIAPI
PchSbiRegisterSave (
  IN OUT PCH_SBI_REGISTER_STRUCT        *PchSbiRegister
  )
{
  UINTN                                 P2sbBase;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_SC,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  if (MmioRead16 (P2sbBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Make sure it's not busy.
  // Poll SBISTAT[0] = 0b
  //
  Timeout = 0xFFFFFFF;
  while ((Timeout--) > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
  }
  if (Timeout == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Save original SBI registers
  //
  PchSbiRegister->SbiAddr    = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIADDR);
  PchSbiRegister->SbiExtAddr = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR);
  PchSbiRegister->SbiData    = MmioRead32 (P2sbBase + R_PCH_P2SB_SBIDATA);
  PchSbiRegister->SbiStat    = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
  PchSbiRegister->SbiRid     = MmioRead16 (P2sbBase + R_PCH_P2SB_SBIRID);

  return EFI_SUCCESS;
}


/**
  This function restores all PCH SBI registers
  The save and restore operations must be done while using the PchSbiExecution inside SMM.
  It prevents the racing condition of PchSbiExecution re-entry between POST and SMI.
  Before using this function, make sure the P2SB is not hidden.

  @param[in] PchSbiRegister             Structure for restoring the registers

  @retval    EFI_SUCCESS                Successfully completed.
  @retval    EFI_DEVICE_ERROR           Device is hidden.

**/
EFI_STATUS
EFIAPI
PchSbiRegisterRestore (
  IN PCH_SBI_REGISTER_STRUCT            *PchSbiRegister
  )
{
  UINTN                                 P2sbBase;
  UINTN                                 Timeout;
  UINT16                                SbiStat;

  P2sbBase      = MmPciBase (
                    DEFAULT_PCI_BUS_NUMBER_SC,
                    PCI_DEVICE_NUMBER_PCH_P2SB,
                    PCI_FUNCTION_NUMBER_PCH_P2SB
                    );
  if (MmioRead16 (P2sbBase + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Make sure it's not busy.
  // Poll SBISTAT[0] = 0b
  //
  Timeout = 0xFFFFFFF;
  while ((Timeout--) > 0){
    SbiStat = MmioRead16 (P2sbBase + R_PCH_P2SB_SBISTAT);
    if ((SbiStat & B_PCH_P2SB_SBISTAT_INITRDY) == 0) {
        break;
    }
  }
  if (Timeout == 0) {
    return EFI_DEVICE_ERROR;
  }
  //
  // Restore original SBI registers
  //
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIADDR   , PchSbiRegister->SbiAddr);
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIEXTADDR, PchSbiRegister->SbiExtAddr);
  MmioWrite32 (P2sbBase + R_PCH_P2SB_SBIDATA   , PchSbiRegister->SbiData);
  MmioWrite16 (P2sbBase + R_PCH_P2SB_SBISTAT   , PchSbiRegister->SbiStat);
  MmioWrite16 (P2sbBase + R_PCH_P2SB_SBIRID    , PchSbiRegister->SbiRid);

  return EFI_SUCCESS;
}

