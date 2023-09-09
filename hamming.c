#include "hamming.h"

#include "bm.h"
#include "bv.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int lookup[16] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR,
    1, 0, HAM_ERR };

uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, bm_rows(G));
    BitMatrix *c = bm_multiply(m, G);
    uint8_t code = bm_to_data(c);
    return code;
}

HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    BitMatrix *c = bm_from_data(code, bm_rows(Ht));
    BitMatrix *e = bm_multiply(c, Ht);
    uint8_t error = bm_to_data(e);
    int L = lookup[error];
    if (L == HAM_OK) {
        *msg = error;
        return HAM_OK;
    } else if (L == HAM_ERR) {
        *msg = error;
        return HAM_ERR;
    } else {
        code ^= (L << 1);
        *msg = code;
        return HAM_CORRECT;
    }
}
