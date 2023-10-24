/* TACHE Robert-Andrei - 312CD */

#pragma once

#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    unsigned char type; // 0 - Intern ; 1 - Frunza
    unsigned char color[3]; // R - color[0] ; G - color[1] ; B - color[2]
    struct node *children[4];
} TreeNode, *Tree;

typedef unsigned char* RGB;

void treeWrite(Tree root, RGB color);
Tree createTree(RGB color);
int checkTolerance(int tolerance, unsigned int dimensiune_imagine, unsigned char *imag, int size, int x, int y);
Tree processImage(int tolerance, unsigned int dimensiune_imagine, unsigned char *imag, int size, int x, int y);
int height(Tree root);
int blockCount(Tree root);
int minHeight(Tree root);
void writeCompressed(FILE *out, unsigned int dimensiune_imagine, Tree root);
void decompress(FILE *in, Tree root, FILE *out);
void destroyTree(Tree *root);
