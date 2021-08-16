/* $NetBSD: unorddf2.c,v 1.1 2003/05/06 08:58:19 rearnsha Exp $ */
/** @file
*
*  Copyright (c) 2013 - 2014, ARM Limited. All rights reserved.
*
*  This program and the accompanying materials
*  are licensed and made available under the terms and conditions of the BSD License
*  which accompanies this distribution.  The full text of the license may be found at
*  http://opensource.org/licenses/bsd-license.php
*
*  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
*  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
*
**/
/*
 * Written by Richard Earnshaw, 2003.  This file is in the Public Domain.
 */

#include "softfloat-for-gcc.h"
#include "milieu.h"
#include "softfloat.h"

#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: unorddf2.c,v 1.1 2003/05/06 08:58:19 rearnsha Exp $");
#endif /* LIBC_SCCS and not lint */

flag __unorddf2(float64, float64);

flag
__unorddf2(float64 a, float64 b)
{
    /*
     * The comparison is unordered if either input is a NaN.
     * Test for this by comparing each operand with itself.
     * We must perform both comparisons to correctly check for
     * signalling NaNs.
     */
    return 1 ^ (float64_eq(a, a) & float64_eq(b, b));
}
