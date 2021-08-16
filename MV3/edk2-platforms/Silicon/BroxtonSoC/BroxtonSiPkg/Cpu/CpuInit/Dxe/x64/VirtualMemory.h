/** @file
  x64 Long Mode Virtual Memory Management Definitions.

  Copyright (c) 2004 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _VIRTUAL_MEMORY_H_
#define _VIRTUAL_MEMORY_H_

#pragma pack(1)

///
/// Page-Map Level-4 Offset (PML4) and
/// Page-Directory-Pointer Offset (PDPE) entries 4K & 2MB
///
typedef union {
  struct {
    UINT64  Present : 1;                ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite : 1;              ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor : 1;         ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough : 1;           ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled : 1;          ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed : 1;               ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Reserved : 1;               ///< Reserved
    UINT64  MustBeZero : 2;             ///< Must Be Zero
    UINT64  Available : 3;              ///< Available for use by system software
    UINT64  PageTableBaseAddress : 40;  ///< Page Table Base Address
    UINT64  AvabilableHigh : 11;        ///< Available for use by system software
    UINT64  Nx : 1;                     ///< No Execute bit
  } Bits;
  UINT64  Uint64;
} x64_PAGE_MAP_AND_DIRECTORY_POINTER_2MB_4K;

///
/// Page-Directory Offset 4K
///
typedef union {
  struct {
    UINT64  Present : 1;                ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite : 1;              ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor : 1;         ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough : 1;           ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled : 1;          ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed : 1;               ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Reserved : 1;               ///< Reserved
    UINT64  MustBeZero : 1;             ///< Must Be Zero
    UINT64  Reserved2 : 1;              ///< Reserved
    UINT64  Available : 3;              ///< Available for use by system software
    UINT64  PageTableBaseAddress : 40;  ///< Page Table Base Address
    UINT64  AvabilableHigh : 11;        ///< Available for use by system software
    UINT64  Nx : 1;                     ///< No Execute bit
  } Bits;
  UINT64  Uint64;
} x64_PAGE_DIRECTORY_ENTRY_4K;

///
/// Page Table Entry 4K
///
typedef union {
  struct {
    UINT64  Present : 1;                ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite : 1;              ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor : 1;         ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough : 1;           ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled : 1;          ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed : 1;               ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty : 1;                  ///< 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  PAT : 1;                    ///< 0 = Ignore Page Attribute Table
    UINT64  Global : 1;                 ///< 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available : 3;              ///< Available for use by system software
    UINT64  PageTableBaseAddress : 40;  ///< Page Table Base Address
    UINT64  AvabilableHigh : 11;        ///< Available for use by system software
    UINT64  Nx : 1;                     ///< 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64  Uint64;
} x64_PAGE_TABLE_ENTRY_4K;

///
/// Page Table Entry 2MB
///
typedef union {
  struct {
    UINT64  Present : 1;                ///< 0 = Not present in memory, 1 = Present in memory
    UINT64  ReadWrite : 1;              ///< 0 = Read-Only, 1= Read/Write
    UINT64  UserSupervisor : 1;         ///< 0 = Supervisor, 1=User
    UINT64  WriteThrough : 1;           ///< 0 = Write-Back caching, 1=Write-Through caching
    UINT64  CacheDisabled : 1;          ///< 0 = Cached, 1=Non-Cached
    UINT64  Accessed : 1;               ///< 0 = Not accessed, 1 = Accessed (set by CPU)
    UINT64  Dirty : 1;                  ///< 0 = Not Dirty, 1 = written by processor on access to page
    UINT64  MustBe1 : 1;                ///< Must be 1
    UINT64  Global : 1;                 ///< 0 = Not global page, 1 = global page TLB not cleared on CR3 write
    UINT64  Available : 3;              ///< Available for use by system software
    UINT64  PAT : 1;                    ///<
    UINT64  MustBeZero : 8;             ///< Must be zero;
    UINT64  PageTableBaseAddress : 31;  ///< Page Table Base Address
    UINT64  AvabilableHigh : 11;        ///< Available for use by system software
    UINT64  Nx : 1;                     ///< 0 = Execute Code, 1 = No Code Execution
  } Bits;
  UINT64  Uint64;
} x64_PAGE_TABLE_ENTRY_2M;

typedef struct {
  UINT64  Present : 1;                  ///< 0 = Not present in memory, 1 = Present in memory
  UINT64  ReadWrite : 1;                ///< 0 = Read-Only, 1= Read/Write
  UINT64  UserSupervisor : 1;           ///< 0 = Supervisor, 1=User
  UINT64  WriteThrough : 1;             ///< 0 = Write-Back caching, 1=Write-Through caching
  UINT64  CacheDisabled : 1;            ///< 0 = Cached, 1=Non-Cached
  UINT64  Accessed : 1;                 ///< 0 = Not accessed, 1 = Accessed (set by CPU)
  UINT64  Dirty : 1;                    ///< 0 = Not Dirty, 1 = written by processor on access to page
  UINT64  Reserved : 57;
} x64_PAGE_TABLE_ENTRY_COMMON;

typedef union {
  x64_PAGE_TABLE_ENTRY_4K     Page4k;
  x64_PAGE_TABLE_ENTRY_2M     Page2Mb;
  x64_PAGE_TABLE_ENTRY_COMMON Common;
} x64_PAGE_TABLE_ENTRY;

#pragma pack()

#endif

