#ifndef BS_CODING_TREE_H
#define BS_CODING_TREE_H

#include <common/common.h>
#include <logs.h>

// **********************************************************************
// Functions

/// @brief Dump the tree into a file
/// @param root Tree root
/// @param filepath Path to the output file
/// @return `0` if the tree was dumped successfully, `-1` if the file cannot be created, `-2` if the tree is empty
int dump_tree_to_file(node_t **root, const char *filepath);

/// @brief Build the coding tree
/// @param symbols_count Number of symbols
/// @param symbols_arr Array of symbols
/// @param nodes_count Number of tree nodes
/// @return Root of the tree
node_t *build_coding_tree(int symbols_count, ism_t **symbols_arr, unsigned int *nodes_count);

/// @brief Load the coding tree from a file
/// @param filepath Path to the input file
/// @param nodes_count Number of tree nodes
/// @param bytes_read Number of bytes read from the file
/// @return Root of the tree
node_t *load_tree_from_file(const char *filepath, unsigned int *nodes_count, unsigned int *bytes_read);

#endif