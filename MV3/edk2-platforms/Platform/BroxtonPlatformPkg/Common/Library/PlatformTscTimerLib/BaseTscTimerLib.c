/** @file
  A Base Timer Library implementation which uses the Time Stamp Counter in the processor.

  For Pentium 4 processors, Intel Xeon processors (family [0FH], models [03H and higher]);
    for Intel Core Solo and Intel Core Duo processors (family [06H], model [0EH]);
    for the Intel Xeon processor 5100 series and Intel Core 2 Duo processors (family [06H], model [0FH]);
    for Intel Core 2 and Intel Xeon processors (family [06H], display_model [17H]);
    for Intel Atom processors (family [06H], display_model [1CH]):
  the time-stamp counter increments at a constant rate.
  That rate may be set by the maximum core-clock to bus-clock ratio of the processor or may be set by
  the maximum resolved frequency at which the processor is booted. The maximum resolved frequency may
  differ from the maximum qualified frequency of the processor.

  The specific processor configuration determines the behavior. Constant TSC behavior ensures that the
  duration of each clock tick is uniform and supports the use of the TSC as a wall clock timer even if
  the processor core changes frequency. This is the architectural behavior moving forward.

  A Processor's support for invariant TSC is indicated by CPUID.0x80000007.EDX[8].

  Copyright (c) 2009 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "TscTimerLibInternal.h"

/**
  Get TSC frequency.

  @return The number of TSC counts per second.

**/
UINT64
InternalGetTscFrequency (
  VOID
  )
{
  return InternalCalculateTscFrequency ();
}

