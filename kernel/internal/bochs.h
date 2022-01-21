//
// Created by KrisnaPranav on 21/01/22.
//

#pragma once

#include "graphics.h"

namespace Kernel {
    #define VBE_DISPI_BANK_ADDRESS 0x10000
    #define VBE_DISPI_BANK_SIZE_KB 64
    #define VBE_DISPI_MAX_XRES 1024
    #define VBE_DISPI_MAX_YRES 768
    #define VBE_DISPI_IOPORT_INDEX          0x01CE
    #define VBE_DISPI_IOPORT_DATA           0x01CF

    #define VBE_DISPI_INDEX_ID              0x0
    #define VBE_DISPI_INDEX_XRES            0x1
    #define VBE_DISPI_INDEX_YRES            0x2
    #define VBE_DISPI_INDEX_BPP             0x3
    #define VBE_DISPI_INDEX_ENABLE          0x4
    #define VBE_DISPI_INDEX_BANK            0x5
    #define VBE_DISPI_INDEX_VIRT_WIDTH      0x6
    #define VBE_DISPI_INDEX_VIRT_HEIGHT     0x7
    #define VBE_DISPI_INDEX_X_OFFSET        0x8
    #define VBE_DISPI_INDEX_Y_OFFSET        0x9
}