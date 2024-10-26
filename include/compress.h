#ifndef BS_COMPRESS_H
#define BS_COMPRESS_H

// Global libraries
#include <stdio.h>
#include <string.h>

// Local libraries
#include <code_table.h>
#include <constants.h>
#include <logs.h>

/// File sizes handlers
extern unsigned int in_file_size;
extern unsigned int out_file_size;

/// funtion implemented in huffman.c
extern int swap_endian(int num);

// **********************************************************************
// Functions

/// @brief Compress a file using the Huffman algorithm and save the compressed data in a new file
/// @param filepath Path to the file to compress
/// @param output_filepath Path to the output file
/// @param tree_root Root of the coding tree
/// @param nodes_count Number of nodes of the coding tree
/// @param code_table Code table
/// @param bits_count Number of bits of the compressed data
/// @param in_file_size Size of the input file pointer
/// @param out_file_size Size of the output file pointer
/// @return 0 if the file was compressed successfully
/// @return 1 if the file was not found
/// @return 2 if the output file cannot be created
/// @return 3 if an error occurred while compressing the file
int compress_file(const char *input_filepath, const char *output_filepath, node_t **tree_root, unsigned int nodes_count, code_t ***code_table, unsigned int bits_count, unsigned int *in_file_size, unsigned int *out_file_size);

#endif