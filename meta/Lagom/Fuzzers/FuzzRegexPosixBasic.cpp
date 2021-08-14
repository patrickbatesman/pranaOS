/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/StringView.h>
#include <libregex/Regex.h>
#include <stddef.h>
#include <stdint.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    auto pattern = StringView(static_cast<const unsigned char*>(data), size);
    [[maybe_unused]] auto re = Regex<PosixBasic>(pattern);
    return 0;
}
