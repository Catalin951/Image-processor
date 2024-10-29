#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"
#define MAX_IMG 100
#define RGB 3
#define MAX_PATH_LENGTH 100
void free_imagine(int ***imagine, int N, int M) {
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            free(imagine[i][j]);
        }
        free(imagine[i]);
    }
    free(imagine);
}
int ***alocare_imagine(int N, int M) {
    int ***imagine_noua = (int ***)malloc(N * sizeof(int **));
    for (int i = 0; i < N; i++) {
        imagine_noua[i] = (int **)malloc(M * sizeof(int *));
        for (int j = 0; j < M; j++) {
            imagine_noua[i][j] = (int *)malloc(3 * sizeof(int));
        }
    }
    return imagine_noua;
}
void copy_from_to_img(int ***imagine_copiata, int ***imagine_paste, int N, int M) {
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++)
        for (int k = 0; k < RGB; k++)
            imagine_paste[i][j][k] = imagine_copiata[i][j][k];   // copy every element
}
void free_filtru(float ** filtru, int size) {
        for (int i = 0; i < size; i++) {
            free(filtru[i]);
        }
free(filtru);
}
float **alocare_filtru(int size) {
float **filtru_nou = malloc(size * sizeof(float *));
for (int i = 0; i < size; i++)
    filtru_nou[i] = malloc(size * sizeof(float));
return filtru_nou;
}
void copy_from_to_filter(float ** filtru_copiat, float ** filtru_paste, int size) {
for(int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
        filtru_paste[i][j] = filtru_copiat[i][j];
}
int main() {
    int ****imagine = NULL;
    int *nr_linii_img = NULL;
    int *nr_coloane_img = NULL;
    float ***filtru = NULL;
    int *size_filtru = NULL;
    int nr_filtre = 0;
    int nr_img = 0;
    while (1) {
        char comanda[3];
        scanf("%s", comanda);
        if (strcmp(comanda, "e") == 0) break;
        if (strcmp(comanda, "l") == 0) {
            int N = 0, M = 0;
            char path[MAX_PATH_LENGTH];
            scanf("%d %d %s", &N, &M, path);
            imagine = realloc(imagine, (nr_img + 1) * sizeof(int ***));
            imagine[nr_img] = (int ***)malloc(N * sizeof(int **));       // pointer to images
            for (int i = 0; i < N; i++) {
                imagine[nr_img][i] = (int **)malloc(M * sizeof(int *));
                for (int j = 0; j < M; j++) {
                    imagine[nr_img][i][j] = (int *)malloc(RGB * sizeof(int));    // allocate image
                }
            }
            nr_linii_img = realloc(nr_linii_img, (nr_img + 1) * sizeof(int));  // realloc lines and cols
            nr_linii_img[nr_img] = N;
            nr_coloane_img = realloc(nr_coloane_img, (nr_img + 1) * sizeof(int));
            nr_coloane_img[nr_img] = M;
            read_from_bmp(imagine[nr_img], N, M, path);  // place image
            nr_img++;
        }
        if (strcmp(comanda, "s") == 0) {  // save image to path
            int index = 0;
            char path[MAX_PATH_LENGTH];
            scanf("%d %s", &index, path);
            write_to_bmp(imagine[index], nr_linii_img[index], nr_coloane_img[index], path);
            }
        if (strcmp(comanda, "ah") == 0) {  // rotate image horizontally
            int index = 0;
            scanf("%d", &index);
            flip_horizontal(imagine[index], nr_linii_img[index], nr_coloane_img[index]);
        }
        if (strcmp(comanda, "ar") == 0) {  // rotate 90 left
            int index = 0;
            scanf("%d", &index);
            imagine[index] = rotate_left(imagine[index], nr_linii_img[index], nr_coloane_img[index]);
            int swap = nr_linii_img[index];
            nr_linii_img[index] = nr_coloane_img[index];  // schimbam dimensiunile imaginii
            nr_coloane_img[index] = swap;
        }
        if (strcmp(comanda, "ac") == 0) {
            int index = 0, x = 0, y = 0, w = 0, h = 0;
            scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);
            imagine[index] = crop(imagine[index], nr_linii_img[index], nr_coloane_img[index], x, y, h, w);
            nr_linii_img[index] = h;  // change image dimensions
            nr_coloane_img[index] = w;
        }
        if (strcmp(comanda, "ae") == 0) {
            int index = 0, rows = 0, cols = 0, R = 0, G = 0, B = 0;
            scanf("%d %d %d %d %d %d", &index, &rows, &cols, &R, &G, &B);
            imagine[index] = extend(imagine[index], nr_linii_img[index], nr_coloane_img[index], rows, cols, R, G, B);
            nr_linii_img[index] += rows * 2;  // change image dimensions
            nr_coloane_img[index] += cols * 2;
        }
        if (strcmp(comanda, "ap") == 0) {
            int id_dst = 0, id_src = 0, x = 0, y = 0;
            scanf("%d %d %d %d", &id_dst, &id_src, &x, &y);
            int nr_lin_dst = nr_linii_img[id_dst];
            int nr_col_dst = nr_coloane_img[id_dst];
            int nr_lin_src = nr_linii_img[id_src];
            int nr_col_src = nr_coloane_img[id_src];
            paste(imagine[id_dst], nr_lin_dst, nr_col_dst, imagine[id_src], nr_lin_src, nr_col_src, x, y);
        }
        if (strcmp(comanda, "cf") == 0) {
            int size = 0;
            scanf("%d", &size);
            size_filtru = realloc(size_filtru, (nr_filtre + 1) * sizeof(int));
            size_filtru[nr_filtre] = size;
            filtru = realloc(filtru, (nr_filtre + 1) * sizeof(float **));
            filtru[nr_filtre] = malloc(size * sizeof(float *));
            for (int i = 0; i < size; i++) {
             filtru[nr_filtre][i] = malloc(size * sizeof(float));
                for (int j = 0; j < size; j++)
                scanf("%e", & filtru[nr_filtre][i][j]);
            }
            nr_filtre++;
        }
        if (strcmp(comanda, "af") == 0) {
            int index_img = 0, index_filter = 0;
            scanf("%d %d", &index_img, &index_filter);
            int nr_lin = nr_linii_img[index_img];
            int nr_col = nr_coloane_img[index_img];
            int size_f = size_filtru[index_filter];
            imagine[index_img] = apply_filter(imagine[index_img], nr_lin, nr_col, filtru[index_filter], size_f);
        }
        if (strcmp(comanda, "df") == 0) {
            int index_filter = 0;
            scanf("%d", &index_filter);
            for (int i = index_filter; i < nr_filtre - 1; i++) {  // move to the left every filter after the delted one
                free_filtru(filtru[i], size_filtru[i]);  // free
            size_filtru[i] = size_filtru[i + 1];         // modify size
            filtru[i] = alocare_filtru(size_filtru[i]);  // alocate filter with new dim
            copy_from_to_filter(filtru[i + 1], filtru[i], size_filtru[i]);  // next filter copied in new filter
            }
            free_filtru(filtru[nr_filtre - 1], size_filtru[nr_filtre - 1]);  // free last filter
            nr_filtre--;
        }
        if (strcmp(comanda, "di") == 0) {
            int index_img = 0;
            scanf("%d", &index_img);
            for (int i = index_img; i < nr_img - 1; i++) {   // move left all images after the deleted one
                free_imagine(imagine[i], nr_linii_img[i], nr_coloane_img[i]);
                nr_linii_img[i] = nr_linii_img[i + 1];           // modify dimensions
                nr_coloane_img[i] = nr_coloane_img[i + 1];
                imagine[i] = alocare_imagine(nr_linii_img[i], nr_coloane_img[i]);  // dimensions of next img
                copy_from_to_img(imagine[i + 1], imagine[i], nr_linii_img[i], nr_coloane_img[i]);
            }  // free last image
            free_imagine(imagine[nr_img - 1], nr_linii_img[nr_img - 1], nr_coloane_img[nr_img -1]);
            nr_img--;
        }
    }
    for (int i = 0; i < nr_img; i++)
    free_imagine(imagine[i], nr_linii_img[i], nr_coloane_img[i]);
    free(imagine);
    free(nr_coloane_img);
    free(nr_linii_img);
    for (int i = 0; i < nr_filtre; i++)
    free_filtru(filtru[i], size_filtru[i]);
    free(size_filtru);
    free(filtru);
    return 0;
}
