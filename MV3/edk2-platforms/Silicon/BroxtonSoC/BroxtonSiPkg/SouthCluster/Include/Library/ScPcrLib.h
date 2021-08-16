/** @file
  Header file for PchPcrLib.

  Copyright (c) 2014 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _PCH_PCR_LIB_H_
#define _PCH_PCR_LIB_H_

#include <ScAccess.h>

/**
  Read PCR register.
  It returns PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of this Port ID
  @param[out] OutData                       Buffer of Output Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrRead32 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  OUT UINT32                            *OutData
  );

/**
  Read PCR register.
  It returns PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of this Port ID
  @param[out] OutData                       Buffer of Output Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrRead16 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  OUT UINT16                            *OutData
  );

/**
  Read PCR register.
  It returns PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of this Port ID
  @param[out] OutData                       Buffer of Output Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrRead8 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  OUT UINT8                             *OutData
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  InData                        Input Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrWrite32 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT32                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  InData                        Input Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrWrite16 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT16                            InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  InData                        Input Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrWrite8 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT8                             InData
  );

/**
  Write PCR register.
  It programs PCR register and size in 4bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  AndData                       AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                        OR Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrAndThenOr32 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 2bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  AndData                       AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                        OR Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrAndThenOr16 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT16                            AndData,
  IN  UINT16                            OrData
  );

/**
  Write PCR register.
  It programs PCR register and size in 1bytes.
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid                           Port ID
  @param[in]  Offset                        Register offset of Port ID.
  @param[in]  AndData                       AND Data. Must be the same size as Size parameter.
  @param[in]  OrData                        OR Data. Must be the same size as Size parameter.

  @retval     EFI_SUCCESS                   Successfully completed.
  @retval     EFI_INVALID_PARAMETER         Invalid offset passed.

**/
EFI_STATUS
PchPcrAndThenOr8 (
  IN  SC_SBI_PID                        Pid,
  IN  UINT16                            Offset,
  IN  UINT8                             AndData,
  IN  UINT8                             OrData
  );

#endif // _PCH_PCR_LIB_H_

