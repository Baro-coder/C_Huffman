#include <compress/compress.h>

static FILE *inFile;
static FILE *outFile;

static unsigned int _in_file_size = 0;
static unsigned int _out_file_size = 0;

void _write_buffer(unsigned char *buffer, unsigned short *buffer_size)
{
    if (*buffer_size == 0 || buffer == NULL || outFile == NULL || *buffer_size > 8)
    {
        return;
    }

    logs_log_trace(USER, " * BUFFER WRITE: [0x%02X]", *buffer);
    fwrite(buffer, sizeof(char), 1, outFile);
    *buffer = 0x00;
    *buffer_size = 0;
    _out_file_size++;
}

void _write_node(node_t *node)
{
    if (node == NULL)
    {
        return;
    }

    char s = '\0';
    // proxy node
    if (node->symbol == '\0')
    {
        s = (char)node->freq;
    }
    // leaf node
    else
    {
        s = node->symbol;
    }
    fwrite(&s, sizeof(char), 1, outFile);
    _out_file_size++;
}

void _write_tree_recursive(node_t *node, unsigned int depth)
{
    if (node == NULL)
        return;

    if (node->left != NULL && node->right != NULL)
    {
        _write_node(node);
        _write_node(node->left);
        _write_node(node->right);
    }

    _write_tree_recursive(node->left, depth + 1);
    _write_tree_recursive(node->right, depth + 1);
}

int compress_file(const char *input_filepath, const char *output_filepath, node_t **tree_root, unsigned int nodes_count, code_t ***code_table, unsigned int bits_count, unsigned int *in_file_size, unsigned int *out_file_size)
{
    // Open the input file
    logs_log_trace(USER, "Opening input file ...");
    inFile = fopen(input_filepath, "rb");
    if (inFile == NULL)
    {
        return 1;
    }

    // Create the output file
    logs_log_trace(USER, "Opening output file ...");
    outFile = fopen(output_filepath, "wb");
    if (outFile == NULL)
    {
        fclose(inFile);
        return 2;
    }

    // Check params
    logs_log_trace(USER, "Checking coding tree and code table ...");
    if (*tree_root == NULL || nodes_count == 0 || code_table == NULL || bits_count == 0)
    {
        return 3;
    }

    logs_log_debug(USER, "Writing header ...");
    // Write the header
    /*
     * The header is composed by:
     * - 4 bytes: the number of nodes of the coding tree
     * - n bytes: dumped coding tree (...[parent, left, right], ...)
     * - 4 bytes: the number of bits of the compressed data
     */
    // -- Write the number of nodes (convert the number of nodes to little-endian)
    logs_log_trace(USER, " - Writing the number of nodes (%u)...", nodes_count);
    unsigned int nodes_count_le = swap_endian(nodes_count);
    fwrite(&nodes_count_le, sizeof(int), 1, outFile);
    _out_file_size += 4;

    // -- Write the coding tree
    logs_log_trace(USER, " - Writing the coding tree ...");
    _write_tree_recursive(*tree_root, 0);

    // -- Write the number of bits (convert the number of bits to little-endian)
    logs_log_trace(USER, " - Writing the number of bits (%u)...", bits_count);
    unsigned int bits_count_le = swap_endian(bits_count);
    fwrite(&bits_count_le, sizeof(unsigned int), 1, outFile);
    _out_file_size += 4;
    logs_log_debug(USER, "Header size: %u", _out_file_size);

    // Write the compressed data
    logs_log_debug(USER, "Writing compressed data ...");
    char ch;
    unsigned char buffer = 0x00;
    unsigned short buffer_size = 0;

    while ((ch = fgetc(inFile)) != EOF)
    {
        _in_file_size++;
        unsigned char index = (unsigned char)ch;
        unsigned int sequence = (*code_table)[index]->sequence;
        unsigned int word_size = (*code_table)[index]->word_size;

        logs_log_trace(USER, "  - [0x%02X] -> [0x%08X] (%u bits)", index, sequence, word_size);

        for (int i = word_size - 1; i >= 0; i--)
        {
            unsigned char bit = (sequence >> i) & 1;
            buffer = buffer | (bit << (7 - buffer_size));
            buffer_size++;

            if (buffer_size == 8)
            {
                _write_buffer(&buffer, &buffer_size);
            }
        }
    }
    _write_buffer(&buffer, &buffer_size);

    logs_log_debug(USER, "Compressed data written: OK");

    // Close the files
    logs_log_trace(USER, "Closing files ...");
    fclose(inFile);
    fclose(outFile);

    logs_log_debug(USER, "Input file size: %u", _in_file_size);
    logs_log_debug(USER, "Output file size: %u", _out_file_size);
    *in_file_size = _in_file_size;
    *out_file_size = _out_file_size;

    return 0;
}
