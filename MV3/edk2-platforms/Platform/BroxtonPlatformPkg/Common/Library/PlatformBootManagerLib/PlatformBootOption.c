/** @file
  This file include all platform action which can be customized by IBV/OEM.

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "PlatformBootManager.h"

BOOLEAN    mContinueBoot  = FALSE;
BOOLEAN    mUiAppBoot     = FALSE;
BOOLEAN    mPxeBoot       = FALSE;
BOOLEAN    mAnyKeypressed = FALSE;
BOOLEAN    mHotKeypressed = FALSE;
EFI_EVENT  HotKeyEvent    = NULL;
UINTN      mUiAppOptionNumber;
CHAR16     *TmpStr = L"Press [F2] key to enter BIOS Setup";

EFI_GRAPHICS_OUTPUT_BLT_PIXEL mForeground;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL mBackground;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL mColor;

EFI_GUID mUiFile = { 0x462CAA21, 0x7614, 0x4503, { 0x83, 0x6E, 0x8A, 0xB6, 0xF4, 0x66, 0x23, 0x31 } };
EFI_GUID mBootMenuFile = { 0xEEC25BDC, 0x67F2, 0x4D95, { 0xB1, 0xD5, 0xF8, 0x1B, 0x20, 0x39, 0xD1, 0x1D }};
EFI_GUID gUefiShellFileGuid = { 0x7C04A583, 0x9E3E, 0x4f1c, 0xAD, 0x65, 0xE0, 0x52, 0x68, 0xD0, 0xB4, 0xD1 };

#define INTERNAL_UEFI_SHELL_NAME      L"Internal UEFI Shell 2.0"
#define UEFI_HARD_DRIVE_NAME          L"UEFI Hard Drive"
#define UEFI_NVME_DRIVE_NAME          L"UEFI NVMe Storage"

/**
  Show progress bar with title above it. It only works in Graphics mode.

  @param TitleForeground Foreground color for Title.
  @param TitleBackground Background color for Title.
  @param Title           Title above progress bar.
  @param ProgressColor   Progress bar color.
  @param Progress        Progress (0-100)
  @param PreviousValue   The previous value of the progress.

  @retval  EFI_STATUS       Success update the progress bar.

**/
EFI_STATUS
PlatformBdsShowProgress (
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL TitleForeground,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL TitleBackground,
  IN CHAR16                        *Title,
  IN EFI_GRAPHICS_OUTPUT_BLT_PIXEL ProgressColor,
  IN UINTN                         Progress,
  IN UINTN                         PreviousValue
  )
{
  EFI_STATUS                     Status;
  EFI_GRAPHICS_OUTPUT_PROTOCOL   *GraphicsOutput;
  UINT32                         SizeOfX;
  UINT32                         SizeOfY;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL  Color;
  UINTN                          BlockHeight;
  UINTN                          BlockWidth;
  UINTN                          BlockNum;
  UINTN                          PosX;
  UINTN                          PosY;
  UINTN                          Index;

  if (Progress > 100) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gBS->HandleProtocol (
                  gST->ConsoleOutHandle,
                  &gEfiGraphicsOutputProtocolGuid,
                  (VOID **) &GraphicsOutput
                  );
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  SizeOfX = 0;
  SizeOfY = 0;
  if (GraphicsOutput != NULL) {
    SizeOfX = GraphicsOutput->Mode->Info->HorizontalResolution;
    SizeOfY = GraphicsOutput->Mode->Info->VerticalResolution;
  } else {
    return EFI_UNSUPPORTED;
  }

  BlockWidth  = SizeOfX / 100;
  BlockHeight = SizeOfY / 50;

  BlockNum    = Progress;

  PosX        = 0;
  PosY        = SizeOfY * 48 / 50;

  if (BlockNum == 0) {
    //
    // Clear progress area
    //
    SetMem (&Color, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);

    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                          GraphicsOutput,
                          &Color,
                          EfiBltVideoFill,
                          0,
                          0,
                          0,
                          PosY - EFI_GLYPH_HEIGHT - 1,
                          SizeOfX,
                          SizeOfY - (PosY - EFI_GLYPH_HEIGHT - 1),
                          SizeOfX * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                          );
    } else {
      return EFI_UNSUPPORTED;
    }
  }
  //
  // Show progress by drawing blocks.
  //
  for (Index = PreviousValue; Index < BlockNum; Index++) {
    PosX = Index * BlockWidth;
    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                          GraphicsOutput,
                          &ProgressColor,
                          EfiBltVideoFill,
                          0,
                          0,
                          PosX,
                          PosY,
                          BlockWidth - 1,
                          BlockHeight,
                          (BlockWidth) * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                          );
    } else {
      return EFI_UNSUPPORTED;
    }
  }

  PrintXY (
    (SizeOfX - StrLen (Title) * EFI_GLYPH_WIDTH) / 2,
    PosY - EFI_GLYPH_HEIGHT - 1,
    &TitleForeground,
    &TitleBackground,
    Title
    );

  return EFI_SUCCESS;
}

/*++

This function will create a SHELL BootOption to boot.

  @param  void

  @retval EFI_STATUS

--*/
EFI_DEVICE_PATH_PROTOCOL *
BdsCreateShellDevicePath (
  VOID
  )

{
  UINTN                             FvHandleCount;
  EFI_HANDLE                        *FvHandleBuffer;
  UINTN                             Index;
  EFI_STATUS                        Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *Fv;
  UINTN                             Size;
  UINT32                            AuthenticationStatus;
  EFI_DEVICE_PATH_PROTOCOL          *DevicePath;
  VOID                              *Buffer;

  DevicePath  = NULL;
  Status      = EFI_SUCCESS;

  DEBUG ((DEBUG_ERROR, "[FVMAIN2] BdsCreateShellDevicePath\n"));
  gBS->LocateHandleBuffer (
        ByProtocol,
        &gEfiFirmwareVolume2ProtocolGuid,
        NULL,
        &FvHandleCount,
        &FvHandleBuffer
        );

  for (Index = 0; Index < FvHandleCount; Index++) {
    gBS->HandleProtocol (
          FvHandleBuffer[Index],
          &gEfiFirmwareVolume2ProtocolGuid,
          (VOID **) &Fv
          );

    Buffer  = NULL;
    Size    = 0;
    Status  = Fv->ReadSection (
                    Fv,
                    &gUefiShellFileGuid,
                    EFI_SECTION_PE32,
                    0,
                    &Buffer,
                    &Size,
                    &AuthenticationStatus
                    );
    if (EFI_ERROR (Status)) {
      //
      // Skip if no shell file in the FV.
      //
      continue;
    } else {
      //
      // Found the shell.
      //
      break;
    }
  }

  if (EFI_ERROR (Status)) {
    //
    // No shell present.
    //
    if (FvHandleCount) {
      FreePool (FvHandleBuffer);
    }
    return NULL;
  }
  //
  // Build the shell boot option.
  //
  DevicePath = DevicePathFromHandle (FvHandleBuffer[Index]);

  if (FvHandleCount) {
    FreePool (FvHandleBuffer);
  }

  return DevicePath;
}

EFI_STATUS
CreateFvBootOption (
  EFI_GUID                     *FileGuid,
  CHAR16                       *Description,
  EFI_BOOT_MANAGER_LOAD_OPTION *BootOption,
  UINT32                       Attributes,
  UINT8                        *OptionalData,    OPTIONAL
  UINT32                       OptionalDataSize
  )
{
  EFI_STATUS                         Status;
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;
  EFI_FIRMWARE_VOLUME2_PROTOCOL      *Fv;
  UINT32                             AuthenticationStatus;
  VOID                               *Buffer;
  UINTN                              Size;

  if ((BootOption == NULL) || (FileGuid == NULL) || (Description == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  EfiInitializeFwVolDevicepathNode (&FileNode, FileGuid);

  Status = gBS->HandleProtocol (
                  gImageHandle,
                  &gEfiLoadedImageProtocolGuid,
                  (VOID **) &LoadedImage
                  );
  if (!EFI_ERROR (Status)) {
    Status = gBS->HandleProtocol (
                    LoadedImage->DeviceHandle,
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID **) &Fv
                    );
    if (!EFI_ERROR (Status)) {
      Buffer  = NULL;
      Size    = 0;
      Status  = Fv->ReadSection (
                      Fv,
                      FileGuid,
                      EFI_SECTION_PE32,
                      0,
                      &Buffer,
                      &Size,
                      &AuthenticationStatus
                      );
      if (Buffer != NULL) {
        FreePool (Buffer);
      }
    }
  }
    if (EFI_ERROR (Status)) {
      return EFI_NOT_FOUND;
    }

    DevicePath = AppendDevicePathNode (
                   DevicePathFromHandle (LoadedImage->DeviceHandle),
                   (EFI_DEVICE_PATH_PROTOCOL *) &FileNode
                   );

  Status = EfiBootManagerInitializeLoadOption (
             BootOption,
             LoadOptionNumberUnassigned,
             LoadOptionTypeBoot,
             Attributes,
             Description,
             DevicePath,
             OptionalData,
             OptionalDataSize
             );
  FreePool (DevicePath);
  return Status;
}

/**
  Return the index of the load option in the load option array.

  The function consider two load options are equal when the 
  OptionType, Attributes, Description, FilePath and OptionalData are equal.

  @param Key    Pointer to the load option to be found.
  @param Array  Pointer to the array of load options to be found.
  @param Count  Number of entries in the Array.

  @retval -1          Key wasn't found in the Array.
  @retval 0 ~ Count-1 The index of the Key in the Array.
**/
INTN
PlatformFindLoadOption (
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Key,
  IN CONST EFI_BOOT_MANAGER_LOAD_OPTION *Array,
  IN UINTN                              Count
  )
{
  UINTN                             Index;

  for (Index = 0; Index < Count; Index++) {
    if ((Key->OptionType == Array[Index].OptionType) &&
        (Key->Attributes == Array[Index].Attributes) &&
        (StrCmp (Key->Description, Array[Index].Description) == 0) &&
        (CompareMem (Key->FilePath, Array[Index].FilePath, GetDevicePathSize (Key->FilePath)) == 0) &&
        (Key->OptionalDataSize == Array[Index].OptionalDataSize) &&
        (CompareMem (Key->OptionalData, Array[Index].OptionalData, Key->OptionalDataSize) == 0)) {
      return (INTN) Index;
    }
  }

  return -1;
}

UINTN
RegisterFvBootOption (
  EFI_GUID       *FileGuid,
  CHAR16         *Description,
  UINTN          Position,
  UINT32         Attributes,
  UINT8          *OptionalData,    OPTIONAL
  UINT32         OptionalDataSize
  )
{
  EFI_STATUS                       Status;
  UINTN                            OptionIndex;
  EFI_BOOT_MANAGER_LOAD_OPTION     NewOption;
  EFI_BOOT_MANAGER_LOAD_OPTION     *BootOptions;
  UINTN                            BootOptionCount;

  NewOption.OptionNumber = LoadOptionNumberUnassigned;
  Status = CreateFvBootOption (FileGuid, Description, &NewOption, Attributes, OptionalData, OptionalDataSize);
  if (!EFI_ERROR (Status)) {
    BootOptions = EfiBootManagerGetLoadOptions (&BootOptionCount, LoadOptionTypeBoot);

    OptionIndex = PlatformFindLoadOption (&NewOption, BootOptions, BootOptionCount);

    if (OptionIndex == -1) {
      Status = EfiBootManagerAddLoadOptionVariable (&NewOption, Position);
      ASSERT_EFI_ERROR (Status);
    } else {
      NewOption.OptionNumber = BootOptions[OptionIndex].OptionNumber;
    }
    EfiBootManagerFreeLoadOption (&NewOption);
    EfiBootManagerFreeLoadOptions (BootOptions, BootOptionCount);
  }

  return NewOption.OptionNumber;
}


VOID
BootUi (
  VOID
  )
{
  EFI_STATUS                   Status;
  EFI_BOOT_MANAGER_LOAD_OPTION BootOption;

  Status = EfiBootManagerGetBootManagerMenu (&BootOption);
  if (!EFI_ERROR (Status)) {
    EfiBootManagerBoot (&BootOption);
    EfiBootManagerFreeLoadOption (&BootOption);
  }
}

VOID
EFIAPI
PlatformBootManagerWaitCallback (
  UINT16          TimeoutRemain
  )
{
  UINT16                        TimeoutDefault;
  EFI_STATUS                    Status;
  EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL  *TxtInEx;
  EFI_KEY_DATA                  KeyData;
  BOOLEAN                       PausePressed;

  TimeoutDefault = PcdGet16 (PcdPlatformBootTimeOut);
  if (TimeoutDefault == TimeoutRemain) {
    SetMem (&mForeground, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);
    SetMem (&mBackground, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0x0);
    SetMem (&mColor, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL), 0xff);
  }

  PlatformBdsShowProgress (
    mForeground,
    mBackground,
    TmpStr,
    mColor,
    ((TimeoutDefault - TimeoutRemain) * 100 / TimeoutDefault),
    0
    );
        
  //
  // Pause on PAUSE key.
  //
  Status = gBS->HandleProtocol (gST->ConsoleInHandle, &gEfiSimpleTextInputExProtocolGuid, (VOID **) &TxtInEx);
  ASSERT_EFI_ERROR (Status);

  PausePressed = FALSE;

  while (TRUE) {
    Status = TxtInEx->ReadKeyStrokeEx (TxtInEx, &KeyData);
    if (EFI_ERROR (Status)) {
      break;
    }

    if (KeyData.Key.ScanCode == SCAN_PAUSE) {
      PausePressed = TRUE;
      break;
    }
  }

  //
  // Loop until non-PAUSE key pressed.
  //
  while (PausePressed) {
    Status = TxtInEx->ReadKeyStrokeEx (TxtInEx, &KeyData);
    if (!EFI_ERROR (Status)) {
      DEBUG ((
        DEBUG_INFO, "[PauseCallback] %x/%x %x/%x\n",
        KeyData.Key.ScanCode, KeyData.Key.UnicodeChar,
        KeyData.KeyState.KeyShiftState, KeyData.KeyState.KeyToggleState
        ));
      PausePressed = (BOOLEAN) (KeyData.Key.ScanCode == SCAN_PAUSE);
    }
  }
}

/**
  Check if the boot option is a EFI network boot option.

  @param  Option         The boot option need to be processed.

  @retval TRUE           It is a EFI network boot option.
  @retval FALSE          It is not a EFI network boot option.

**/
BOOLEAN
IsEfiNetWorkBootOption (
  IN  EFI_BOOT_MANAGER_LOAD_OPTION     *Option
  )
{
  EFI_DEVICE_PATH_PROTOCOL   *Node;

  for (Node = Option->FilePath; !IsDevicePathEndType (Node); Node = NextDevicePathNode (Node)) {
    if (DevicePathType (Node) == MESSAGING_DEVICE_PATH) {
      switch (DevicePathSubType (Node)) {
      case MSG_MAC_ADDR_DP:
      case MSG_VLAN_DP:
      case MSG_IPv4_DP:
      case MSG_IPv6_DP:
        return TRUE;
      }
    }
  }
  return FALSE;
}

VOID
RegisterDefaultBootOption (
  VOID
  )
{
  EFI_DEVICE_PATH_PROTOCOL           *DevicePath;
  EFI_LOADED_IMAGE_PROTOCOL          *LoadedImage;
  MEDIA_FW_VOL_FILEPATH_DEVICE_PATH  FileNode;
  UINT16                             *ShellData;
  UINT32                             ShellDataSize; 

  //
  // Shell.
  //
  ShellData = NULL;
  ShellDataSize = 0;
  RegisterFvBootOption (&gUefiShellFileGuid, INTERNAL_UEFI_SHELL_NAME,  (UINTN) -1, LOAD_OPTION_ACTIVE, (UINT8 *)ShellData, ShellDataSize);

  //
  // UiApp.
  //
  mUiAppOptionNumber = RegisterFvBootOption (&mUiFile, L"Enter Setup",   (UINTN) -1, (LOAD_OPTION_CATEGORY_APP | LOAD_OPTION_ACTIVE | LOAD_OPTION_HIDDEN), NULL, 0);
  
  if (mUiAppOptionNumber == LoadOptionNumberUnassigned) {
    DEBUG ((DEBUG_ERROR, "UiAppOptionNumber (%d) should not be same to LoadOptionNumberUnassigned(%d).\n", mUiAppOptionNumber, LoadOptionNumberUnassigned));
  }
 
  //
  // Boot Manager Menu.
  //
  EfiInitializeFwVolDevicepathNode (&FileNode, &mBootMenuFile);

  gBS->HandleProtocol (
         gImageHandle,
         &gEfiLoadedImageProtocolGuid,
         (VOID **) &LoadedImage
         );
  DevicePath = AppendDevicePathNode (DevicePathFromHandle (LoadedImage->DeviceHandle), (EFI_DEVICE_PATH_PROTOCOL *) &FileNode);

}

VOID
RegisterBootOptionHotkey (
  UINT16                       OptionNumber,
  EFI_INPUT_KEY                *Key,
  BOOLEAN                      Add
  )
{
  EFI_STATUS                   Status;

  if (!Add) {
    //
    // No enter hotkey when force to setup or there is no boot option.
    //
    Status = EfiBootManagerDeleteKeyOptionVariable (NULL, 0, Key, NULL);
    ASSERT (Status == EFI_SUCCESS || Status == EFI_NOT_FOUND);
  } else {
    //
    // Register enter hotkey for the first boot option.
    //
    Status = EfiBootManagerAddKeyOptionVariable (NULL, OptionNumber, 0, Key,NULL);
    ASSERT (Status == EFI_SUCCESS || Status == EFI_ALREADY_STARTED);
  }
}

EFI_STATUS
EFIAPI
DetectKeypressCallback (
  IN EFI_KEY_DATA     *KeyData
)
{
  mHotKeypressed = TRUE;

  if (HotKeyEvent != NULL) {
    gBS->SignalEvent(HotKeyEvent);
  }

  return EFI_SUCCESS;
}

/**
  This function is called after all the boot options are enumerated and ordered properly.
**/
VOID
RegisterStaticHotkey (
  VOID
  )
{

  EFI_INPUT_KEY                 Enter;
  EFI_KEY_DATA                  F2;
  EFI_KEY_DATA                  F7;
  BOOLEAN                       EnterSetup;
  EFI_STATUS                    Status;
  EFI_BOOT_MANAGER_LOAD_OPTION  BootOption;

  EnterSetup = FALSE;

  //
  // [Enter]
  //
  mContinueBoot = !EnterSetup;
  if (mContinueBoot) {
    Enter.ScanCode    = SCAN_NULL;
    Enter.UnicodeChar = CHAR_CARRIAGE_RETURN;
    EfiBootManagerRegisterContinueKeyOption (0, &Enter, NULL);
  }

  //
  // [F2]/[F7].
  //
  
  F7.Key.ScanCode    = SCAN_F7;
  F7.Key.UnicodeChar = CHAR_NULL;
  F7.KeyState.KeyShiftState = EFI_SHIFT_STATE_VALID;
  F7.KeyState.KeyToggleState = 0;
  Status = EfiBootManagerGetBootManagerMenu (&BootOption);
  ASSERT_EFI_ERROR (Status);
  //
  RegisterBootOptionHotkey ((UINT16) BootOption.OptionNumber, &F7.Key, TRUE);
  EfiBootManagerFreeLoadOption (&BootOption);

  F2.Key.ScanCode    = SCAN_F2;
  F2.Key.UnicodeChar = CHAR_NULL;
  F2.KeyState.KeyShiftState = EFI_SHIFT_STATE_VALID;
  F2.KeyState.KeyToggleState = 0;
  mUiAppBoot  = !EnterSetup;
  RegisterBootOptionHotkey ((UINT16) mUiAppOptionNumber, &F2.Key, mUiAppBoot);
}

UINT8
BootOptionType (
  IN EFI_DEVICE_PATH_PROTOCOL   *DevicePath
  )
{
  EFI_DEVICE_PATH_PROTOCOL      *Node;
  EFI_DEVICE_PATH_PROTOCOL      *NextNode;

  for (Node = DevicePath; !IsDevicePathEndType (Node); Node = NextDevicePathNode (Node)) {
    if (DevicePathType (Node) == MESSAGING_DEVICE_PATH) {
      //
      // Make sure the device path points to the driver device.
      //
      NextNode = NextDevicePathNode (Node);
      if (DevicePathSubType(NextNode) == MSG_DEVICE_LOGICAL_UNIT_DP) {
        //
        // If the next node type is Device Logical Unit, which specify the Logical Unit Number (LUN),
        // skip it.
        //
        NextNode = NextDevicePathNode (NextNode);
      }
      if (IsDevicePathEndType (NextNode)) {
        if ((DevicePathType (Node) == MESSAGING_DEVICE_PATH)) {
          return DevicePathSubType (Node);
        } else {
          return MSG_SATA_DP;
        }
      }
    }
  }

  return (UINT8) -1;
}

/**
  Returns the priority number.
  
  OptionType                 Default Priority
  ------------------------------------
  PXE                         5
  DVD                         2
  USB                         1
  NVME                        4
  HDD                         3
  EFI Shell                   6
  Others                      100

  @param BootOption
**/
UINTN
BootOptionPriority (
  CONST EFI_BOOT_MANAGER_LOAD_OPTION *BootOption
  )
{
    //
    // EFI boot options.
    //
    switch (BootOptionType (BootOption->FilePath)) {
      case MSG_MAC_ADDR_DP:
      case MSG_VLAN_DP:
      case MSG_IPv4_DP:
      case MSG_IPv6_DP:
        return 5;
      case MSG_SATA_DP:
      case MSG_ATAPI_DP:
        return 2;
      case MSG_USB_DP:
        return 1;
    }
    
    if (StrCmp (BootOption->Description, INTERNAL_UEFI_SHELL_NAME) == 0) {
      return 6;
    }
    if (StrCmp (BootOption->Description, UEFI_HARD_DRIVE_NAME) == 0) {
      return 3;
    }
    if (StrCmp (BootOption->Description, UEFI_NVME_DRIVE_NAME) == 0) {
      return 4;
    }
    
    return 100;
}

INTN
EFIAPI
CompareBootOption (
  CONST EFI_BOOT_MANAGER_LOAD_OPTION  *Left,
  CONST EFI_BOOT_MANAGER_LOAD_OPTION  *Right
  )
{
  return BootOptionPriority (Left) - BootOptionPriority (Right);
}
