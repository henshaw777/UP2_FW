/** @file
  Header file for Config Block Lib implementation.

  Copyright (c) 2015 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _CONFIG_BLOCK_LIB_H_
#define _CONFIG_BLOCK_LIB_H_

/**
  Create config block table

  @param[in]     NumberOfBlocks            Number of Config Blocks.

  @retval        Size                      Size of Config Block Table Header and Offset Table.

**/
UINT32
EFIAPI
GetSizeOfConfigBlockTableHeaders (
  IN     UINT16    NumberOfBlocks
  );

/**
  Create config block table

  @param[out]  ConfigBlockTableAddress      On return, points to a pointer to the beginning of Config Block Table Address.
  @param[in]   NumberOfBlocks               Number of Config Blocks.
  @param[in]   TotalSize                    Max size to be allocated for the Config Block Table.

  @retval      EFI_INVALID_PARAMETER        Invalid Parameter.
  @retval      EFI_OUT_OF_RESOURCES         Out of resources.
  @retval      EFI_SUCCESS                  Successfully created Config Block Table at ConfigBlockTableAddress.

**/
EFI_STATUS
EFIAPI
CreateConfigBlockTable (
  OUT    VOID      **ConfigBlockTableAddress,
  IN     UINT16    NumberOfBlocks,
  IN     UINT32    TotalSize
  );

/**
  Add config block into config block table structure.

  @param[in]  ConfigBlockTableAddress      A pointer to the beginning of Config Block Table Address.
  @param[out] ConfigBlockAddress           On return, points to a pointer to the beginning of Config Block Address.

  @retval     EFI_OUT_OF_RESOURCES         Config Block Table is full and cannot add new Config Block or
                                           Config Block Offset Table is full and cannot add new Config Block.
  @retval     EFI_SUCCESS                  Successfully added Config Block.

**/
EFI_STATUS
EFIAPI
AddConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  OUT    VOID      **ConfigBlockAddress
  );

/**
  Retrieve a specific Config Block data by GUID.

  @param[in]   ConfigBlockTableAddress     A pointer to the beginning of Config Block Table Address.
  @param[in]   ConfigBlockGuid             A pointer to the GUID uses to search specific Config Block.
  @param[out]  ConfigBlockAddress          On return, points to a pointer to the beginning of Config Block Address.

  @retval      EFI_NOT_FOUND               Could not find the Config Block.
  @retval      EFI_SUCCESS                 Config Block found and return.

**/
EFI_STATUS
EFIAPI
GetConfigBlock (
  IN     VOID      *ConfigBlockTableAddress,
  IN     EFI_GUID  *ConfigBlockGuid,
  OUT    VOID      **ConfigBlockAddress
  );

#endif // _CONFIG_BLOCK_LIB_H_

