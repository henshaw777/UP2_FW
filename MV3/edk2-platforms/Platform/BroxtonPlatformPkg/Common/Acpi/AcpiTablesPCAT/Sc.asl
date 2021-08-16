/** @file
  Copyright (c) 2012 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

Include ("GpioLib.asl")
Scope(\)
{
  //
  // Define ABASE I/O as an ACPI operating region. The base address
  // can be found in Device 31, Registers 40-43h.
  //
  OperationRegion(PMIO, SystemIo, \PMBS, 0x46)
  Field(PMIO, ByteAcc, NoLock, Preserve) {
      ,      8,
      PWBS,  1,    // Power Button Status
    Offset(0x20),
      ,      13,
      PMEB,  1,    // PME_B0_STS
    Offset(0x42),  // General Purpose Control
      ,      1,
      GPEC,  1
  }
  Field(PMIO, ByteAcc, NoLock, WriteAsZeros) {
    Offset(0x20),  // GPE0 Status
      ,      4,
      PSCI,  1,    // PUNIT SCI Status
      SCIS,  1     // GUNIT SCI Status
  }

  //
  // Define a Memory Region that will allow access to the PMC
  // Register Block.  Note that in the Intel Reference Solution, the PMC
  // will get fixed up dynamically during POST.
  //
  OperationRegion(PMCR, SystemMemory, Add(DD1A,0x1000), 0x80) // PMC Space
  Field(PMCR,DWordAcc,Lock,Preserve) {
    Offset(0x34),  //  Function Disable Register
    ,      22,
    RP2D,  1,      //  (22) Root Port 2 Disable
    RP1D,  1,      //  (23) Root Port 1 Disable
    Offset(0x38),  //  Function Disable Register 1
    ,      3,
    RP3D,  1,      //  (03) Root Port 3 Disable
    RP4D,  1,      //  (04) Root Port 4 Disable
    RP5D,  1,      //  (05) Root Port 5 Disable
    RP6D,  1,      //  (06) Root Port 6 Disable
  }

  //
  // Support S0, S3, S4, and S5.  The proper bits to be set when
  // entering a given sleep state are found in the Power Management
  // 1 Control ( PM1_CNT ) register, located at ACPIBASE + 04h,
  // bits 10d - 12d.
  //

} //end Scope(\)

scope (\_SB.PCI0) {
  Name(LTRN, 0)
  Name(OBFN, 0)

  Name(LMSL, 0)
  Name(LNSL, 0)

  //
  // LPC Bridge - Device 31, Function 0, this is only for PCH register Memory Region declare,
  // it's better to be declared as early as possible since it's widely used in whole ACPI name space.
  // Please add any code which needs to reference any register of it after this
  //
  Device(LPCB) {
    Name(_ADR, 0x001F0000)

    OperationRegion(LPC, PCI_Config, 0x00, 0x100)
    Field(LPC, AnyAcc, NoLock, Preserve)
    {
      Offset(0x02),
      CDID, 16,
      Offset(0x08),
      CRID,  8,
      Offset(0x80),
      IOD0,  8,
      IOD1,  8,
      Offset(0xA0),
      , 9,
      PRBL,  1,
      Offset(0xAC),
      ,      8,
      ,      8,
      XUSB,  1,
      Offset(0xB8),
          ,  22,
      GR0B,  2,
      ,      8,
      Offset(0xBC),
      ,      2,
      GR19,  2,
      ,     28,
    }
  }

  //
  // PCIE Root Port #01
  //
  Device(RP01) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA1,0)) {
        Return (RPA1)
      } Else {
        Return (0x00140000)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR1, LTRN)
      Store (PML1, LMSL)
      Store (PNL1, LNSL)
      Store (OBF1, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If (PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #01"

  //
  // PCIE Root Port #02
  //
  Device(RP02) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA2,0)) {
        Return (RPA2)
      } Else {
        Return (0x00140001)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR2, LTRN)
      Store (PML2, LMSL)
      Store (PNL2, LNSL)
      Store (OBF2, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x08, 4)) }  //GPE enable bit for RP2 is 8, can wake up from S4 state
    Method(_PRT, 0) {
      If (PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #02"

  //
  // PCIE Root Port #03
  //
  Device(RP03) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA3,0)) {
        Return (RPA3)
      } Else {
        Return (0x00130000)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR3, LTRN)
      Store (PML3, LMSL)
      Store (PNL3, LNSL)
      Store (OBF3, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If (PICM) { Return(AR04) }// APIC mode
      Return (PR04) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #03"

  //
  // PCIE Root Port #04
  //
  Device(RP04) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA4,0)) {
        Return (RPA4)
      } Else {
        Return (0x00130001)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR4, LTRN)
      Store (PML4, LMSL)
      Store (PNL4, LNSL)
      Store (OBF4, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If(PICM) { Return(AR05) }// APIC mode
      Return (PR05) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #04"

  //
  // PCIE Root Port #05
  //
  Device(RP05) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA5,0)) {
        Return (RPA5)
      } Else {
        Return (0x00130002)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR5, LTRN)
      Store (PML5, LMSL)
      Store (PNL5, LNSL)
      Store (OBF5, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If (PICM) { Return(AR06) }// APIC mode
      Return (PR06) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #05"

  //
  // PCIE Root Port #06
  //
  Device(RP06) {
    Method (_ADR, 0) {
      If (LNotEqual(RPA6,0)) {
        Return (RPA6)
      } Else {
        Return (0x00130003)
      }
    }
    //
    // Pass LTRx to LTRN so PchPcie.asl can be reused for PCIes.
    //
    Method(_INI)
    {
      Store (LTR6, LTRN)
      Store (PML6, LMSL)
      Store (PNL6, LNSL)
      Store (OBF6, OBFN)
    }
    Include("ScPcie.asl")
    Method(_PRW, 0) { Return(GPRW(0x09, 4)) }  // can wakeup from S4 state
    Method(_PRT, 0) {
      If (PICM) { Return(AR07) }// APIC mode
      Return (PR07) // PIC Mode
    } // end _PRT
  } // end "PCIE Root Port #06"

  include ("ScSata.asl")
  // xHCI Controller - Device 20, Function 0
  include ("ScXhci.asl")
  include ("ScXdci.asl")
  include ("ScScc.asl")
  include ("ScLpss.asl")
  include ("ScAudio.asl")
}

