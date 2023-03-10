/** @file
  Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __EFI_USB_FUIO_MEM_NODE__
#define __EFI_USB_FUIO_MEM_NODE__

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/DriverLib.h>

#define  USB_DEBUG_MEM_NODE_INFO  EFI_D_INIT
#define  USB_DEBUG_MEM_NODE_ERROR EFI_D_ERROR


typedef struct {
  UINTN                         Size;
  VOID                          *AllocatePtr;
  VOID                          *NextPtr;
} USB_MEM_NODE;

/**
  The SearchNode function search a memory address for record the driver allocate
  memory region and the node to the head link list.

  @param[in]  This              A pointer to the EFI_USBFN_IO_PROTOCOL instance.
  @param[in]  Buffer            The driver alocate memory address.
  @param[out] Node              The match node record of the driver aloocate
                                memory region.
  @param[out] PNode             The pervious match node record of the driver
                                aloocate memory region.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_NOT_FOUND         The memory Buffer didn't find.
**/
EFI_STATUS
SearchNode (
  IN  EFI_USBFN_IO_PROTOCOL    *This,
  IN  VOID                     *Buffer,
  OUT USB_MEM_NODE             **Node,
  OUT USB_MEM_NODE             **PNode
  );

/**
  The InsertNewNodeToHead function remove a memory for record the driver allocate
  memory region and the node to the head link list.

  @param[in]  This              A pointer to the EFI_USBFN_IO_PROTOCOL instance.
  @param[in]  Buffer            The driver alocate memory address.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_NOT_FOUND         The memory Buffer didn't find.
**/
EFI_STATUS
RemoveNode (
  IN  EFI_USBFN_IO_PROTOCOL    *This,
  IN  VOID                     *Buffer
  );

/**
  The InsertNewNodeToHead function allocates a memory for record the driver allocate
  memory region and insert the node to the head link list.

  @param[in]  This              A pointer to the EFI_USBFN_IO_PROTOCOL instance.
  @param[out] USB_MEM_NODE      return the new node address.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_INVALID_PARAMETER A parameter is invalid.
  @retval EFI_OUT_OF_RESOURCES  The requested transfer Buffer could not be allocated.

**/
EFI_STATUS
InsertNewNodeToHead (
  IN  EFI_USBFN_IO_PROTOCOL    *This,
  OUT USB_MEM_NODE             **Node
  );

  #endif


