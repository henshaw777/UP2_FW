## @file
#  Platform description.
#
#  Copyright (c) 2012 - 2018, Intel Corporation. All rights reserved.<BR>
#
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution. The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php.
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                       = BroxtonPlatformPkg
  PLATFORM_GUID                       = 1D3686CF-D2CE-4B6E-9F59-28F61976CB73
  PLATFORM_VERSION                    = 0.1
  DSC_SPECIFICATION                   = 0x00010005

  OUTPUT_DIRECTORY                    = Build/$(PLATFORM_NAME)
  SUPPORTED_ARCHITECTURES             = IA32
  BUILD_TARGETS                       = DEBUG|RELEASE
  SKUID_IDENTIFIER                    = DEFAULT

  #
  # Set platform specific package/folder names, same as passed from PREBUILD script.
  # PLATFORM_PACKAGE would be the same as PLATFORM_NAME as well as package build folder
  # The CORE_PACKAGES value could be set to '.' if Core packages get moved to Workspace root
  #
  DEFINE SILICON_NAME                    = BroxtonSoC
  DEFINE PLATFORM_PACKAGE_COMMON         = $(PLATFORM_NAME)/Common
  DEFINE PLATFORM_SI_PACKAGE             = $(SILICON_NAME)/BroxtonSiPkg

  !include $(PLATFORM_NAME)/DefineAtBuildMacros.dsc
  !include $(PLATFORM_NAME)/PlatformDsc/Defines.dsc

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

################################################################################
#
# Library Class section - list of all Library Classes needed by this Platform.
#
################################################################################
[LibraryClasses.Common]
  #
  # RC Package
  #
  !include $(PLATFORM_SI_PACKAGE)/SiPkgCommonLib.dsc
  #
  # Platform
  #
  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.dsc

[LibraryClasses.IA32.SEC]
  !if $(PERFORMANCE_ENABLE) == TRUE
    PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
  !endif

[LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE, LibraryClasses.IA32.SEC]
!if $(PERFORMANCE_ENABLE) == TRUE
  !if $(INTEL_FPDT_ENABLE) == TRUE
    PerformanceLib|$(PLATFORM_PACKAGE_COMMON)/Library/PeiFpdtPerformanceLib/PeiFpdtPerformanceLib.inf
  !else
    PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  !endif
!endif
  #
  # RC Package
  #
  !include $(PLATFORM_SI_PACKAGE)/SiPkgPeiLib.dsc

  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.IA32.PEI.dsc

[LibraryClasses.IA32.PEIM, LibraryClasses.IA32.PEI_CORE]
!if $(PERFORMANCE_ENABLE) == TRUE
  !if $(INTEL_FPDT_ENABLE) == TRUE
    PerformanceLib|$(PLATFORM_PACKAGE_COMMON)/Library/PeiFpdtPerformanceLib/PeiFpdtPerformanceLib.inf
  !else
    PerformanceLib|MdeModulePkg/Library/PeiPerformanceLib/PeiPerformanceLib.inf
  !endif
  TimerLib|$(PLATFORM_PACKAGE_COMMON)/Library/PlatformTscTimerLib/PeiTscTimerLib.inf
!endif

[LibraryClasses.IA32.DXE_DRIVER]
  !if $(PERFORMANCE_ENABLE) == TRUE
    PerformanceLib|MdeModulePkg/Library/DxePerformanceLib/DxePerformanceLib.inf
    TimerLib|$(PLATFORM_PACKAGE_COMMON)/Library/PlatformTscTimerLib/DxeTscTimerLib.inf
  !endif

[LibraryClasses.Common.DXE_DRIVER, LibraryClasses.Common.DXE_CORE, LibraryClasses.Common.DXE_SMM_DRIVER, LibraryClasses.Common.SMM_CORE, LibraryClasses.Common.DXE_RUNTIME_DRIVER]
  #
  # RC Package
  #
  !include $(PLATFORM_SI_PACKAGE)/SiPkgDxeLib.dsc

[LibraryClasses.Common.DXE_DRIVER]
  !include PlatformDsc/LibraryClasses.DxeDriver.dsc

[LibraryClasses.Common.DXE_CORE]
  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.DxeCore.dsc

[LibraryClasses.Common.DXE_SMM_DRIVER]
  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.DxeSmmDriver.dsc

[LibraryClasses.Common.SMM_CORE]
  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.SmmCore.dsc

[LibraryClasses.Common.DXE_RUNTIME_DRIVER]
  !include $(PLATFORM_NAME)/PlatformDsc/LibraryClasses.DxeRuntimeDriver.dsc

[LibraryClasses.Common.UEFI_APPLICATION]
  PcdLib|MdePkg/Library/DxePcdLib/DxePcdLib.inf

################################################################################
#
# Pcd Section - list of all EDK II PCD Entries defined by this Platform
#
################################################################################
[PcdsFeatureFlag.Common]
  !include $(PLATFORM_NAME)/PlatformDsc/PcdsFeatureFlag.dsc

[PcdsFixedAtBuild.Common]
  gEfiMdeModulePkgTokenSpaceGuid.PcdPeiCoreMaxFvSupported|0x9
  !include $(PLATFORM_NAME)/PlatformDsc/PcdsFixedAtBuild.dsc

[PcdsPatchableInModule.Common]
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x807B05FF
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x$(PLATFORM_PCIEXPRESS_BASE)
  gEfiIntelFrameworkModulePkgTokenSpaceGuid.PcdLegacyBiosCacheLegacyRegion|FALSE
  ## Default Creator ID for ACPI table creation, override to "BRXT"
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultCreatorId|0x54585242

[PcdsDynamicHii.Common.DEFAULT]
  !include $(PLATFORM_NAME)/PlatformDsc/PcdsDynamicHii.Default.dsc

[PcdsDynamicDefault.Common.DEFAULT]
  !include $(PLATFORM_NAME)/PlatformDsc/PcdsDynamicDefault.Default.dsc

[Components.IA32]
  !include $(PLATFORM_SI_PACKAGE)/SiPkgPei.dsc
  !include $(PLATFORM_NAME)/PlatformDsc/Components.IA32.dsc
  !include $(PLATFORM_NAME)/PlatformDsc/Components.dsc
  !include $(PLATFORM_SI_PACKAGE)/SiPkgDxe.dsc
  $(PLATFORM_SI_PACKAGE)/SouthCluster/ScInit/Dxe/ScInitDxe.inf

################################################################################
#
# Build Options section - list of all Build Flags used by this Platform.
#
################################################################################
#[BuildOptions.Common]

#
# Force PE/COFF sections to be aligned at 4KB boundaries to support page level
# protection of DXE_RUNTIME_DRIVER modules
#
[BuildOptions.common.EDKII.DXE_RUNTIME_DRIVER]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000

#
# Force PE/COFF sections to be aligned at 4KB boundaries to support page level
# protection of DXE_SMM_DRIVER/SMM_CORE modules
#
[BuildOptions.common.EDKII.DXE_SMM_DRIVER, BuildOptions.common.EDKII.SMM_CORE]
  MSFT:*_*_*_DLINK_FLAGS = /ALIGN:4096
  GCC:*_*_*_DLINK_FLAGS = -z common-page-size=0x1000

[BuildOptions.Common.EDKII]
   !include $(PLATFORM_NAME)/PlatformDsc/BuildOptions.EDKII.dsc

