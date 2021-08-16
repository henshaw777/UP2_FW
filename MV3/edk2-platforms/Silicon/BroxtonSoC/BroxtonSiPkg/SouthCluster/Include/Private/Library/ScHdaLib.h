/** @file
  This library provides SC HD Audio functions.

  Copyright (c) 2014 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _SC_HDA_LIB_H_
#define _SC_HDA_LIB_H_

#include <Private/Library/DxeScHdaNhlt.h>
#include <Private/ScHdaEndpoints.h>

/**
  Prints NHLT (Non HDA-Link Table) to be exposed via ACPI (aka. OED (Offload Engine Driver) Configuration Table).

  @param[in]   NhltAcpiTable     The NHLT table to print

**/
VOID
NhltAcpiTableDump(
  IN NHLT_ACPI_TABLE           *NhltTable
  );

/**
  Constructs EFI_ACPI_DESCRIPTION_HEADER structure for NHLT table.

  @param[in, out] NhltTable            NHLT table for which header will be created
  @param[in]      NhltTableSize        Size of NHLT table

  @retval         None

**/
VOID
NhltAcpiHeaderConstructor (
  IN OUT NHLT_ACPI_TABLE        *NhltTable,
  IN UINT32                     NhltTableSize
  );

/**
  Constructs NHLT_ACPI_TABLE structure based on given Endpoints list.

  @param[in]      EndpointTable          List of endpoints for NHLT
  @param[in, out] NhltTable              NHLT table to be created
  @param[in, out] NhltTableSize          Size of created NHLT table

  @retval         EFI_SUCCESS            NHLT created successfully
  @retval         EFI_BAD_BUFFER_SIZE    Not enough resources to allocate NHLT

**/
EFI_STATUS
NhltConstructor(
  IN SC_HDA_NHLT_ENDPOINTS     *EndpointTable,
  IN OUT NHLT_ACPI_TABLE       **NhltTable,
  IN OUT UINT32                *NhltTableSize
  );

#endif

