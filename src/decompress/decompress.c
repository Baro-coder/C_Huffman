#include <decompress.h>

int decompress_file(const char *input_filepath, const char *output_filepath, unsigned int file_offset, node_t *tree_root, unsigned int *bits_count)
{
    // Open the input file
    logs_log_trace(USER, "Opening input file ...");
    FILE *inFile = fopen(input_filepath, "rb");
    if (inFile == NULL)
    {
        return 1;
    }

    // Create the output file
    logs_log_trace(USER, "Opening output file ...");
    FILE *outFile = fopen(output_filepath, "wb");
    if (outFile == NULL)
    {
        fclose(inFile);
        return 2;
    }

    // Check params
    logs_log_trace(USER, "Checking coding tree ...");
    if (tree_root == NULL || bits_count == NULL)
    {
        return 3;
    }

    logs_log_trace(USER, "Seeking to the beginning of the file + %d bytes...", file_offset);
    fseek(inFile, file_offset, SEEK_SET);

    fread(bits_count, sizeof(unsigned int), 1, inFile);
    *bits_count = swap_endian(*bits_count);
    logs_log_trace(USER, "Reading the number of bits (%u)...", *bits_count);

    unsigned int bits_read = 0;

    logs_log_debug(USER, "Decoding ...");
    char buffer = 0x00;
    node_t *current = tree_root;
    while (bits_read < *bits_count)
    {
        fread(&buffer, sizeof(char), 1, inFile);

        for (int i = 7; i >= 0; i--)
        {
            if (bits_read >= *bits_count)
            {
                break;
            }

            if ((buffer >> i) & 1)
            {
                current = current->right;
            }
            else
            {
                current = current->left;
            }

            if (current->left == NULL && current->right == NULL)
            {
                logs_log_trace(USER, "Writing symbol %c...", current->symbol);
                fwrite(&(current->symbol), sizeof(char), 1, outFile);
                current = tree_root;
            }

            bits_read++;
        }
    }

    fclose(inFile);
    fclose(outFile);

    if (bits_read != *bits_count)
    {
        return 3;
    }

    logs_log_debug(USER, "Decoding : OK");
    return 0;
}