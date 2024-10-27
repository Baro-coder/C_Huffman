#ifndef BS_PROCEDURES_H
#define BS_PROCEDURES_H

#include <common/common.h>
#include <logs.h>

// Enum - Status Codes
typedef enum STATUS_CODES
{
    STATUS_OK = 0,                       // OK
    STATUS_ERROR = -1,                   // Generic error
    STATUS_ERROR_BAD_USAGE = 1,          // Bad usage
    STATUS_ERROR_FILE_NOT_FOUND = 2,     // File not found
    STATUS_ERROR_FILE_CANNOT_CREATE = 3, // Output file cannot be created
    STATUS_ERROR_DECOMPRESS = 4,         // Error decompressing file
    STATUS_ERROR_COMPRESS = 5,           // Error compressing file
    STATUS_ERROR_CRC = 6,                // CRC32 checksum error
    STATUS_ERROR_ASCII = 7               // ASCII error
} STATUS;

// **********************************************************************
// Functions

/// @brief Compress a file
/// @param filepath Path to the file to compress
/// @return STATUS_OK if the file was compressed successfully
/// @return STATUS_ERROR_FILE_NOT_FOUND if the file was not found
/// @return STATUS_ERROR_FILE_CANNOT_CREATE if the output file cannot be created
/// @return STATUS_ERROR_COMPRESS if an error occurred while compressing the file
/// @return STATUS_ERROR if an error occurred
STATUS procedure_compress(char *filepath);

/// @brief  Decompress a file
/// @param filepath Path to the compressed file
/// @return STATUS_OK if the file was decompressed successfully
/// @return STATUS_ERROR_FILE_NOT_FOUND if the file was not found
/// @return STATUS_ERROR_FILE_CANNOT_CREATE if the output file cannot be created
/// @return STATUS_ERROR_DECOMPRESS if an error occurred while decompressing the file
/// @return STATUS_ERROR_CRC if the CRC32 checksum error
/// @return STATUS_ERROR if an error occurred
STATUS procedure_decompress(char *filepath);

/// @brief Calculate CRC32 checksum of a file
/// @param filepath Path to the file
/// @return STATUS_CRC_CALC_OK if the CRC32 checksum was calculated successfully
/// @return STATUS_CRC_CALC_ERROR_FILE_NOT_FOUND if the file was not found
STATUS procedure_crc_calc(char *filepath);

#endif