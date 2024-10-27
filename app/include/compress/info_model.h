#ifndef BS_INFO_SRC_MODEL_H
#define BS_INFO_SRC_MODEL_H

#include <compress/compress.h>
#include <logs.h>

// Functions

/// @brief Dump the info source model into a file
/// @param symbols_arr Address of an array to dump
/// @param filepath File to write
/// @return `0` if the model was dumped successfully or `-1` if an error occurred
int dump_model_to_file(ism_t ***symbols_arr, const char *filepath);

/// @brief Build the info source model from a file and store it in an array
/// @param filepath File to read
/// @param symbols_arr Address of an array to store the symbols
/// @return `0` if the model was built successfully or `-1` if an error occurred
int build_info_model(FILE *filepath, ism_t ***symbols_arr);

/// @brief Sort the info source model array
/// @param symbols_arr Address of an array to sort
/// @return Number of elements in the array or `-1` if an error occurred
int sort_model(ism_t ***symbols_arr);

#endif