#include "bv.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length; //length in bits
    uint8_t *vector; //array of bytes
};

typedef struct BitVector BitVector;

BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    v->length = length;
    uint32_t bits = length;
    if (bits % 8 == 0) {
        v->vector = (uint8_t *) calloc(bits / 8, sizeof(uint8_t));
    } else {
        v->vector = (uint8_t *) calloc((bits / 8) + 1, sizeof(uint8_t));
    }
    if (!v->vector) {
        free(v);
        v = NULL;
    }
    return v;
}

void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
    return;
}

uint32_t bv_length(BitVector *v) {
    return v->length;
}

void bv_set_bit(BitVector *v, uint32_t i) {
    v->vector[i / 8] |= (1 << (i % 8));
}

void bv_clr_bit(BitVector *v, uint32_t i) {
    v->vector[i / 8] &= ~(1 << (i % 8));
}

void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    v->vector[i / 8] ^= (bit << (i % 8));
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    uint8_t result = v->vector[i / 8] & (1 << (i % 8));
    uint8_t ans = result >> (i % 8);
    return ans;
}

/*
void bv_print(BitVector *v) {
    
}
*/
