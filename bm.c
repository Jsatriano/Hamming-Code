#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

typedef struct BitMatrix BitMatrix;

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    m->vector = bv_create(rows * cols);
    m->rows = rows;
    m->cols = cols;
    return m;
}

void bm_delete(BitMatrix **m) {
    if (*m) {
        bv_delete(&(*m)->vector);
        free(*m);
        *m = NULL;
    }
    return;
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_set_bit(m->vector, r * m->cols + c);
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_clr_bit(m->vector, r * m->cols + c);
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t result = bv_get_bit(m->vector, r * m->cols + c);
    return result;
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length);
    for (uint32_t i = 0; i < length; i += 1) {
        if (byte & (1 << i)) {
            bm_set_bit(m, 0, i);
        } else {
            bm_clr_bit(m, 0, i);
        }
    }
    return m;
}

uint8_t bm_to_data(BitMatrix *m) {
    uint8_t value = 0;
    for (uint32_t i = 0; i < bv_length(m->vector); i += 1) {
        if (bm_get_bit(m, 0, i) == 1) {
            value ^= (1 << i);
        }
    }
    return value;
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    BitMatrix *ans = bm_create(A->rows, B->cols);
    for (uint32_t i = 0; i < A->rows; i += 1) {
        for (uint32_t j = 0; j < B->cols; j += 1) {
            uint8_t sum = 0;
            for (uint32_t k = 0; k < A->cols; k += 1) {
                sum ^= bm_get_bit(A, i, k) & bm_get_bit(B, k, j);
            }
            if (sum == 1) {
                bm_set_bit(ans, i, j);
            } else {
                bm_clr_bit(ans, i, j);
            }
        }
    }
    return ans;
}

/*
void bm_print(BitMatrix *m) {
    
}
*/
