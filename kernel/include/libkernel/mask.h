/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#ifndef _KERNEL_LIBKERNEL_MASK_H
#define _KERNEL_LIBKERNEL_MASK_H

#define MASKDEFINE(N, P, S) \
    N##_POS = (P),          \
    N##_SIZE = (S),         \
    N##_MASK = ((~(~0 << (S))) << (P))

#define TOKEN_PASTE_IMPL(x, y) x ## y
#define TOKEN_PASTE(x, y) TOKEN_PASTE_IMPL(x, y)
#define SKIP(x, y) char TOKEN_PASTE(prefix, __LINE__)[y - x - 8]

#endif 