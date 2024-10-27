#include <compress/code_table.h>

code_t **_code_table = NULL;

static int code_table_size = 0;
static unsigned int _bits_count = 0;

int dump_code_table_to_file(code_t ***code_table, int code_table_size, const char *filepath)
{
    logs_log_trace(USER, "Dumping the code table to the file: %s", filepath);

    FILE *file = fopen(filepath, "wb");
    if (file == NULL)
    {
        logs_log_error(USER, "Error opening the file: %s", filepath);
        return -1;
    }

    if (code_table_size == 0)
    {
        logs_log_error(USER, "Code table is empty!");
        return -2;
    }

    if (*code_table == NULL)
    {
        logs_log_error(USER, "Code table is NULL!");
        return -3;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if ((*code_table)[i] == NULL)
        {
            continue;
        }
        fprintf(file, "0x%02X;", (*code_table)[i]->symbol);
        for (int j = 0; j < (*code_table)[i]->word_size; j++)
        {
            fprintf(file, "%d", ((*code_table)[i]->sequence >> j) & 1);
        }
        fprintf(file, ";%d\n", (*code_table)[i]->word_size);
    }

    fclose(file);

    return 0;
}

static void _traverse_tree(node_t *node, int sequence, int word_size)
{
    int seq = 0;
    // left
    // shift and add 0
    if (node->left != NULL)
    {
        seq = sequence;
        seq = seq << 1;
        seq = seq | 0;
        _traverse_tree(node->left, seq, word_size + 1);
    }
    // right
    // shift and add 1
    if (node->right != NULL)
    {
        seq = sequence;
        seq = seq << 1;
        seq = seq | 1;
        _traverse_tree(node->right, seq, word_size + 1);
    }
    // leaf
    if (node->left == NULL && node->right == NULL)
    {
        _code_table[(unsigned char)node->symbol] = (code_t *)malloc(sizeof(code_t));
        _code_table[(unsigned char)node->symbol]->symbol = node->symbol;
        _code_table[(unsigned char)node->symbol]->sequence = sequence;
        _code_table[(unsigned char)node->symbol]->word_size = word_size;

        logs_log_trace(USER, " * New Entry : Symbol: [0x%02X], Sequence: [0x%02X], Word size: %d", node->symbol, sequence, word_size);

        code_table_size++;
        _bits_count += node->freq * word_size;
    }
}

int build_code_table(node_t **tree_root, code_t ***code_table, unsigned int *bits_count)
{
    if (*tree_root == NULL)
    {
        return -1;
    }

    _code_table = (code_t **)malloc(ALPHABET_SIZE * sizeof(code_t *));
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        _code_table[i] = NULL;
    }

    logs_log_trace(USER, "Starting to traverse the tree ... ");
    _traverse_tree(*tree_root, 0, 0);

    *bits_count = _bits_count;
    *code_table = _code_table;

    return code_table_size;
}