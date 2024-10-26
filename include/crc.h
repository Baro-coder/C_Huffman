#ifndef BS_CRC_H
#define BS_CRC_H

// Global libraries
#include <stdio.h>

// Local libraries
#include <huffman.h>
#include <constants.h>
#include <logs.h>

// CRC32 initial value
#define CRC32_INIT 0xFFFFFFFF
// CRC32 Polynomial Standard
#define CRC32_POLYNOMIAL_STD 0x04C11DB7
// CRC32 Polynomial Reversed
#define CRC32_POLYNOMIAL_REV 0xEDB88320

/// **********************************************************************
/// Functions

/// @brief Calculate CRC32 checksum of a buffer
/// @param buf Data buffer
/// @param len Buffer length
/// @return CRC32 checksum
int crc32(const unsigned char *buf, int len);

/// @brief Calculate CRC32 checksum of a file
/// @param filename Path to the input file
/// @return CRC32 checksum or `-1` if the file was not found
int crc32_file(const char *filename);

/// @brief Read CRC32 checksum from the end of a file
/// @param filename Path to the input file
/// @return CRC32 checksum or `-1` if the file was not found
int crc32_file_read(const char *filename);

#endif