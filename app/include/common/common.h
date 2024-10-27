#ifndef BS_COMMON_H
#define BS_COMMON_H

// Global libraries
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/time.h>

// Local libraries
#include <common/structures.h>
#include <common/constants.h>
#include <common/procedures.h>
#include <common/coding_tree.h>
#include <common/crc.h>

#include <compress/compress.h>

#include <decompress/decompress.h>

#include <logs.h>

// Functions

/// @brief Swap endianess of a number from the little-endian to big-endian and vice-versa
/// @param num Number to swap endianess
/// @return Swapped number
int swap_endian(int num);

#endif