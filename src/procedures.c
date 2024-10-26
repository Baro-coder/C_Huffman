#include <procedures.h>

// **********************************************************************
// Files

static STATUS _check_file(const char *filepath, FILE **inFile)
{
    logs_log_debug(USER, "Checking file ... ");
    FILE *file = fopen(filepath, "rb");
    if (!file)
    {
        logs_log_error(USER, "Error opening file!");
        return STATUS_ERROR_FILE_NOT_FOUND;
    }
    fclose(file);
    logs_log_debug(USER, " + File check passed.");
    *inFile = fopen(filepath, "rb");
    return STATUS_OK;
}

static STATUS _check_ASCII(const char *filepath, FILE **inFile)
{
    logs_log_debug(USER, "Checking ASCII...");
    *inFile = fopen(filepath, "rb");
    if (!(*inFile))
    {
        logs_log_error(USER, "Error opening file!");
        return STATUS_ERROR_FILE_NOT_FOUND;
    }

    int b; // read byte
    while ((b = fgetc(*inFile)) != EOF)
    {
        printf("%c", b);
        if (b > 127)
        {
            printf("\n");
            fclose(*inFile);
            logs_log_error(USER, "Error! File is not ASCII!");
            return STATUS_ERROR_ASCII;
        }
    }
    printf("\n");

    logs_log_debug(USER, " + ASCII check passed.");
    fclose(*inFile);
    *inFile = fopen(filepath, "rb");
    return 0;
}

static STATUS _build_info_model_dump_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the info model dumpfile path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_MODELS) + strlen(input_filepath) + SUFIX_MODEL_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_MODELS);
    strcat(*output_filepath, filename);
    strcat(*output_filepath, SUFIX_MODEL);

    logs_log_debug(USER, " - Model dumpfile path: %s", *output_filepath);

    return STATUS_OK;
}

static STATUS _build_tree_dump_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the tree dumpfile path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_TREES) + strlen(input_filepath) + SUFIX_CODING_TREE_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_TREES);
    strcat(*output_filepath, filename);
    strcat(*output_filepath, SUFIX_CODING_TREE);

    logs_log_debug(USER, " - Tree dumpfile path: %s", *output_filepath);

    return STATUS_OK;
}

static STATUS _build_code_table_dump_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the code table dumpfile path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_TABLES) + strlen(input_filepath) + SUFIX_CODE_TABLE_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_TABLES);
    strcat(*output_filepath, filename);
    strcat(*output_filepath, SUFIX_CODE_TABLE);

    logs_log_debug(USER, " - Code table dumpfile path: %s", *output_filepath);

    return STATUS_OK;
}

static STATUS _build_crc_dump_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the CRC32 dumpfile path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_CRC) + strlen(input_filepath) + SUFIX_CRC_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_CRC);
    strcat(*output_filepath, filename);
    strcat(*output_filepath, SUFIX_CRC);

    logs_log_debug(USER, " - CRC dumpfile path: %s", *output_filepath);

    return STATUS_OK;
}

static STATUS _build_compress_output_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the output file path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_DECOMPRESSED) + strlen(input_filepath) + SUFIX_COMPRESSED_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_COMPRESSED);
    strcat(*output_filepath, filename);
    strcat(*output_filepath, SUFIX_COMPRESSED);

    logs_log_debug(USER, " - Output file path: %s", *output_filepath);

    return STATUS_OK;
}

static STATUS _build_decompress_output_filepath(const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "Building the output file path ... ");

    *output_filepath = (char *)malloc(strlen(DIR_COMPRESSED) + strlen(input_filepath) - SUFIX_COMPRESSED_SIZE + 1);
    const char *filename = strrchr(input_filepath, '/');
    if (filename == NULL)
    {
        filename = input_filepath;
    }
    else
    {
        filename++;
    }

    strcpy(*output_filepath, DIR_DECOMPRESSED);
    strncat(*output_filepath, filename, strlen(filename) - SUFIX_COMPRESSED_SIZE);

    logs_log_debug(USER, " - Output file path: %s", *output_filepath);

    return STATUS_OK;
}

// **********************************************************************
// Info source model

static STATUS _build_info_model(FILE *pFile, ism_t ***symbols_arr)
{
    logs_log_debug(USER, "Info source model build ... ");

    logs_log_trace(USER, "Allocating memory for the symbols array ... ");
    *symbols_arr = (ism_t **)malloc(ALPHABET_SIZE * sizeof(ism_t *));
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        (*symbols_arr)[i] = NULL;
    }

    if (build_info_model(pFile, symbols_arr) != 0)
    {
        return STATUS_ERROR_FILE_NOT_FOUND;
    }

    logs_log_debug(USER, "Info source model build : OK");
    return STATUS_OK;
}

static STATUS _sort_info_model(ism_t ***symbols_arr, int *symbols_count)
{
    logs_log_debug(USER, "Info source model sort ... ");

    int count = sort_model(symbols_arr);
    if (count == -1)
    {
        return STATUS_ERROR;
    }

    *symbols_count = count;
    logs_log_debug(USER, " * Symbols count: %u", *symbols_count);

    logs_log_debug(USER, "Info source model sort : OK");
    return STATUS_OK;
}

static STATUS _dump_info_model(ism_t ***symbols_arr, const char *filepath, char **output_filepath)
{
    logs_log_debug(USER, "Info source model dump ... ");

    if (_build_info_model_dump_filepath(filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_COMPRESS;
    }

    if (dump_model_to_file(symbols_arr, *output_filepath) != 0)
    {
        return STATUS_ERROR_FILE_CANNOT_CREATE;
    }

    logs_log_debug(USER, "Info source model dump : OK");
    return STATUS_OK;
}

// **********************************************************************
// Coding tree

static STATUS _build_coding_tree(int symbols_count, ism_t **symbols_arr, node_t **tree_root, unsigned int *nodes_count)
{
    logs_log_debug(USER, "Coding tree build ... ");

    *tree_root = build_coding_tree(symbols_count, symbols_arr, nodes_count);
    if (*tree_root == NULL)
    {
        return STATUS_ERROR;
    }
    logs_log_debug(USER, "Nodes count: %u", *nodes_count);

    logs_log_debug(USER, "Coding tree build : OK");
    return STATUS_OK;
}

static STATUS _load_tree(const char *filepath, node_t **tree_root, unsigned int *nodes_count, unsigned int *bytes_read)
{
    logs_log_debug(USER, "Coding tree load ... ");

    *tree_root = load_tree_from_file(filepath, nodes_count, bytes_read);
    if (*tree_root == NULL)
    {
        return STATUS_ERROR_FILE_NOT_FOUND;
    }
    logs_log_debug(USER, "Nodes count: %u", *nodes_count);

    logs_log_debug(USER, "Coding tree load : OK");
    return STATUS_OK;
}

static STATUS _dump_tree(node_t **tree_root, const char *filepath, char **output_filepath)
{
    logs_log_debug(USER, "Coding tree dump ... ");

    if (_build_tree_dump_filepath(filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_COMPRESS;
    }

    if (dump_tree_to_file(tree_root, *output_filepath) != 0)
    {
        return STATUS_ERROR_FILE_CANNOT_CREATE;
    }

    logs_log_debug(USER, "Coding tree dump : OK");
    return STATUS_OK;
}

// **********************************************************************
// Code table

static STATUS _build_code_table(node_t **tree_root, code_t ***code_table, unsigned int *bits_count, int *code_table_size)
{
    logs_log_debug(USER, "Code table build ... ");

    int size = build_code_table(tree_root, code_table, bits_count);
    if (size < 0)
    {
        logs_log_error(USER, "Tree root is NULL! tree_root: %p", *tree_root);
        return STATUS_ERROR;
    }
    else if (size == 0)
    {
        logs_log_error(USER, "Code table size: %d", size);
        return STATUS_ERROR_COMPRESS;
    }

    *code_table_size = size;
    logs_log_debug(USER, "Code table size: %d", *code_table_size);
    logs_log_debug(USER, "Bits count: %u", *bits_count);

    logs_log_debug(USER, "Code table build : OK");
    return STATUS_OK;
}

static STATUS _dump_code_table(code_t ***code_table, int code_table_size, const char *filepath, char **output_filepath)
{
    logs_log_debug(USER, "Code table dump ... ");

    if (_build_code_table_dump_filepath(filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_COMPRESS;
    }

    if (dump_code_table_to_file(code_table, code_table_size, *output_filepath) != 0)
    {
        return STATUS_ERROR_FILE_CANNOT_CREATE;
    }

    logs_log_debug(USER, "Code table dump : OK");
    return STATUS_OK;
}

// **********************************************************************
// Compression

static STATUS _compress_file(const char *input_filepath, char **output_filepath, node_t **tree_root, unsigned int nodes_count, code_t ***code_table, unsigned int bits_count, unsigned int *in_file_size, unsigned int *out_file_size)
{
    logs_log_debug(USER, "File [%s] compression ... ", input_filepath);

    logs_log_trace(USER, "Building the output file path ... ");
    if (_build_compress_output_filepath(input_filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_COMPRESS;
    }

    int status = compress_file(input_filepath, *output_filepath, tree_root, nodes_count, code_table, bits_count, in_file_size, out_file_size);
    switch (status)
    {
    case 0:
        logs_log_debug(USER, "File [%s] compression : OK", input_filepath);
        return STATUS_OK;
        break;
    case 1:
        return STATUS_ERROR_FILE_NOT_FOUND;
        break;
    case 2:
        return STATUS_ERROR_FILE_CANNOT_CREATE;
        break;
    case 3:
        return STATUS_ERROR_COMPRESS;
        break;
    default:
        return STATUS_ERROR;
        break;
    }
}

// **********************************************************************
// Decompression

static STATUS _decompress_file(const char *input_filepath, char **output_filepath, unsigned int bytes_read, node_t *tree_root)
{
    logs_log_debug(USER, "File [%s] decompression ... ", input_filepath);

    if (_build_decompress_output_filepath(input_filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_DECOMPRESS;
    }

    unsigned int bits_count = 0;
    if (decompress_file(input_filepath, *output_filepath, bytes_read, tree_root, &bits_count) != 0)
    {
        return STATUS_ERROR_DECOMPRESS;
    }
    logs_log_debug(USER, "Bits count: %u", bits_count);

    logs_log_debug(USER, "File [%s] decompression : OK", input_filepath);
    return STATUS_OK;
}

// **********************************************************************
// CRC32

static STATUS _calculate_crc(const char *filepath, int *crc)
{
    logs_log_debug(USER, "CRC32 calculation ... ");

    int crc_temp = crc32_file(filepath);
    if (crc_temp == -1)
    {
        return STATUS_ERROR_FILE_NOT_FOUND;
    }

    logs_log_trace(USER, "CRC32 endianess swap ... ");
    *crc = swap_endian(crc_temp);
    logs_log_debug(USER, "CRC32 value: 0x%08X", *crc);

    logs_log_debug(USER, "CRC32 calculation : OK");
    return STATUS_OK;
}

static STATUS _read_crc(const char *filepath, int *crc)
{
    logs_log_debug(USER, "CRC32 read ... ");

    *crc = crc32_file_read(filepath);
    if (*crc == -1)
    {
        return STATUS_ERROR_FILE_NOT_FOUND;
    }
    logs_log_trace(USER, "CRC32 endianess swap ... ");
    *crc = swap_endian((int)*crc);
    logs_log_debug(USER, "CRC32 value: 0x%08X", *crc);

    logs_log_debug(USER, "CRC32 read : OK");
    return STATUS_OK;
}

static STATUS _append_crc_to_file(const char *filepath, const int *crc, unsigned int *out_file_size)
{
    logs_log_debug(USER, "CRC32 append ... ");

    logs_log_trace(USER, "Opening the file `%s`... ", filepath);
    FILE *outFile = fopen(filepath, "ab");
    if (outFile == NULL)
    {
        return STATUS_ERROR_FILE_NOT_FOUND;
    }

    logs_log_trace(USER, "Writing CRC32 [0x%08X] to the file ... ", *crc);
    fwrite(crc, sizeof(int), 1, outFile);
    fclose(outFile);

    *out_file_size += 4;
    logs_log_trace(USER, "Output file size updated: %u", *out_file_size);

    logs_log_debug(USER, "CRC32 append : OK");

    return STATUS_OK;
}

static STATUS _compare_crc(int crc_1, int crc_2)
{
    logs_log_debug(USER, "CRC32 compare ... ");

    logs_log_trace(USER, "CRC32 values: [0x%08X / 0x%08X] ... ", crc_1, crc_2);

    if (crc_1 != crc_2)
    {
        return STATUS_ERROR_CRC;
    }

    logs_log_debug(USER, "CRC32 compare : OK");
    return STATUS_OK;
}

static STATUS _dump_crc(int crc, const char *input_filepath, char **output_filepath)
{
    logs_log_debug(USER, "CRC32 dump ... ");

    if (_build_crc_dump_filepath(input_filepath, output_filepath) != STATUS_OK)
    {
        return STATUS_ERROR_CRC;
    }

    logs_log_trace(USER, "Opening the file `%s`... ", *output_filepath);
    FILE *outFile = fopen(*output_filepath, "wb");
    if (outFile == NULL)
    {
        return STATUS_ERROR_FILE_CANNOT_CREATE;
    }

    logs_log_trace(USER, "Writing CRC32 [0x%08X] to the file ... ", crc);
    fprintf(outFile, "0x%08X", crc);
    fclose(outFile);

    logs_log_debug(USER, "CRC32 dump : OK");
    return STATUS_OK;
}

// **********************************************************************
// Summary

static void _summary_compress(const char *output_filepath, const unsigned int in_file_size, const unsigned int out_file_size)
{
    logs_log_info(USER, " -- SUMMARY COMPRESS -- ");
    logs_log_info(USER, "  - Output file saved as: %s", output_filepath);
    logs_log_info(USER, "  - Input file size: %u B", in_file_size);
    logs_log_info(USER, "  - Output file size: %u B", out_file_size);
    logs_log_info(USER, "  - Compression ratio: %.2f%%", (float)out_file_size / in_file_size * 100);
}

static void _summary_decompress(const char *output_filepath, STATUS crc_check_status)
{
    logs_log_info(USER, " -- SUMMARY DECOMPRESS -- ");
    logs_log_info(USER, "  - Output file saved as: %s", output_filepath);
    logs_log_info(USER, "  - CRC32 check: %s", crc_check_status == STATUS_OK ? "OK" : "ERROR");
}

// **********************************************************************
// Cleanup

static void _cleanup(FILE *inFile, char *output_filepath)
{
    if (inFile != NULL)
    {
        fclose(inFile);
    }
    if (output_filepath != NULL)
    {
        free(output_filepath);
    }
}

// **********************************************************************
// Procedures

STATUS procedure_compress(char *filepath)
{
    // Procedure steps:
    // 1.  Check if the file exists and ASCII coding
    // 2.  Build the info source model
    // 3.  Sort the model
    // 4.  Dump the model
    // 5.  Build the coding tree
    // 6.  Dump the tree
    // 7.  Build the code table
    // 8.  Dump the code table
    // 9.  Compress the file
    // 10. Calculate CRC32
    // 11. Dump CRC32
    // 12. Append CRC32 to the compressed file
    // 13. Summary

    // Return status
    STATUS status = STATUS_OK;

    // Variables
    FILE *inFile = NULL;              // Input file pointer
    ism_t **symbols_arr = NULL;       // Info source model
    int symbols_count = 0;            // Info source model size
    unsigned int nodes_count = 0;     // Coding tree nodes count
    node_t *tree_root = NULL;         // Coding tree root
    unsigned int bits_count = 0;      // Total bits count
    code_t **code_table = NULL;       // Code table
    int code_table_size = 0;          // Code table size
    char *output_filepath = NULL;     // Output file path
    int crc = 0;                      // CRC32
    unsigned int in_file_size = 0;    // Input file size
    unsigned int out_file_size = 0;   // Output file size
    char *model_filepath = NULL;      // Info source model file path
    char *tree_filepath = NULL;       // Coding tree file path
    char *code_table_filepath = NULL; // Code table file path
    char *crc_filepath = NULL;        // CRC32 file path

    // ----------------------------------------------------------------------
    // AD 1 - Check if the file exists and open it
    status = _check_ASCII(filepath, &inFile);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 2 - Build the info source model
    status = _build_info_model(inFile, &symbols_arr);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 3 - Sort the model
    status = _sort_info_model(&symbols_arr, &symbols_count);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 4 - Dump the model
    status = _dump_info_model(&symbols_arr, filepath, &model_filepath);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 5 - Build the coding tree
    status = _build_coding_tree(symbols_count, symbols_arr, &tree_root, &nodes_count);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 6 - Dump the tree
    status = _dump_tree(&tree_root, filepath, &tree_filepath);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 7 - Build the code table
    status = _build_code_table(&tree_root, &code_table, &bits_count, &code_table_size);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 8 - Dump the code table
    status = _dump_code_table(&code_table, code_table_size, filepath, &code_table_filepath);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 9 - Compress the file
    status = _compress_file(filepath, &output_filepath, &tree_root, nodes_count, &code_table, bits_count, &in_file_size, &out_file_size);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 10 - Calculate CRC32
    status = _calculate_crc(filepath, &crc);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 11 - Dump CRC32
    status = _dump_crc(crc, filepath, &crc_filepath);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 12 - Append CRC32 to the compressed file
    status = _append_crc_to_file(output_filepath, &crc, &out_file_size);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 13 - Summary
    _summary_compress(output_filepath, in_file_size, out_file_size);

    // ----------------------------------------------------------------------
    // Cleanup
    _cleanup(inFile, output_filepath);

    return STATUS_OK;
}

STATUS procedure_decompress(char *filepath)
{
    /// Procedure steps:
    // 1. Check if the file exists and open it
    // 2. Load the coding tree
    // 3. Read CRC32 from the file
    // 4. Decompress the file
    // 5. Calculate CRC32 of the decompressed file
    // 6. Compare CRC32 values
    // 7. Summary

    // Return status
    STATUS status = STATUS_OK;

    // Variables
    FILE *inFile = NULL;          // Input file pointer
    node_t *tree_root = NULL;     // Coding tree root
    unsigned int nodes_count = 0; // Coding tree nodes count
    unsigned int bytes_read = 0;  // Bytes read from the file
    int crc_read = 0;             // CRC32 read from the file
    int crc_decompressed = 0;     // CRC32 of the decompressed file
    char *output_filepath = NULL; // Output file path

    // ----------------------------------------------------------------------
    // AD 1 - Check if the file exists and open it
    status = _check_file(filepath, &inFile);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 2 - Load the coding tree
    status = _load_tree(filepath, &tree_root, &nodes_count, &bytes_read);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 3 - Read CRC32 from the file
    status = _read_crc(filepath, &crc_read);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 4 - Decompress the file
    status = _decompress_file(filepath, &output_filepath, bytes_read, tree_root);
    if (status != STATUS_OK)
    {
        return status;
    }

    // AD 5 - Calculate CRC32 of the decompressed file
    status = _calculate_crc(output_filepath, &crc_decompressed);
    if (status != STATUS_OK)
    {
        fclose(inFile);
        return status;
    }

    // AD 6 - Compare CRC32 values
    status = _compare_crc(crc_read, crc_decompressed);

    // AD 7 - Summary
    _summary_decompress(output_filepath, status);

    // ----------------------------------------------------------------------
    // Cleanup
    _cleanup(inFile, output_filepath);

    return status;
}

STATUS procedure_crc_calc(char *filepath)
{
    STATUS status = STATUS_OK;
    int crc = 0;
    char *output_filepath = NULL;

    logs_log_info(USER, "Calculating CRC32 of the file: %s", filepath);

    status = _calculate_crc(filepath, &crc);
    if (status != STATUS_OK)
    {
        return status;
    }
    logs_log_info(USER, "CRC32: 0x%08X", crc);

    status = _dump_crc(crc, filepath, &output_filepath);
    if (status != STATUS_OK)
    {
        return status;
    }

    return status;
}