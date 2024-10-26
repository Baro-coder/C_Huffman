#include <coding_tree.h>

static unsigned int _nodes_count = 1;

void _dump_node(node_t *node, FILE *pFile)
{
    if (node == NULL || pFile == NULL)
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

    fprintf(pFile, "0x%02X;", s);
}

void _dump_tree_recursive(node_t *node, unsigned int depth, FILE *pFile)
{
    if (node == NULL)
        return;

    if (node->left != NULL && node->right != NULL)
    {
        _dump_node(node, pFile);
        _dump_node(node->left, pFile);
        _dump_node(node->right, pFile);
        fprintf(pFile, "\n");
    }

    _dump_tree_recursive(node->left, depth + 1, pFile);
    _dump_tree_recursive(node->right, depth + 1, pFile);
}

int dump_tree_to_file(node_t **root, const char *filepath)
{
    logs_log_trace(USER, "Dumping the tree into %s... ", filepath);

    FILE *pFile = fopen(filepath, "wb");
    if (pFile == NULL)
    {
        return -1;
    }

    if (root == NULL)
    {
        return -2;
    }

    _dump_tree_recursive(*root, 0, pFile);

    fclose(pFile);

    return 0;
}

node_t *_find_node(node_t *current, char symbol)
{
    if (current == NULL)
    {
        return NULL;
    }

    if (current->symbol == symbol)
    {
        return current;
    }

    if (current->left != NULL)
    {
        node_t *left = _find_node(current->left, symbol);
        if (left != NULL)
        {
            return left;
        }
    }

    if (current->right != NULL)
    {
        node_t *right = _find_node(current->right, symbol);
        if (right != NULL)
        {
            return right;
        }
    }

    return NULL;
}

/* ***************************************************************************** */

node_t *build_coding_tree(int symbols_count, ism_t **symbols_arr, unsigned int *nodes_count)
{
    // root init
    logs_log_trace(USER, " * Root init ...");
    node_t *root = (node_t *)malloc(sizeof(node_t));
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
    root->symbol = '\0';
    root->freq = 0;

    node_t *current = root;
    node_t *new_node;

    for (int i = symbols_count - 1; i >= 0; i--)
    {
        // new node init
        logs_log_trace(USER, " * New leaf : symbol: 0x%02X, count: %d", symbols_arr[i]->symbol, symbols_arr[i]->count);
        new_node = (node_t *)malloc(sizeof(node_t));
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->parent = NULL;
        new_node->symbol = symbols_arr[i]->symbol;
        new_node->freq = symbols_arr[i]->count;
        _nodes_count++;

        if (current->left == NULL && current->right == NULL)
        {
            current->right = new_node;
            new_node->parent = current;
            current->freq += new_node->freq;
        }
        else if (current->left == NULL && current->right != NULL)
        {
            if (current->right->freq < new_node->freq)
            {
                current->left = new_node;
                new_node->parent = current;
                current->freq += new_node->freq;
            }
            else
            {
                current->left = current->right;
                current->right = new_node;
                new_node->parent = current;
                current->freq += new_node->freq;
            }
        }
        else if (current->left != NULL && current->right == NULL)
        {
            if (current->left->freq < new_node->freq)
            {
                current->right = current->left;
                current->left = new_node;
                new_node->parent = current;
                current->freq += new_node->freq;
            }
            else
            {
                current->right = new_node;
                new_node->parent = current;
                current->freq += new_node->freq;
            }
        }
        else
        {
            current->parent = (node_t *)malloc(sizeof(node_t));
            root = current->parent;

            root->parent = NULL;
            root->symbol = '\0';
            root->freq = new_node->freq + current->freq;

            logs_log_trace(USER, "  + New proxy node : count: %d", root->freq);

            if (new_node->freq < current->freq)
            {
                root->right = new_node;
                root->left = current;
            }
            else
            {
                root->left = new_node;
                root->right = current;
            }

            current = root;
            _nodes_count++;
        }
    }

    *nodes_count = _nodes_count;

    return root;
}

/* ***************************************************************************** */

node_t *load_tree_from_file(const char *filepath, unsigned int *nodes_count, unsigned int *bytes_read)
{
    FILE *pFile = fopen(filepath, "rb");
    if (pFile == NULL)
    {
        return NULL;
    }

    fread(nodes_count, sizeof(unsigned int), 1, pFile);
    *nodes_count = swap_endian(*nodes_count);
    *bytes_read += 4;
    logs_log_trace(USER, " * Bytes read updated: %u", *bytes_read);

    node_t *root = NULL;
    node_t *current = NULL;
    unsigned int nodes_loaded = 0;
    char buffer[3];

    while (nodes_loaded < *nodes_count)
    {
        fread(&buffer[0], sizeof(char), 1, pFile); // parent
        fread(&buffer[1], sizeof(char), 1, pFile); // left
        fread(&buffer[2], sizeof(char), 1, pFile); // right
        logs_log_trace(USER, "  - Buffer read: [0x%02X, 0x%02X, 0x%02X]", buffer[0], buffer[1], buffer[2]);
        *bytes_read += 3;
        logs_log_trace(USER, " * Bytes read updated: %u", *bytes_read);

        if (root == NULL)
        {
            if (buffer[0] != 0)
            {
                root = (node_t *)malloc(sizeof(node_t));
                root->parent = NULL;
                root->symbol = buffer[0];
                root->freq = 0;
                nodes_loaded++;
            }

            if (buffer[1] != 0)
            {
                root->left = (node_t *)malloc(sizeof(node_t));
                root->left->parent = root;
                root->left->symbol = buffer[1];
                root->left->freq = 0;
                root->left->left = NULL;
                root->left->right = NULL;
                nodes_loaded++;
            }

            if (buffer[2] != 0)
            {
                root->right = (node_t *)malloc(sizeof(node_t));
                root->right->parent = root;
                root->right->symbol = buffer[2];
                root->right->freq = 0;
                root->right->left = NULL;
                root->right->right = NULL;
                nodes_loaded++;
            }
        }
        else
        {
            current = _find_node(root, buffer[0]);
            if (current == NULL)
            {
                return NULL;
            }

            if (buffer[1] != 0)
            {
                current->left = (node_t *)malloc(sizeof(node_t));
                current->left->parent = current;
                current->left->symbol = buffer[1];
                current->left->freq = 0;
                current->left->left = NULL;
                current->left->right = NULL;
                nodes_loaded++;
            }

            if (buffer[2] != 0)
            {
                current->right = (node_t *)malloc(sizeof(node_t));
                current->right->parent = current;
                current->right->symbol = buffer[2];
                current->right->freq = 0;
                current->right->left = NULL;
                current->right->right = NULL;
                nodes_loaded++;
            }
        }
    }

    fclose(pFile);

    return root;
}