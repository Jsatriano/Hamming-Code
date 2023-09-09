#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "hi:o:"

uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

int main(int argc, char **argv) {
    int opt = 0;
    int byte;
    bool got_i = false;
    bool got_o = false;
    char *input_file;
    char *output_file;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h':
            puts("PROGRAM SUMMARY");
            puts("A Hamming(8, 4) systematic code generator.");
            puts("");
            puts("EXAMPLE INPUT TO COMMAND LINE");
            puts("./encode [-h] [-i infile] [-o outfile]");
            puts("");
            puts("OPTIONS");
            puts("-h             Program usage and help.");
            puts("-i infile      Input data to encode.");
            puts("-o outfile     Output of encoded data.");
            return 0;
        case 'i':
            got_i = true;
            input_file = optarg;
            break;
        case 'o':
            got_o = true;
            output_file = optarg;
            break;
        default: break;
        }
    }
    if (got_i) {
        infile = fopen(input_file, "r");
        if (infile == NULL) {
            fprintf(stderr, "failed to open %s\n", input_file);
            return 1;
        }
    }
    if (got_o) {
        outfile = fopen(output_file, "w");
        if (outfile == NULL) {
            fprintf(stderr, "failed to open %s\n", output_file);
            return 1;
        }
    }
    struct stat statbuf; //set permissions of outfile to be equal to infile
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    BitMatrix *G = bm_create(4, 8);
    for (uint32_t i = 0; i < bm_rows(G); i += 1) {
        if (i == 0) {
            bm_set_bit(G, i, 0);
            bm_set_bit(G, i, 5);
            bm_set_bit(G, i, 6);
            bm_set_bit(G, i, 7);
        }
        if (i == 1) {
            bm_set_bit(G, i, 1);
            bm_set_bit(G, i, 4);
            bm_set_bit(G, i, 6);
            bm_set_bit(G, i, 7);
        }
        if (i == 2) {
            bm_set_bit(G, i, 2);
            bm_set_bit(G, i, 4);
            bm_set_bit(G, i, 5);
            bm_set_bit(G, i, 7);
        }
        if (i == 3) {
            bm_set_bit(G, i, 3);
            bm_set_bit(G, i, 4);
            bm_set_bit(G, i, 5);
            bm_set_bit(G, i, 6);
        }
    }
    while ((byte = fgetc(infile)) != EOF) {
        uint8_t lower = lower_nibble(byte);
        uint8_t upper = upper_nibble(byte);
        uint8_t code_low = ham_encode(G, lower);
        fputc(code_low, outfile);
        uint8_t code_upp = ham_encode(G, upper);
        fputc(code_upp, outfile);
    }

    fclose(infile);
    fclose(outfile);
    bm_delete(&G);
    return 0;
}
