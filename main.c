/* TACHE Robert-Andrei - 312CD */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

void badUsage() {
    printf("Correct usage:\n./quadtree [-c1 factor | -c2 factor | -d] [input_file] [output_file]\n");
    exit(1);
}

int main(int argc, char const *argv[]) {

// Preliminary checks

    if (argc < 4) badUsage();

    int task = 0;
    if (argv[1][1] == 'c') {
        if (argv[1][2] == '1') task = 1;
        else if (argv[1][2] == '2') task = 2;
    } else if (argv[1][1] == 'd') task = 3;

    if (!task || (task != 3 && argc == 4)) {
        badUsage();
    }

    FILE* img = NULL;
    int tolerance = 0;

    if (task != 3) {
        tolerance = atoi(argv[2]);
        img = fopen(argv[3], "rb");
    } else {
        img = fopen(argv[2], "rb");
    }

    if (img == NULL) {
        printf("No image?\n");
        exit(1);
    }

// Image processing

    FILE* out;
    if (task != 3) out = fopen(argv[4], "wb");
    else out = fopen(argv[3], "wb");

    switch (task) {
        case 1: {
            // Header information
            char format[3];
            unsigned int dimensiune_imagine, intensity;
            fread(&format, sizeof(format), 1, img);
            fscanf(img, "%d%d\n", &dimensiune_imagine, &dimensiune_imagine);
            fscanf(img, "%d", &intensity);
            fscanf(img, "%c", &format[0]);

            // Image storage
            unsigned char *imag = malloc(dimensiune_imagine * dimensiune_imagine * 3 * sizeof(unsigned char));

            int i, j;
            for (i = 0; i < dimensiune_imagine; i++) {
                for (j = 0; j < dimensiune_imagine; j++) {
                    fread(&imag[i * dimensiune_imagine * 3 + j * 3 + 0], sizeof(unsigned char), 3, img);
                }
            }

            Tree root = processImage(tolerance, dimensiune_imagine, imag, dimensiune_imagine, 0, 0);
            
            int largestDim, rootHeight = height(root), rootCount = blockCount(root), rootMin = minHeight(root);

            if (rootMin == 0) {
                largestDim = dimensiune_imagine;
            } else {
                largestDim = dimensiune_imagine >> (rootMin - 1);
            }
            
            fprintf(out, "%d\n%d\n%d\n", rootHeight, rootCount, largestDim);
            
            free(imag);
            destroyTree(&root);
            break;
        }
        case 2: {
            // Header information
            char format[3];
            unsigned int dimensiune_imagine, intensity;
            fread(&format, sizeof(format), 1, img);
            fscanf(img, "%d%d\n", &dimensiune_imagine, &dimensiune_imagine);
            fscanf(img, "%d", &intensity);
            fscanf(img, "%c", &format[1]);

            // Image storage
            unsigned char *imag = malloc(dimensiune_imagine * dimensiune_imagine * 3 * sizeof(unsigned char));

            int i, j;
            for (i = 0; i < dimensiune_imagine; i++) {
                for (j = 0; j < dimensiune_imagine; j++) {
                    fread(&imag[i * dimensiune_imagine * 3 + j * 3 + 0], sizeof(unsigned char), 3, img);
                }
            }

            Tree root = processImage(tolerance, dimensiune_imagine, imag, dimensiune_imagine, 0, 0);
            
            writeCompressed(out, dimensiune_imagine, root);

            free(imag);
            destroyTree(&root);
            break;
        }
        case 3: {
            Tree root = NULL;
            decompress(img, root, out);
            destroyTree(&root);
            break;
        }
        default:
            printf("How did we get here?\n");
            break;
    }

    fclose(out);
    fclose(img);

    return 0;
}