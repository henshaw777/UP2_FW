/** @file
  Implementation of translation upon PC ANSI.

  Copyright (c) 2006 - 2016, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "Terminal.h"

/**
  Translate all raw data in the Raw FIFO into unicode, and insert
  them into Unicode FIFO.

  @param TerminalDevice          The terminal device.

**/
VOID
AnsiRawDataToUnicode (
  IN  TERMINAL_DEV    *TerminalDevice
  )
{
  UINT8 RawData;

  //
  // pop the raw data out from the raw fifo,
  // and translate it into unicode, then push
  // the unicode into unicode fifo, until the raw fifo is empty.
  //
  while (!IsRawFiFoEmpty (TerminalDevice) && !IsUnicodeFiFoFull (TerminalDevice)) {

    RawFiFoRemoveOneKey (TerminalDevice, &RawData);

    UnicodeFiFoInsertOneKey (TerminalDevice, (UINT16) RawData);
  }
}

/**
  Check if input string is valid Ascii string, valid EFI control characters
  or valid text graphics.

  @param  TerminalDevice          The terminal device.
  @param  WString                 The input string.

  @retval EFI_UNSUPPORTED         If not all input characters are valid.
  @retval EFI_SUCCESS             If all input characters are valid.

**/
EFI_STATUS
AnsiTestString (
  IN  TERMINAL_DEV    *TerminalDevice,
  IN  CHAR16          *WString
  )
{
  CHAR8 GraphicChar;

  //
  // support three kind of character:
  // valid ascii, valid efi control char, valid text graphics.
  //
  for (; *WString != CHAR_NULL; WString++) {

    if ( !(TerminalIsValidAscii (*WString) ||
        TerminalIsValidEfiCntlChar (*WString) ||
        TerminalIsValidTextGraphics (*WString, &GraphicChar, NULL) )) {

      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}

