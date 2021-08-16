/** @file
  Multiplatform initialization header file.
  This file includes package header files, library classes.

  Copyright (c) 2010 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _MYBOARD_MULTIPLATFORM_LIB_H_
#define _MYBOARD_MULTIPLATFORM_LIB_H_

#include <BoardFunctionsPei.h>
#include <Guid/SetupVariable.h>
#include <Library/MemoryAllocationLib.h>

extern UPDATE_FSPM_UPD_FUNC mUp2UpdateFspmUpdPtr;
extern DRAM_CREATE_POLICY_DEFAULTS_FUNC mUp2DramCreatePolicyDefaultsPtr;

EFI_STATUS
EFIAPI
Up2UpdateFspmUpd (
  IN CONST EFI_PEI_SERVICES  **PeiServices,
  IN FSPM_UPD                *FspUpdRgn
  );

EFI_STATUS
EFIAPI
Up2DramCreatePolicyDefaults (
  IN  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariablePpi,
  OUT DRAM_POLICY_PPI                  **DramPolicyPpi,
  IN  IAFWDramConfig                   *DramConfigData,
  IN  UINTN                            *MrcTrainingDataAddr,
  IN  UINTN                            *MrcBootDataAddr,
  IN  UINT8                            BoardId
  );

#endif

