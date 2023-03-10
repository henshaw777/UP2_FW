;; @file
;  Port 0x80 debug support macros.
;
;  Copyright (c) 2009 - 2016, Intel Corporation. All rights reserved.<BR>
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

%macro  debugInitialize 0
    ;
    ; No initialization is required
    ;
%endmacro

%macro  debugShowPostCode 1
    mov     al, %1
    out     0x80, al
%endmacro

