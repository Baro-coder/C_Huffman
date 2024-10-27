#ifndef BS_STRUCTURES_H
#define BS_STRUCTURES_H

// Info source model
typedef struct _ism
{
    char symbol;        // Symbol
    unsigned int count; // Symbol count
} ism_t;

// Struct - Code Table Entry
typedef struct _code_t
{
    char symbol;            // Symbol
    unsigned int sequence;  // Binary Sequence
    unsigned int word_size; // Binary Sequence Size
} code_t;

// Struct - Node of the coding tree
typedef struct _node
{
    struct _node *left;   // left child
    struct _node *right;  // right child
    struct _node *parent; // parent
    char symbol;          // symbol
    unsigned int freq;    // symbol frequency
} node_t;

#endif