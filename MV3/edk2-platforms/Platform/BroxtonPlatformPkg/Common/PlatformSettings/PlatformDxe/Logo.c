/** @file
  Platform Initialization Driver.

  Copyright (c) 1999 - 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <IndustryStandard/Bmp.h>
#include <PiDxe.h>
#include <Protocol/SimpleTextOut.h>
#include <Protocol/OEMBadging.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/UgaDraw.h>
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeServicesLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>

#include "Logo.h"


/**
  Convert a *.BMP graphics image to a GOP blt buffer. If a NULL Blt buffer
  is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
  buffer is passed in it will be used if it is big enough.

  @param  BmpImage      Pointer to BMP file
  @param  BmpImageSize  Number of bytes in BmpImage
  @param  GopBlt        Buffer containing GOP version of BmpImage.
  @param  GopBltSize    Size of GopBlt in bytes.
  @param  PixelHeight   Height of GopBlt/BmpImage in pixels
  @param  PixelWidth    Width of GopBlt/BmpImage in pixels

  @retval EFI_SUCCESS           GopBlt and GopBltSize are returned.
  @retval EFI_UNSUPPORTED       BmpImage is not a valid *.BMP image
  @retval EFI_BUFFER_TOO_SMALL  The passed in GopBlt buffer is not big enough.
                                    GopBltSize will contain the required size.
  @retval EFI_OUT_OF_RESOURCES  No enough buffer to allocate.

**/
EFI_STATUS
EfiShellConvertBmpToGopBlt (
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
  OUT UINTN     *PixelHeight,
  OUT UINTN     *PixelWidth
  )
{
  UINT8                         *Image;
  UINT8                         *ImageHeader;
  BMP_IMAGE_HEADER              *BmpHeader;
  BMP_COLOR_MAP                 *BmpColorMap;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINT64                        BltBufferSize;
  UINTN                         Index;
  UINTN                         Height;
  UINTN                         Width;
  UINTN                         ImageIndex;
  UINT32                        DataSizePerLine;
  BOOLEAN                       IsAllocated;
  UINT32                        ColorMapNum;

  if (sizeof (BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

BmpHeader = (BMP_IMAGE_HEADER *) BmpImage;

if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M') {
  return EFI_UNSUPPORTED;
}

//
// Doesn't support compress.
//
if (BmpHeader->CompressionType != 0) {
  return EFI_UNSUPPORTED;
}

//
// Only support BITMAPINFOHEADER format.
// BITMAPFILEHEADER + BITMAPINFOHEADER = BMP_IMAGE_HEADER
//
if (BmpHeader->HeaderSize != sizeof (BMP_IMAGE_HEADER) - OFFSET_OF(BMP_IMAGE_HEADER, HeaderSize)) {
  return EFI_UNSUPPORTED;
}

//
// The data size in each line must be 4 byte alignment.
//
DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
BltBufferSize = MultU64x32 (DataSizePerLine, BmpHeader->PixelHeight);
if (BltBufferSize > (UINT32) ~0) {
  return EFI_INVALID_PARAMETER;
}

if ((BmpHeader->Size != BmpImageSize) || 
    (BmpHeader->Size < BmpHeader->ImageOffset) ||
    (BmpHeader->Size - BmpHeader->ImageOffset !=	BmpHeader->PixelHeight * DataSizePerLine)) {
  return EFI_INVALID_PARAMETER;
}

//
// Calculate Color Map offset in the image.
//
Image       = BmpImage;
BmpColorMap = (BMP_COLOR_MAP *) (Image + sizeof (BMP_IMAGE_HEADER));
if (BmpHeader->ImageOffset < sizeof (BMP_IMAGE_HEADER)) {
  return EFI_INVALID_PARAMETER;
}

if (BmpHeader->ImageOffset > sizeof (BMP_IMAGE_HEADER)) {
  switch (BmpHeader->BitPerPixel) {
    case 1:
      ColorMapNum = 2;
      break;
    case 4:
      ColorMapNum = 16;
      break;
    case 8:
      ColorMapNum = 256;
      break;
    default:
      ColorMapNum = 0;
      break;
    }
  //
  // BMP file may has padding data between the bmp header section and the bmp data section.
  //
  if (BmpHeader->ImageOffset - sizeof (BMP_IMAGE_HEADER) < sizeof (BMP_COLOR_MAP) * ColorMapNum) {
    return EFI_INVALID_PARAMETER;
  }
 }

 //
 // Calculate graphics image data address in the image
 //
 Image         = ((UINT8 *) BmpImage) + BmpHeader->ImageOffset;
 ImageHeader   = Image;

 //
 // Calculate the BltBuffer needed size.
 //
 BltBufferSize = MultU64x32 ((UINT64) BmpHeader->PixelWidth, BmpHeader->PixelHeight);
 //
 // Ensure the BltBufferSize * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL) doesn't overflow
 //
 if (BltBufferSize > DivU64x32 ((UINTN) ~0, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
   return EFI_UNSUPPORTED;
 }
 BltBufferSize = MultU64x32 (BltBufferSize, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

 IsAllocated   = FALSE;
 if (*GopBlt == NULL) {
   //
   // GopBlt is not allocated by caller.
   //
   *GopBltSize = (UINTN) BltBufferSize;
   *GopBlt     = AllocatePool (*GopBltSize);
   IsAllocated = TRUE;
   if (*GopBlt == NULL) {
     return EFI_OUT_OF_RESOURCES;
   }
 } else {
   //
   // GopBlt has been allocated by caller.
   //
   if (*GopBltSize < (UINTN) BltBufferSize) {
     *GopBltSize = (UINTN) BltBufferSize;
     return EFI_BUFFER_TOO_SMALL;
   }
 }

 *PixelWidth	= BmpHeader->PixelWidth;
 *PixelHeight	= BmpHeader->PixelHeight;

 //
 // Convert image from BMP to Blt buffer format
 //
 BltBuffer = *GopBlt;
 for (Height = 0; Height < BmpHeader->PixelHeight; Height++) {
   Blt = &BltBuffer[(BmpHeader->PixelHeight - Height - 1) * BmpHeader->PixelWidth];
   for (Width = 0; Width < BmpHeader->PixelWidth; Width++, Image++, Blt++) {
     switch (BmpHeader->BitPerPixel) {
     case 1:
       //
       // Convert 1-bit (2 colors) BMP to 24-bit color
       //
       for (Index = 0; Index < 8 && Width < BmpHeader->PixelWidth; Index++) {
         Blt->Red	  = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Red;
         Blt->Green  = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Green;
         Blt->Blue   = BmpColorMap[((*Image) >> (7 - Index)) & 0x1].Blue;
         Blt++;
         Width++;
       }
    
       Blt--;
       Width--;
       break;

     case 4:
       //
       // Convert 4-bit (16 colors) BMP Palette to 24-bit color
       //
       Index       = (*Image) >> 4;
       Blt->Red    = BmpColorMap[Index].Red;
       Blt->Green  = BmpColorMap[Index].Green;
       Blt->Blue   = BmpColorMap[Index].Blue;
       if (Width < (BmpHeader->PixelWidth - 1)) {
         Blt++;
         Width++;
         Index       = (*Image) & 0x0f;
         Blt->Red    = BmpColorMap[Index].Red;
         Blt->Green  = BmpColorMap[Index].Green;
         Blt->Blue   = BmpColorMap[Index].Blue;
       }
       break;

     case 8:
       //
       // Convert 8-bit (256 colors) BMP Palette to 24-bit color
       //
       Blt->Red    = BmpColorMap[*Image].Red;
       Blt->Green  = BmpColorMap[*Image].Green;
       Blt->Blue   = BmpColorMap[*Image].Blue;
       break;

     case 24:
       //
       // It is 24-bit BMP.
       //
       Blt->Blue   = *Image++;
       Blt->Green  = *Image++;
       Blt->Red    = *Image;
       break;

     default:
       //
       // Other bit format BMP is not supported.
       //
       if (IsAllocated) {
         FreePool (*GopBlt);
         *GopBlt = NULL;
        }
        return EFI_UNSUPPORTED;
      };

    }

    ImageIndex = (UINTN)Image - (UINTN)ImageHeader;
    if ((ImageIndex % 4) != 0) {
      //
      // Bmp Image starts each row on a 32-bit boundary!
      //
      Image = Image + (4 - (ImageIndex % 4));
    }
  }

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
WelcomeLineDisplay (
  IN CHAR16 * WelcomeLine
  ) 
{
  UINTN Length;
  UINTN Line;
  UINTN Columns;
  UINTN Rows;
  UINTN Column;
  EFI_STATUS Status;
  EFI_SIMPLE_TEXT_OUTPUT_MODE SavedConMode;

  Length = StrLen(WelcomeLine);
  Line = 1;
  CopyMem(&SavedConMode, gST->ConOut->Mode, sizeof (SavedConMode));
  gST->ConOut->QueryMode(gST->ConOut, SavedConMode.Mode, &Columns, &Rows);
  Column = (Columns-Length)/2;
  gST->ConOut->SetAttribute(gST->ConOut, EFI_YELLOW);
  gST->ConOut->SetCursorPosition(gST->ConOut, Column, SavedConMode.CursorRow);
  gST->ConOut->OutputString(gST->ConOut, WelcomeLine);
  gST->ConOut->EnableCursor      (gST->ConOut, SavedConMode.CursorVisible);
  gST->ConOut->SetCursorPosition (gST->ConOut, SavedConMode.CursorColumn, (SavedConMode.CursorRow+1));
  Status = gST->ConOut->SetAttribute     (gST->ConOut, SavedConMode.Attribute);
  return Status;
}

EFI_STATUS
EFIAPI
LogoDisplay (
  IN  EFI_GUID  *LogoFile
  )
{
  EFI_STATUS                    Status;
  UINT32                        SizeOfX;
  UINT32                        SizeOfY;
  INTN                          DestX;
  INTN                          DestY;
  UINT8                         *ImageData;
  UINTN                         ImageSize;
  UINTN                         BltSize;
  EFI_BADGING_DISPLAY_ATTRIBUTE Attribute;
  UINTN                         CoordinateX;
  UINTN                         CoordinateY;
  UINTN                         Height;
  UINTN                         Width;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  EFI_UGA_DRAW_PROTOCOL         *UgaDraw;
  UINT32                        ColorDepth;
  UINT32                        RefreshRate;
  EFI_GRAPHICS_OUTPUT_PROTOCOL  *GraphicsOutput;
  UINTN                         LogoDestX;
  UINTN                         LogoDestY;
  UINTN                         LogoHeight;
  UINTN                         LogoWidth;
      
  UgaDraw = NULL;
  //
  // Try to open GOP first
  //
  Status = gBS->LocateProtocol (&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **) &GraphicsOutput);
  if (EFI_ERROR (Status) && FeaturePcdGet (PcdUgaConsumeSupport)) {
    GraphicsOutput = NULL;
    //
    // Open GOP failed, try to open UGA
    //
    Status = gBS->HandleProtocol (gST->ConsoleOutHandle, &gEfiUgaDrawProtocolGuid, (VOID **) &UgaDraw);
  }
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Try to open Boot Logo Protocol.
  //

  if (GraphicsOutput != NULL) {
    SizeOfX = GraphicsOutput->Mode->Info->HorizontalResolution;
    SizeOfY = GraphicsOutput->Mode->Info->VerticalResolution;

  } else if (UgaDraw != NULL && FeaturePcdGet (PcdUgaConsumeSupport)) {
    Status = UgaDraw->GetMode (UgaDraw, &SizeOfX, &SizeOfY, &ColorDepth, &RefreshRate);
    if (EFI_ERROR (Status)) {
      return EFI_UNSUPPORTED;
    }
  } else {
    return EFI_UNSUPPORTED;
  }

  Blt = NULL;
  LogoDestX = 0;
  LogoDestY = 0;
  LogoHeight = 0;
  LogoWidth = 0;
      
  ImageData = NULL;
  ImageSize = 0;

  //
  // Get the specified image from FV.
  //
  Status = GetSectionFromAnyFv (LogoFile, EFI_SECTION_RAW, 0, (VOID **) &ImageData, &ImageSize);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  CoordinateX = 0;
  CoordinateY = 0;
  Attribute   = EfiBadgingDisplayAttributeRightTop;

  if (Blt != NULL) {
    FreePool (Blt);
  }

  //
  // Try BMP decoder
  //
  Blt = NULL;
  Status = EfiShellConvertBmpToGopBlt (
             ImageData,
             ImageSize,
             (VOID **) &Blt,
             &BltSize,
             &Height,
             &Width
             );

  if (EFI_ERROR (Status)) {
    FreePool (ImageData);
  }

  //
  // Calculate the display position according to Attribute.
  //
  switch (Attribute) {
    case EfiBadgingDisplayAttributeLeftTop:
      DestX = CoordinateX;
      DestY = CoordinateY;
      break;

    case EfiBadgingDisplayAttributeCenterTop:
      DestX = (SizeOfX - Width) / 2;
      DestY = CoordinateY;
      break;

    case EfiBadgingDisplayAttributeRightTop:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = CoordinateY;;
      break;

    case EfiBadgingDisplayAttributeCenterRight:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = (SizeOfY - Height) / 2;
      break;

    case EfiBadgingDisplayAttributeRightBottom:
      DestX = (SizeOfX - Width - CoordinateX);
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeCenterBottom:
      DestX = (SizeOfX - Width) / 2;
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeLeftBottom:
      DestX = CoordinateX;
      DestY = (SizeOfY - Height - CoordinateY);
      break;

    case EfiBadgingDisplayAttributeCenterLeft:
      DestX = CoordinateX;
      DestY = (SizeOfY - Height) / 2;
      break;

    case EfiBadgingDisplayAttributeCenter:
      DestX = (SizeOfX - Width) / 2;
      DestY = (SizeOfY - Height) / 2;
      break;

    default:
      DestX = CoordinateX;
      DestY = CoordinateY;
      break;
    }

  if ((DestX >= 0) && (DestY >= 0)) {
    if (GraphicsOutput != NULL) {
      Status = GraphicsOutput->Blt (
                                 GraphicsOutput,
                                 Blt,
                                 EfiBltBufferToVideo,
                                 0,
                                 0,
                                 (UINTN) DestX,
                                 (UINTN) DestY,
                                 Width,
                                 Height,
                                 Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)
                                 );
    } else if (UgaDraw != NULL && FeaturePcdGet (PcdUgaConsumeSupport)) {
      Status = UgaDraw->Blt (
                          UgaDraw,
                          (EFI_UGA_PIXEL *) Blt,
                          EfiUgaBltBufferToVideo,
                          0,
                          0,
                          (UINTN) DestX,
                          (UINTN) DestY,
                          Width,
                          Height,
                          Width * sizeof (EFI_UGA_PIXEL)
                          );
    } else {
      Status = EFI_UNSUPPORTED;
    }

   }

   FreePool (ImageData);

   FreePool (Blt);

   return Status;
 }


VOID
EFIAPI
EfiShellProtocolCallback (
  IN EFI_EVENT Event,
  IN VOID *Context
)
{
  CHAR16 *Welcome;
  Welcome = L"Welcome to https://minnowboard.org/setup";
  WelcomeLineDisplay(Welcome);
  LogoDisplay(PcdGetPtr (PcdLogoFileGuid));
  return;
}

