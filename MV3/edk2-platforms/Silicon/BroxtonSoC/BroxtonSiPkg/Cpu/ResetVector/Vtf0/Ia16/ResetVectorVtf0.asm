;; @file
;  First code exectuted by processor after resetting.
;
;  Copyright (c) 2008 - 2016, Intel Corporation. All rights reserved.<BR>
;
;  This program and the accompanying materials
;  are licensed and made available under the terms and conditions of the BSD License
;  which accompanies this distribution.  The full text of the license may be found at
;  http://opensource.org/licenses/bsd-license.php.
;
;  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
;  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED
;
;;

BITS    16

ALIGN   16
FITHeader:
  DQ 0,0,0,0

applicationProcessorEntryPoint:
;
; Application Processors entry point
;
; GenFv generates code aligned on a 4k boundary which will jump to this
; location.  (0xffffffe0)  This allows the Local APIC Startup IPI to be
; used to wake up the application processors.
;
    jmp     short EarlyApInitReal16

ALIGN   8

    DD      0

;
; The VTF signature
;
; VTF-0 means that the VTF (Volume Top File) code does not require
; any fixups.
;
vtfSignature:
    DB      'V', 'T', 'F', 0

ALIGN   16 ; 0fffffff0

resetVector:
;
; Reset Vector
;
; This is where the processor will begin execution
;
; EDIT:LEE: changing this to "xcgh ax,cx" (running it twice will be a nop). This is just a quick way to verify I am running MY FW.
;   I should see 0x91, 0x91
;    nop
;    nop
    xchg ax,cx
    xchg ax,cx
    jmp     short EarlyBspInitReal16
ALIGN   16

fourGigabytes:

