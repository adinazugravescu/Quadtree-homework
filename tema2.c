#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tema2.h"
int main (int argc, const char *argv[]) {
    FILE* in;
    char type[3];
    unsigned int width, height, maxColor;
    if (argc == 5) {  
        char* f = (char*)argv[2];
        int factor = atoi(f);
        in = fopen(argv[3], "r");
        fscanf(in, "%s %u %u %u", type, &width, &height, &maxColor);
        fseek(in, 1, SEEK_CUR);
        Pixel** mat;
        mat = malloc(height*sizeof(Pixel*));
        for (int i = 0; i < height ; i++)
          mat[i] = malloc(width*sizeof(Pixel));
        for (int i = 0 ; i < width ; i++) { //reading the pixels in a Pixel matrix
            for (int j = 0 ; j < height ; j++) {
                fread(&mat[i][j].red, sizeof(unsigned char), 1, in);
                fread(&mat[i][j].green, sizeof(unsigned char), 1, in);
                fread(&mat[i][j].blue, sizeof(unsigned char), 1, in);
            }
        }
        Tree tree = createTree(width, 0, 0, mat, factor);
        if (strcmp(argv[1], "-c1") == 0) {
            FILE* out = fopen(argv[4], "wt");
            int level = heightTree(tree);
            level++; //number of levels is 1+height
            int colored = colorNodes(tree); ///number of leaves
            int aux = 1;
            int minH = minHeight(tree); // the height at which the first leaf is located, therefore also the node
            for (int i = 0 ; i < minH ; i++) //that retains the largest area in the image
                aux *= 2;
            int greatestZone = height/aux; //"side" of pixels will be divided by 2^hminim
            fprintf(out, "%d\n%d\n%d\n", level, colored, greatestZone);
            freeTree(tree);//free tree
            for (int i = 0 ; i < height ; i++) //release memory allocated for matrix
                free(mat[i]);
            free(mat);
            fclose(in);
            fclose(out);
            return 0;
        }
        else {
            FILE* out = fopen(argv[4], "wb");
            fwrite(&height,sizeof(unsigned int),1,out);
            printLevel(tree, out, height);
            fclose(in);
            fclose(out);
            freeTree(tree);//free tree
            for (int i = 0 ; i < height ; i++) //release memory allocated for matrix
            free(mat);
            return 0;
        }
    }
    return 0;
}
    
