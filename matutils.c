#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include "spkmeans.h"
#include "matutils.h"
#include "strutils.h"


Matrix build_matrix(size_t n) {
    size_t i;
    Matrix mat = (Matrix) malloc(n * sizeof(Vector));

    for (i = 0; i < n; i++) {
        mat[i] = (Vector) calloc(n, sizeof(double)); 
    }
    return mat;
}


Matrix build_identity_mat(size_t n) {
    size_t i;
    Matrix mat = build_matrix(n);

    for (i=0; i < n; i++) {
        mat[i][i] = 1;
    }
    return mat;
}    


void free_matrix(Matrix mat, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        free(mat[i]);
    }
    free(mat);
}


void print_matrix(Matrix mat, size_t n, size_t m) {
    size_t i;
    for (i = 0; i < n; i++) {
        print_vector(mat[i], m);
        printf("\n");
    }
}


void print_vector(Vector vector, size_t m) {
    size_t i;
    for (i = 0; i < m; i++) {
        printf("%.4f", vector[i]);
        if (i < m - 1) {
            printf(",");
        }
    }
}


Matrix get_mat_from_file(char *filename, size_t *n, size_t *m) {
    size_t i, line_len;
    char *line = NULL;
    char *line_idx;
    Vector vector;
    Matrix mat;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        FATAL_ERROR();
    }

    mat = (Matrix) malloc(sizeof(Vector));

    while (getline(&line, &line_len, file) != -1) {
        if (*m == 0) {
           *m = strcount(line, COMMA) + 1; 
        }

        vector = (Vector) calloc(*m, sizeof(double));;
        line_idx = line;

        for (i = 0; i < (*m); i++) {
            vector[i] = strtod(line_idx, &line_idx);
            if (*line_idx != COMMA && *line_idx != NEWLINE) {
                FATAL_ERROR();
            }
            line_idx++;
        }

        mat = (Matrix) realloc(mat, (++(*n)) * sizeof(Vector));
        mat[(*n) - 1] = vector;
    }

    free(line);
    fclose(file);
    return mat;
}
