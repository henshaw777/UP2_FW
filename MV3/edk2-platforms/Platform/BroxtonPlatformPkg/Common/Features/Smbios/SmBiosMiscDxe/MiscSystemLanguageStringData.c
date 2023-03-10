/** @file
  Static data of System language string information.
  System language string information is Misc. subclass type 12 and SMBIOS type 13.

  Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "CommonHeader.h"
#include "MiscSubclassDriver.h"

//
// Static (possibly build generated) Bios Vendor data.
//
MISC_SMBIOS_TABLE_DATA(EFI_MISC_SYSTEM_LANGUAGE_STRING_DATA, SystemLanguageString) = {
  0,
  STRING_TOKEN(STR_MISC_SYSTEM_LANGUAGE_EN_US)
};

