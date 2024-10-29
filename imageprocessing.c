#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#define RGB 3
#define MAX_VAL 255
#define MIN_VAL 0
int ***flip_horizontal(int ***image, int N, int M) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M/2 ; j++)
            for (int k = 0; k < RGB; k++) {     // every element in the left side of the matrix
                                                // is interchanged with the one positioned simetrical
                int swap = image[i][j][k];      // to the right side of the matrix
                image[i][j][k] = image[i][M - j - 1][k];
                image[i][M-j-1][k] = swap;
            }
return image;
}
int ***rotate_left(int ***image, int N, int M) {
    int ***matrice_90 = malloc(M * sizeof(int **));
    for (int i = 0; i < M; i++) {
        matrice_90[i] = malloc(N * sizeof(int *));
        for (int j = 0; j < N; j++) {
            matrice_90[i][j] = malloc(RGB * sizeof(int));   // by rotating a matrix 90 degrees to the left
            for (int k = 0; k < RGB; k++)                   // the matrix becomes its transpose, inverted
            matrice_90[i][j][k] = image[j][M-i-1][k];       // by the middle line
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
return matrice_90;
}
int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {  // i, j represent line, column
int ***m = (int ***)malloc(h * sizeof(int **));
    for (int i = 0; i < h; i++) {
        m[i] = (int **)malloc(w * sizeof(int *));
        for (int j = 0; j < w; j++) {
            m[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }

    for (int i = 0, y_crop = y; i < h; i++, y_crop++)         // new matrix starts 0, 0
        for (int j = 0, x_crop = x; j < w; j++, x_crop++)     // x_crop,y_crop lin, cols from initial
            for (int k = 0; k < RGB; k++)                     // matrix that gets cropped
                m[i][j][k] = image[y_crop][x_crop][k];  // every line copied at the beginning
                                                        // image has for dims h, w the crop from original image
for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return m;
}
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
    int N_ext = N + 2 * rows;
    int M_ext = M + 2 * cols;
    int ***extended = (int ***)malloc(N_ext * sizeof(int **));
    for (int i = 0; i < N_ext; i++) {
        extended[i] = (int **)malloc(M_ext * sizeof(int *));
        for (int j = 0; j < M_ext; j++) {
            extended[i][j] = (int *)malloc(RGB * sizeof(int));
        }
    }
    for (int i = 0; i < N_ext; i++)
        for (int j = 0; j < M_ext; j++) {
            for (int k = 0; k < RGB; k++) {  // if elem is in the original matrix, copy in extended
                if ( (cols <= j && j < M_ext - cols) && (rows <= i && i < N_ext - rows) ) {
                    extended[i][j][k] = image[i-rows][j-cols][k];
                } else {
                    extended[i][j][0] = new_R;
                    extended[i][j][1] = new_G;
                    extended[i][j][2] = new_B;
                }
            }
        }
for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
return extended;
}
// starting from the requested position from dest image
// replace every element from dest image
// while not crossing edges of both images
int ***paste(int ***img_dst, int N_dst, int M_dst, int *** img_src, int N_src, int M_src, int x, int y) {
    for (int i = y, l_src = 0 ; i < N_dst && l_src < N_src; i++, l_src++)
        for (int j = x, c_src = 0; j < M_dst && c_src < M_src; j++, c_src++)
            for (int k = 0; k < RGB; k++) {
                img_dst[i][j][k] = img_src[l_src][c_src][k];
            }
return img_dst;
}
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
    int ***m = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        m[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            m[i][j] = (int *)malloc(RGB * sizeof(int));
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            for (int k = 0; k < RGB; k++) {                      // for every pixel
                float suma = 0;
                for (int fl = 0; fl < filter_size; fl++)
                    for (int fc = 0; fc < filter_size; fc++) {  // sum of pixels around it
                        float termen_suma = 0;
                        int l_el = i + fl - filter_size/2;
                        int c_el = j + fc - filter_size/2;
                        if (l_el >= 0  && l_el < N && c_el >= 0 && c_el < M)
                        termen_suma = filter[fl][fc] * (float)image[l_el][c_el][k];
                        suma += termen_suma;
                    }
            m[i][j][k] = (int) suma;        // float to int
            if (m[i][j][k] > MAX_VAL) m[i][j][k] = MAX_VAL;    // restrict value to  0-255
            else if (m[i][j][k] < MIN_VAL) m[i][j][k] = MIN_VAL;
            }
    for (int i = 0; i < N; i++) {             // free old image
        for (int j = 0; j < M; j++) {
            free(image[i][j]);
        }
        free(image[i]);
    }
    free(image);
    return m;
}
