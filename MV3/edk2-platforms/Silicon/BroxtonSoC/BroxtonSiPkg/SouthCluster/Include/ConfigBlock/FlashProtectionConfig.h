/** @file
  Flash Protection policy.

  Copyright (c) 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _FLASH_PROTECTION_CONFIG_H_
#define _FLASH_PROTECTION_CONFIG_H_

#define FLASH_PROTECTION_CONFIG_REVISION 1

extern EFI_GUID gFlashProtectionConfigGuid;

#pragma pack (push,1)

/**
  The SC provides a method for blocking writes and reads to specific ranges
  in the SPI flash when the Protected Ranges are enabled.
  PROTECTED_RANGE is used to specify if flash protection are enabled,
  the write protection enable bit and the read protection enable bit,
  and to specify the upper limit and lower base for each register
  Platform code is responsible to get the range base by ScGetSpiRegionAddresses routine,
  and set the limit and base accordingly.

**/
typedef struct {
  UINT32                WriteProtectionEnable     :  1;     ///< Write or erase is blocked by hardware. <b>0: Disable</b>; 1: Enable.
  UINT32                ReadProtectionEnable      :  1;     ///< Read is blocked by hardware. <b>0: Disable</b>; 1: Enable.
  UINT32                RsvdBits                  :  30;    ///< Reserved
  /**
    The address of the upper limit of protection
    This is a left shifted address by 12 bits with address bits 11:0 are assumed to be FFFh for limit comparison
  **/
  UINT16                ProtectedRangeLimit;
  /**
    The address of the upper limit of protection
    This is a left shifted address by 12 bits with address bits 11:0 are assumed to be 0
  **/
  UINT16                ProtectedRangeBase;
} PROTECTED_RANGE;

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  PROTECTED_RANGE       ProtectRange[SC_FLASH_PROTECTED_RANGES];
} SC_FLASH_PROTECTION_CONFIG;

#pragma pack (pop)

#endif // _FLASH_PROTECTION_CONFIG_H_

