/** @file
  This code defines ACPI DMA Remapping table related definitions.
  See the System Agent BIOS specification for definition of the table.

  Copyright (c) 1999 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _DMA_REMAPPING_TABLE_H_
#define _DMA_REMAPPING_TABLE_H_

#include <Uefi.h>
#include <Base.h>
#include <IndustryStandard/Acpi30.h>

#pragma pack(1)

///
/// DMAR table signature
///
#define EFI_ACPI_VTD_DMAR_TABLE_SIGNATURE   0x52414D44  ///< "DMAR"
#define EFI_ACPI_DMAR_TABLE_REVISION        1
#define EFI_ACPI_DRHD_ENGINE_HEADER_LENGTH  0x10
#define EFI_ACPI_RMRR_HEADER_LENGTH         0x18
#define MAX_PCI_DEPTH                       5

typedef struct {
  UINT8   Type;
  UINT8   Length;
  UINT16  Reserved;
  UINT8   EnumId;
  UINT8   StartBusNumber;
  UINT8   PciPath[2];     ///< device, function
} EFI_ACPI_DEV_SCOPE_STRUCTURE;

typedef struct {
  UINT16                        Type;
  UINT16                        Length;
  UINT8                         Flags;
  UINT8                         Reserved;
  UINT16                        SegmentNum;
  EFI_PHYSICAL_ADDRESS          RegisterBaseAddress;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];
} EFI_ACPI_DRHD_ENGINE1_STRUCT;

typedef struct {
  UINT16                        Type;
  UINT16                        Length;
  UINT8                         Flags;
  UINT8                         Reserved;
  UINT16                        SegmentNum;
  EFI_PHYSICAL_ADDRESS          RegisterBaseAddress;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_DRHD_ENGINE2_STRUCT;

typedef struct {
  UINT16                        Type;
  UINT16                        Length;
  UINT16                        Reserved;
  UINT16                        SegmentNum;
  EFI_PHYSICAL_ADDRESS          RmrBaseAddress;
  EFI_PHYSICAL_ADDRESS          RmrLimitAddress;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[2];
} EFI_ACPI_RMRR_USB_STRUC;

typedef struct {
  UINT16                        Type;
  UINT16                        Length;
  UINT16                        Reserved;
  UINT16                        SegmentNum;
  EFI_PHYSICAL_ADDRESS          RmrBaseAddress;
  EFI_PHYSICAL_ADDRESS          RmrLimitAddress;
  EFI_ACPI_DEV_SCOPE_STRUCTURE  DeviceScope[1];    ///< IGD
} EFI_ACPI_RMRR_IGD_STRUC;

typedef struct {
  UINT16                        Type;
  UINT16                        Length;
  UINT8                         Reserved[3];
  UINT8                         AcpiDeviceNumber;
  UINT8                         AcpiObjectName[20];
} EFI_ACPI_ANDD_STRUC;

typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER   Header;
  UINT8                         HostAddressWidth;
  UINT8                         Flags;
  UINT8                         Reserved[10];
  EFI_ACPI_DRHD_ENGINE1_STRUCT  DrhdEngine1;
  EFI_ACPI_DRHD_ENGINE2_STRUCT  DrhdEngine2;
  EFI_ACPI_RMRR_USB_STRUC       RmrrUsb;
  EFI_ACPI_RMRR_IGD_STRUC       RmrrIgd;
} EFI_ACPI_DMAR_TABLE;

#pragma pack()

#endif

