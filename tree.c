/* TACHE Robert-Andrei - 312CD */

#include <math.h>
#include <stdio.h>

#include "tree.h"
#include "queue.h"

void treeWrite(Tree root, RGB color) {
    if (root == NULL) return;

    root->color[0] = color[0]; // R
    root->color[1] = color[1]; // G
    root->color[2] = color[2]; // B
}

Tree createTree(RGB color) {
	Tree root = malloc(sizeof(TreeNode));

    root->type = 1;
	treeWrite(root, color);

    for (int i = 0; i < 4; i++) {
        root->children[i] = NULL;
    }

	return root;
}

int checkTolerance(int tolerance, unsigned int dimensiune_imagine, unsigned char *imag, int size, int x, int y) {
    int i, j;
    unsigned long long red = 0, green = 0, blue = 0, square = size*size, mean = 0;

    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            red +=  imag[i*dimensiune_imagine*3 + j*3 + 0];
            green += imag[i*dimensiune_imagine*3 + j*3 + 1];
            blue += imag[i*dimensiune_imagine*3 + j*3 + 2];
        }
    }

    red = red / square;
    green = green / square;
    blue = blue / square;
    
    for (i = x; i < x + size; i++) {
        for (j = y; j < y + size; j++) {
            mean += (red - imag[i*dimensiune_imagine*3 + j*3 + 0])*(red - imag[i*dimensiune_imagine*3 + j*3 + 0]) +
                (green - imag[i*dimensiune_imagine*3 + j*3 + 1])*(green - imag[i*dimensiune_imagine*3 + j*3 + 1]) +
                (blue - imag[i*dimensiune_imagine*3 + j*3 + 2])*(blue - imag[i*dimensiune_imagine*3 + j*3 + 2]);
        }
    }

    mean = mean / (3 * square);

    return ((int)mean <= tolerance);
}

Tree processImage(int tolerance, unsigned int dimensiune_imagine, unsigned char *imag, int size, int x, int y) {
    unsigned char color[3];
    unsigned long long red = 0, green = 0, blue = 0, square = size * size;

    for (int i = x; i < x + size; i++) {
        for (int j = y; j < y + size; j++) {
            red += imag[i*dimensiune_imagine*3 + j*3 + 0];
            green += imag[i*dimensiune_imagine*3 + j*3 + 1];
            blue += imag[i*dimensiune_imagine*3 + j*3 + 2];
        }
    }

    red = red / square;
    green = green / square;
    blue = blue / square;

    color[0] = (unsigned char) red;
    color[1] = (unsigned char) green;
    color[2] = (unsigned char) blue;

    Tree root = createTree(color);

    if (checkTolerance(tolerance, dimensiune_imagine, imag, size, x, y)) {
        return root;
    } else {
        root->type = 0;
        root->children[0] = processImage(tolerance, dimensiune_imagine, imag, size/2, x, y);
        root->children[1] = processImage(tolerance, dimensiune_imagine, imag, size/2, x, y+size/2);
        root->children[2] = processImage(tolerance, dimensiune_imagine, imag, size/2, x+size/2, y+size/2);
        root->children[3] = processImage(tolerance, dimensiune_imagine, imag, size/2, x+size/2, y);
    }

    return root;
}

int height(Tree root) {
	if (root == NULL) return 0;
    int max = -1, childHeight;
    for (int i = 0; i < 4; i++) {
        childHeight = height(root->children[i]);
        if (max < childHeight) {
            max = childHeight;
        }
    }
	return ++max;
}

int blockCount(Tree root) {
    if (root->type == 1) return 1;
    int count = 0;
    for (int i = 0; i < 4; i++) {
        count += blockCount(root->children[i]);
    }
    return count;
}

// Returns smallest height
int minHeight(Tree root) {
    if (root->type == 1) return 1;
    int min = minHeight(root->children[0]), childHeight;
    for (int i = 1; i < 4; i++) {
        childHeight = minHeight(root->children[i]);
        if (min > childHeight) {
            min = childHeight;
        }
    }
    return min + 1;
}

void printLevel(Tree root, int level, FILE *out) {
    if (root == NULL) return;

    if (level == 0) {
        if (root->type == 0) {
            fwrite(&(root->type), sizeof(unsigned char), 1, out);
        } else if (root->type == 1) {
            fwrite(&(root->type), sizeof(unsigned char), 1, out);
            fwrite(&(root->color), sizeof(unsigned char), 3, out);
        }
    } else if (root->type == 0) {
        for (int i = 0; i < 4; i++) {
            printLevel(root->children[i], level - 1, out);
        }
    }
}

void writeCompressed(FILE *out, unsigned int dimensiune_imagine, Tree root) {
    fwrite(&dimensiune_imagine, sizeof(unsigned int), 1, out);
    int rootLevels = height(root);

    for (int i = 0; i <= rootLevels; i++) {
        printLevel(root, i, out);
    }
}

Tree parseVector(Tree *pixelVector, int count) {
    Queue *zeroQueue = initQueue();

    int i = 0, kid = 0;
    while (i < count) { // Make a queue of intern nodes that "need filling"
        if (!isEmptyQueue(zeroQueue)) {
            zeroQueue->front->value->children[kid] = pixelVector[i];
            ++kid;
            if (kid == 4) { // All children full => next node please!
                kid = 0;
                dequeue(zeroQueue);
            }
        }

        if (pixelVector[i]->type == 0) {
            enqueue(zeroQueue, pixelVector[i]);
        }
        ++i;
    }
    
    free(zeroQueue);

    return pixelVector[0];
}

void remakeImage(Tree root, unsigned char *imag, unsigned int img_dim, int size, int x, int y) {
    if (root->type == 0) { // basically do that wacky stuff for making a root, but reversed
        remakeImage(root->children[0], imag, img_dim, size/2, x, y);
        remakeImage(root->children[1], imag, img_dim, size/2, x, y+size/2);
        remakeImage(root->children[2], imag, img_dim, size/2, x+size/2, y+size/2);
        remakeImage(root->children[3], imag, img_dim, size/2, x+size/2, y);
    } else {
        for (int i = x; i < x + size; i++) {
            for (int j = y; j < y + size; j++) {
                imag[img_dim*i*3 + j*3 + 0] = root->color[0];
                imag[img_dim*i*3 + j*3 + 1] = root->color[1];
                imag[img_dim*i*3 + j*3 + 2] = root->color[2];
            }
        }
    }
}

void writeImage(FILE *out, char *format, unsigned int dimensiune_imagine, int intensity, unsigned char *imag) {
    int i, j;
    fprintf(out, "%s\n%d %d\n%d\n", format, dimensiune_imagine, dimensiune_imagine, intensity);

    for (i = 0; i < dimensiune_imagine; i++) {
        for (j = 0; j < dimensiune_imagine; j++) {
            fwrite(&imag[i*dimensiune_imagine*3 + j*3], sizeof(unsigned char), 3, out);
        }
    }
}

void decompress(FILE *in, Tree root, FILE *out) {
    char *format = "P6";
    unsigned int intensity = 255;
    unsigned int img_dim;
    fread(&img_dim, sizeof(unsigned int), 1, in);

    unsigned char type, color[3]; // dummy color
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;

    Tree *pixelVector = malloc(sizeof(Tree) * img_dim * img_dim * 2); // at most this many... right?
    int count = 0;

    while (!feof(in)) { // go through and make up a vector to parse after
        Tree freshRoot = createTree(color);
        fread(&type, sizeof(unsigned char), 1, in);
        if (type == 1) {
            fread(&color, sizeof(unsigned char), 3, in);
            treeWrite(freshRoot, color);
        } else {
            freshRoot->type = 0;
        }
        pixelVector[count] = freshRoot;
        count++;
    }

    root = parseVector(pixelVector, count);

    unsigned char *imag = malloc(img_dim * img_dim * 3 * sizeof(unsigned char)); // image
    remakeImage(root, imag, img_dim, img_dim, 0, 0);
    writeImage(out, format, img_dim, intensity, imag);

    for (int i = 0; i < count; i++) free(pixelVector[i]);
    free(imag);
    free(pixelVector);
}

void destroyTree(Tree *root) {
	if (*root == NULL) {
        return;
    } else {
        for (int i = 0; i < 4; i++) {
		    destroyTree(&(*root)->children[i]);
        }

		Tree aux = *root;
		*root = NULL;
		free(aux);
	}
}