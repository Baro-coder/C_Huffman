#ifndef BS_HUFFMAN_H
#define BS_HUFFMAN_H

// Global libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>

// Local libraries
#include <procedures.h>
#include <info_src_model.h>
#include <coding_tree.h>
#include <code_table.h>
#include <compress.h>
#include <decompress.h>
#include <crc.h>
#include <constants.h>
#include <logs.h>

/// Constants

// Modes
#define MODE_COMPRESS "-c"   // Compress mode
#define MODE_DECOMPRESS "-d" // Decompress mode
#define MODE_CRC_CALC "-k"   // CRC32 checksum calculation mode

// Functions

/// @brief Swap endianess of a number from the little-endian to big-endian and vice-versa
/// @param num Number to swap endianess
/// @return Swapped number
int swap_endian(int num);

#endif