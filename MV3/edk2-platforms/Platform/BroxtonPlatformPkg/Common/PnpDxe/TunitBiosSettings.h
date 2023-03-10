/** @file
  Copyright (c) 2015 - 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef TUNIT_BIOS_SETTINGS_HH
#define TUNIT_BIOS_SETTINGS_HH

#define TUNIT_VALUEFORPERF_MSG_VALUES_PLATFORM_DEFAULT \
  \
    {0x52, 0x7810,   0,   0, 0x0},   \
    {0x52, 0x7810,   1,   1, 0x0},   \
    {0x52, 0x7810,   2,   2, 0x0},   \
    {0x52, 0x7810,   3,   3, 0x0},   \
    {0x52, 0x7810,   7,   7, 0x0},   \
    {0x52, 0x7810,  18,  18, 0x0},   \
    {0x52, 0x7810,  19,  19, 0x0},   \
    {0x52, 0x7810,  20,  20, 0x0},   \
    {0x52, 0x781c,   0,   0, 0x1},   \
    {0x52, 0x781c,   1,   1, 0x1},   \
    {0x52, 0x781c,   2,   2, 0x0},   \
    {0x52, 0x781c,   3,   3, 0x0},   \
    {0x52, 0x781c,   4,   4, 0x1},   \
    {0x52, 0x781c,   5,   5, 0x1},   \
    {0x52, 0x781c,   6,   6, 0x1},   \
    {0x52, 0x781c,   7,   7, 0x1},   \
    {0x52, 0x7820,   0,   0, 0x1},   \
    {0x52, 0x7820,   1,   1, 0x1},   \
    {0x52, 0x7820,   2,   2, 0x0},   \
    {0x52, 0x7820,   3,   3, 0x0},   \
    {0x52, 0x7820,   4,   4, 0x1},   \
    {0x52, 0x7820,   5,   5, 0x1},   \
    {0x52, 0x7820,   6,   6, 0x1},   \
    {0x52, 0x7820,   7,   7, 0x1},   \
    {0x52, 0x523c,   0,   0, 0x0},   \
    {0x52, 0x523c,   2,   2, 0x0},   \
    {0x52, 0x523c,   3,   3, 0x0},   \
    {0x52, 0x523c,  18,  18, 0x0},   \
    {0x52, 0x523c,  19,  19, 0x0},   \
    {0x52, 0x523c,  20,  20, 0x0},   \
    {0x52, 0x5288,   0,   0, 0x1},   \
    {0x52, 0x5288,   1,   1, 0x1},   \
    {0x52, 0x5288,   2,   2, 0x0},   \
    {0x52, 0x5288,   3,   3, 0x0},   \
    {0x52, 0x5288,   4,   4, 0x1},   \
    {0x52, 0x5288,   5,   5, 0x1},   \
    {0x52, 0x5288,   6,   6, 0x1},   \
    {0x52, 0x5288,   7,   7, 0x1},   \
    {0x52, 0x528c,   0,   0, 0x1},   \
    {0x52, 0x528c,   1,   1, 0x1},   \
    {0x52, 0x528c,   2,   2, 0x0},   \
    {0x52, 0x528c,   3,   3, 0x0},   \
    {0x52, 0x528c,   4,   4, 0x1},   \
    {0x52, 0x528c,   5,   5, 0x1},   \
    {0x52, 0x528c,   6,   6, 0x1},   \
    {0x52, 0x528c,   7,   7, 0x1}  \


#define TUNIT_VALUEFORPOWER_MSG_VALUES_PLATFORM_DEFAULT \
  \
    {0x52, 0x7810,   0,   0, 0x0},   \
    {0x52, 0x7810,   1,   1, 0x0},   \
    {0x52, 0x7810,   2,   2, 0x0},   \
    {0x52, 0x7810,   3,   3, 0x0},   \
    {0x52, 0x7810,   7,   7, 0x0},   \
    {0x52, 0x7810,  18,  18, 0x0},   \
    {0x52, 0x7810,  19,  19, 0x0},   \
    {0x52, 0x7810,  20,  20, 0x0},   \
    {0x52, 0x781c,   0,   0, 0x1},   \
    {0x52, 0x781c,   1,   1, 0x1},   \
    {0x52, 0x781c,   2,   2, 0x0},   \
    {0x52, 0x781c,   3,   3, 0x0},   \
    {0x52, 0x781c,   4,   4, 0x1},   \
    {0x52, 0x781c,   5,   5, 0x1},   \
    {0x52, 0x781c,   6,   6, 0x1},   \
    {0x52, 0x781c,   7,   7, 0x1},   \
    {0x52, 0x7820,   0,   0, 0x1},   \
    {0x52, 0x7820,   1,   1, 0x1},   \
    {0x52, 0x7820,   2,   2, 0x0},   \
    {0x52, 0x7820,   3,   3, 0x0},   \
    {0x52, 0x7820,   4,   4, 0x1},   \
    {0x52, 0x7820,   5,   5, 0x1},   \
    {0x52, 0x7820,   6,   6, 0x1},   \
    {0x52, 0x7820,   7,   7, 0x1},   \
    {0x52, 0x523c,   0,   0, 0x0},   \
    {0x52, 0x523c,   2,   2, 0x0},   \
    {0x52, 0x523c,   3,   3, 0x0},   \
    {0x52, 0x523c,  18,  18, 0x0},   \
    {0x52, 0x523c,  19,  19, 0x0},   \
    {0x52, 0x523c,  20,  20, 0x0},   \
    {0x52, 0x5288,   0,   0, 0x1},   \
    {0x52, 0x5288,   1,   1, 0x1},   \
    {0x52, 0x5288,   2,   2, 0x0},   \
    {0x52, 0x5288,   3,   3, 0x0},   \
    {0x52, 0x5288,   4,   4, 0x1},   \
    {0x52, 0x5288,   5,   5, 0x1},   \
    {0x52, 0x5288,   6,   6, 0x1},   \
    {0x52, 0x5288,   7,   7, 0x1},   \
    {0x52, 0x528c,   0,   0, 0x1},   \
    {0x52, 0x528c,   1,   1, 0x1},   \
    {0x52, 0x528c,   2,   2, 0x0},   \
    {0x52, 0x528c,   3,   3, 0x0},   \
    {0x52, 0x528c,   4,   4, 0x1},   \
    {0x52, 0x528c,   5,   5, 0x1},   \
    {0x52, 0x528c,   6,   6, 0x1},   \
    {0x52, 0x528c,   7,   7, 0x1}  \


#define TUNIT_VALUEFORPWRPERF_MSG_VALUES_PLATFORM_DEFAULT \
  \
    {0x52, 0x7810,   0,   0, 0x0},   \
    {0x52, 0x7810,   1,   1, 0x0},   \
    {0x52, 0x7810,   2,   2, 0x0},   \
    {0x52, 0x7810,   3,   3, 0x0},   \
    {0x52, 0x7810,   7,   7, 0x0},   \
    {0x52, 0x7810,  18,  18, 0x0},   \
    {0x52, 0x7810,  19,  19, 0x0},   \
    {0x52, 0x7810,  20,  20, 0x0},   \
    {0x52, 0x781c,   0,   0, 0x1},   \
    {0x52, 0x781c,   1,   1, 0x1},   \
    {0x52, 0x781c,   2,   2, 0x0},   \
    {0x52, 0x781c,   3,   3, 0x0},   \
    {0x52, 0x781c,   4,   4, 0x1},   \
    {0x52, 0x781c,   5,   5, 0x1},   \
    {0x52, 0x781c,   6,   6, 0x1},   \
    {0x52, 0x781c,   7,   7, 0x1},   \
    {0x52, 0x7820,   0,   0, 0x1},   \
    {0x52, 0x7820,   1,   1, 0x1},   \
    {0x52, 0x7820,   2,   2, 0x0},   \
    {0x52, 0x7820,   3,   3, 0x0},   \
    {0x52, 0x7820,   4,   4, 0x1},   \
    {0x52, 0x7820,   5,   5, 0x1},   \
    {0x52, 0x7820,   6,   6, 0x1},   \
    {0x52, 0x7820,   7,   7, 0x1},   \
    {0x52, 0x523c,   0,   0, 0x0},   \
    {0x52, 0x523c,   2,   2, 0x0},   \
    {0x52, 0x523c,   3,   3, 0x0},   \
    {0x52, 0x523c,  18,  18, 0x0},   \
    {0x52, 0x523c,  19,  19, 0x0},   \
    {0x52, 0x523c,  20,  20, 0x0},   \
    {0x52, 0x5288,   0,   0, 0x1},   \
    {0x52, 0x5288,   1,   1, 0x1},   \
    {0x52, 0x5288,   2,   2, 0x0},   \
    {0x52, 0x5288,   3,   3, 0x0},   \
    {0x52, 0x5288,   4,   4, 0x1},   \
    {0x52, 0x5288,   5,   5, 0x1},   \
    {0x52, 0x5288,   6,   6, 0x1},   \
    {0x52, 0x5288,   7,   7, 0x1},   \
    {0x52, 0x528c,   0,   0, 0x1},   \
    {0x52, 0x528c,   1,   1, 0x1},   \
    {0x52, 0x528c,   2,   2, 0x0},   \
    {0x52, 0x528c,   3,   3, 0x0},   \
    {0x52, 0x528c,   4,   4, 0x1},   \
    {0x52, 0x528c,   5,   5, 0x1},   \
    {0x52, 0x528c,   6,   6, 0x1},   \
    {0x52, 0x528c,   7,   7, 0x1}  \


#endif // TUNIT_BIOS_SETTINGS_HH

