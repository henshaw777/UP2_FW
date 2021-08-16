/** @file
  This file contains CPU Gnvs Struct specific to processor.

  Copyright (c) 1999 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef _POWER_MGMT_NVS_STRUCT_H_
#define _POWER_MGMT_NVS_STRUCT_H_

//
// Processor Power Management GlobalNvs Revisions
//
#define CPU_GLOBAL_NVS_AREA_REVISION_1  1

//
// Structure Declarations
//
#pragma pack(1)
///
/// CPU Global NVS Area definition
///
typedef struct {
  UINT8    Revision;                             ///< (0) CPU GlobalNvs Revision
  //
  // PPM Flag Values
  //
  UINT32   PpmFlags;                             ///< (1-4) PPM Flags
  //
  // Number of Logical Processors Values
  //
  UINT8    LogicalProcessorCount;                ///< (5) Number of Logical Processors
  //
  // Thermal Configuration Values
  //
  UINT8    AutoCriticalTripPoint;                ///< (6) Auto Critical Trip Point
  UINT8    AutoPassiveTripPoint;                 ///< (7) Auto Passive Trip Point
  UINT8    AutoActiveTripPoint;                  ///< (8) Auto Active Trip Point
  UINT32   Cpuid;                                ///< (9-12) CPUID

  UINT8    Reserved[28];                         ///< (13-40) Reserved
  //
  // Mwait Hints and Latency values for C3/C6/C7/C7S
  //
  UINT8    C3MwaitValue;                         ///< (41) Mwait Hint value for C3
  UINT8    C6MwaitValue;                         ///< (42) Mwait Hint value for C6
  UINT8    C7MwaitValue;                         ///< (43) Mwait Hint value for C6
  UINT8    CDMwaitValue;                         ///< (44) Mwait Hint value for C7/C8/C9/C10
  UINT16   C3Latency;                            ///< (45-46) Latency value for C3
  UINT16   C6Latency;                            ///< (47-48) Latency Value for C6
  UINT16   C7Latency;                            ///< (49-50) Latency Value for C6
  UINT16   CDLatency;                            ///< (51-52) Latency Value for C7/C8/C9/C10
  UINT16   CDIOLevel;                            ///< (53-54) IO Level Value for C7/C8/C9/C10
  UINT16   CDPowerValue;                         ///< (55-56) Power Value for C7/C8/C9/C10
  UINT8    MiscPowerManagementFlags;             ///< (57) MiscPowerManagementFlags
  //
  // DTS
  //
  UINT8    EnableDigitalThermalSensor;           ///< (58) DTS Function enable
  UINT8    BspDigitalThermalSensorTemperature;   ///< (59) Temperature of BSP
  UINT8    ApDigitalThermalSensorTemperature;    ///< (60) Temperature of AP
  UINT8    DigitalThermalSensorSmiFunction;      ///< (61) SMI function call via DTS IO Trap
  UINT8    PackageDTSTemperature;                ///< (62) Package temperature
  UINT8    IsPackageTempMSRAvailable;            ///< (63) Package Temperature MSR available
  UINT8    Ap2DigitalThermalSensorTemperature;   ///< (64) Temperature of the second AP
  UINT8    Ap3DigitalThermalSensorTemperature;   ///< (65) Temperature of the third AP

  UINT8    Reserved1[16];                        ///< (66-81) Reserved
  UINT8    DtsAcpiEnable;                        ///< (82) DTS is in ACPI Mode Enabled
  //
  // Software Guard Extension
  //
  UINT8    SgxStatus;                            ///< (83)    SE Status
  UINT64   EpcBaseAddress;                       ///< (84-91) EPC Base Address
  UINT64   EpcLength;                            ///< (92-99) EPC Length
  //
  //  HWP
  //
  UINT8    HwpVersion;                           ///< (100) HWP Status
  UINT16   HwpIoTrapAddress;                     ///< (101-102) IoTrap Address for HWP
  UINT16   HwpIoTrapLength;                      ///< (103-104) IoTrap Length for HWP
  UINT8    PowerState;                           ///< (105) Power State
  UINT8    EnableHdcPolicy;                      ///< (106) Hardware Duty Cycling Policy
  UINT8    HwpInterruptStatus;                   ///< (107) HWP Interrupt Status

  UINT8    MonitorMwaitEnable;                   ///< (108) Mwait Enable
} CPU_GLOBAL_NVS;
#pragma pack()

typedef struct _CPU_GLOBAL_NVS_AREA_CONFIG {
  CPU_GLOBAL_NVS  *Area;
} CPU_GLOBAL_NVS_AREA_CONFIG;

#pragma pack(1)
typedef struct _FVID_HEADER {
  UINT32  Stepping;    ///< Matches value returned by CPUID function 1
  UINT16  MaxBusRatio; ///< Matches BUS_RATIO_MAX field in PERF_STS_MSR
  UINT16  EistStates;  ///< Number of states of FVID (N)
} FVID_HEADER;

typedef struct _FVID_STATE {
  UINT32  State;    ///< State Number (0 - N-1)
  UINT16  BusRatio; ///< BUS_RATIO_SEL value to be written to PERF_CTL
  UINT32  Power;    ///< Typical power consumed by CPU in this state
} FVID_STATE;

typedef union _FVID_TABLE {
  FVID_HEADER  FvidHeader;
  FVID_STATE   FvidState;
  UINT64       FvidData;
} FVID_TABLE;
#pragma pack()

#endif

