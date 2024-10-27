/*
 *  Copyright (c) 2024 Baro-coder
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the MIT LICENSE. See `huffman.c` or `https://github.com/Baro-coder/C_Huffman?tab=MIT-1-ov-file` for details.
 */

#ifndef BS_HUFFMAN_H
#define BS_HUFFMAN_H

// Local libraries
#include <common/common.h>
#include <compress/compress.h>
#include <decompress/decompress.h>
#include <logs.h>

// Modes
#define MODE_HELP "-h"       // Help mode
#define MODE_VERSION "-v"    // Version mode
#define MODE_COMPRESS "-c"   // Compress mode
#define MODE_DECOMPRESS "-d" // Decompress mode
#define MODE_CRC_CALC "-k"   // CRC32 checksum calculation mode

#endif