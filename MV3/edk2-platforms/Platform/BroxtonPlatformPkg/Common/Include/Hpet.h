/** @file
  This file describes the contents of the ACPI HEPT Table.

  Copyright (c) 1999 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _HPET_H
#define _HPET_H

//
// Statements that include other files
//

#include <IndustryStandard/Acpi10.h>
#include <IndustryStandard/Acpi20.h>
#include <IndustryStandard/Acpi30.h>
#include <IndustryStandard/HighPrecisionEventTimerTable.h>
#include "PlatformBaseAddresses.h"

//
// HPET Definitions
//
#define EFI_ACPI_HPET_TABLE_REVISION            0x01
#define MAIN_COUNTER_MIN_PERIODIC_CLOCK_TICKS   0x80        //approx 1ms
#define EFI_ACPI_EVENT_TIMER_BLOCK_ID           0x8086A201

#endif

