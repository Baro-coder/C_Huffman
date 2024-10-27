#ifndef BS_DECOMPRESS_H
#define BS_DECOMPRESS_H

#include <common/common.h>
#include <common/coding_tree.h>

#include <logs.h>

/// @brief Decompress a file
/// @param filepath Path to the file to decompress
/// @param output_filepath Path to the output file
/// @param file_offset Bytes to skip at the beginning of the file
/// @param tree_root Coding tree root
/// @param bits_count Compression bits count
/// @return 0 if the file was decompressed successfully
/// @return 1 if the file was not found
/// @return 2 if the output file cannot be created
/// @return 3 if an error occurred while compressing the file
int decompress_file(const char *input_filepath, const char *output_filepath, unsigned int file_offset, node_t *tree_root, unsigned int *bits_count);

#endif