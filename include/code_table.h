#ifndef BS_CODE_TABLE_H
#define BS_CODE_TABLE_H

// Local libraries
#include <coding_tree.h>
#include <constants.h>
#include <logs.h>

// Struct - Code Table Entry
typedef struct _code_t
{
    char symbol;            // Symbol
    unsigned int sequence;  // Binary Sequence
    unsigned int word_size; // Binary Sequence Size
} code_t;

// **********************************************************************
// Functions

/// @brief Dump the code table to the file
/// @param code_table Code table
/// @param code_table_size Code table size
/// @param filepath Output file path
/// @return `0` if the code table was dumped successfully, `-1` if the file cannot be created, `-2` if the code table is empty or `-3` if the code table is `NULL`
int dump_code_table_to_file(code_t ***code_table, int code_table_size, const char *filepath);

/// @brief Build the code table
/// @param tree_root Root of the coding tree
/// @param code_table Code table
/// @param bits_count Total bits count
/// @return Code table size or `-1` if the tree is empty
int build_code_table(node_t **tree_root, code_t ***code_table, unsigned int *bits_count);

#endif