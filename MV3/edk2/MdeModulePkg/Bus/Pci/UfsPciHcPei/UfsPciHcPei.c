/** @file
  UfsPciHcPei driver is used to provide platform-dependent info, mainly UFS host controller
  MMIO base, to upper layer UFS drivers.

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "UfsPciHcPei.h"

EDKII_UFS_HOST_CONTROLLER_PPI  mUfsHostControllerPpi = { GetUfsHcMmioBar };

EFI_PEI_PPI_DESCRIPTOR   mPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiUfsHostControllerPpiGuid,
  &mUfsHostControllerPpi
};

/**
  Get the MMIO base address of UFS host controller.

  @param[in]  This               The protocol instance pointer.
  @param[in]  ControllerId       The ID of the UFS host controller.
  @param[out] MmioBar            Pointer to the UFS host controller MMIO base address.

  @retval EFI_SUCCESS            The operation succeeds.
  @retval EFI_INVALID_PARAMETER  The parameters are invalid.

**/
EFI_STATUS
EFIAPI
GetUfsHcMmioBar (
  IN     EDKII_UFS_HOST_CONTROLLER_PPI *This,
  IN     UINT8                         ControllerId,
     OUT UINTN                         *MmioBar
  )
{
  UFS_HC_PEI_PRIVATE_DATA  *Private;

  if ((This == NULL) || (MmioBar == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Private = UFS_HC_PEI_PRIVATE_DATA_FROM_THIS (This);

  if (ControllerId >= Private->TotalUfsHcs) {
    return EFI_INVALID_PARAMETER;
  }
  
  *MmioBar = (UINTN)Private->UfsHcPciAddr[ControllerId];

  return EFI_SUCCESS;
}

/**
  The user code starts with this function.
  
  @param  FileHandle             Handle of the file being invoked.
  @param  PeiServices            Describes the list of possible PEI Services.

  @retval EFI_SUCCESS            The driver is successfully initialized.
  @retval Others                 Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
InitializeUfsHcPeim (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  EFI_BOOT_MODE            BootMode;
  EFI_STATUS               Status;
  UINT16                   Bus;
  UINT16                   Device;
  UINT16                   Function;
  UINT32                   Size;
  UINT8                    SubClass;
  UINT8                    BaseClass;
  UFS_HC_PEI_PRIVATE_DATA  *Private;

  //
  // Shadow this PEIM to run from memory
  //
  if (!EFI_ERROR (PeiServicesRegisterForShadow (FileHandle))) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesGetBootMode (&BootMode);
  ///
  /// We do not export this in S3 boot path, because it is only for recovery.
  ///
  if (BootMode == BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }

  Private = (UFS_HC_PEI_PRIVATE_DATA *) AllocateZeroPool (sizeof (UFS_HC_PEI_PRIVATE_DATA));
  if (Private == NULL) {
    DEBUG ((EFI_D_ERROR, "Failed to allocate memory for UFS_HC_PEI_PRIVATE_DATA! \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Private->Signature            = UFS_HC_PEI_SIGNATURE;
  Private->UfsHostControllerPpi = mUfsHostControllerPpi;
  Private->PpiList              = mPpiList;
  Private->PpiList.Ppi          = &Private->UfsHostControllerPpi;

  for (Bus = 0; Bus < 256; Bus++) {
    for (Device = 0; Device < 32; Device++) {
      for (Function = 0; Function < 8; Function++) {
        SubClass  = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x0A));
        BaseClass = PciRead8 (PCI_LIB_ADDRESS (Bus, Device, Function, 0x0B));

        if ((SubClass == 0x09) && (BaseClass == PCI_CLASS_MASS_STORAGE)) {
          //
          // Get the Ufs Pci host controller's MMIO region size.
          //
          PciAnd16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), (UINT16)~(EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
          PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), 0xFFFFFFFF);
          Size = PciRead32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET));
          //
          // Assign resource to the Ufs Pci host controller's MMIO BAR.
          // Enable the Ufs Pci host controller by setting BME and MSE bits of PCI_CMD register.
          //
          PciWrite32 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_BASE_ADDRESSREG_OFFSET), (UINT32)(PcdGet32 (PcdUfsPciHostControllerMmioBase) + Size * Private->TotalUfsHcs));
          PciOr16 (PCI_LIB_ADDRESS (Bus, Device, Function, PCI_COMMAND_OFFSET), (EFI_PCI_COMMAND_BUS_MASTER | EFI_PCI_COMMAND_MEMORY_SPACE));
          //
          // Record the allocated Mmio base address.
          //
          Private->UfsHcPciAddr[Private->TotalUfsHcs] = PcdGet32 (PcdUfsPciHostControllerMmioBase) + Size * Private->TotalUfsHcs;
          Private->TotalUfsHcs++;
          ASSERT (Private->TotalUfsHcs < MAX_UFS_HCS);
        }
      }
    }
  }

  ///
  /// Install Ufs Host Controller PPI
  ///
  Status = PeiServicesInstallPpi (&Private->PpiList);

  ASSERT_EFI_ERROR (Status);
  return Status;
}
