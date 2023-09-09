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

#define OPTIONS "hvi:o:"

uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

int main(int argc, char **argv) {
    int opt = 0;
    int lower = 0;
    int upper = 0;
    uint8_t lower_dc = 0;
    uint8_t upper_dc = 0;
    bool got_i = false;
    bool got_o = false;
    bool got_v = false;
    bool verbose = false;
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
            puts("./decode [-h] [-v] [-i infile] [-o outfile]");
            puts("");
            puts("OPTIONS");
            puts("-h             Program usage and help.");
            puts("-v		 print stats of decoding to stderr.");
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
        case 'v': got_v = true; break;
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
    if (got_v) {
        verbose = true;
    }
    struct stat statbuf; //set permissions of outfile to be equal to infile
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    BitMatrix *Ht = bm_create(8, 4);
    for (uint32_t i = 0; i < bm_rows(Ht); i += 1) {
        if (i == 0) {
            bm_set_bit(Ht, i, 1);
            bm_set_bit(Ht, i, 2);
            bm_set_bit(Ht, i, 3);
        }
        if (i == 1) {
            bm_set_bit(Ht, i, 0);
            bm_set_bit(Ht, i, 2);
            bm_set_bit(Ht, i, 3);
        }
        if (i == 2) {
            bm_set_bit(Ht, i, 0);
            bm_set_bit(Ht, i, 1);
            bm_set_bit(Ht, i, 3);
        }
        if (i == 3) {
            bm_set_bit(Ht, i, 0);
            bm_set_bit(Ht, i, 1);
            bm_set_bit(Ht, i, 2);
        }
        if (i == 4) {
            bm_set_bit(Ht, i, 0);
        }
        if (i == 5) {
            bm_set_bit(Ht, i, 1);
        }
        if (i == 6) {
            bm_set_bit(Ht, i, 2);
        }
        if (i == 7) {
            bm_set_bit(Ht, i, 3);
        }
    }

    int bytes_proc = 0;
    int uncorr_err = 0;
    int corr_err = 0;
    float err_rate;
    int8_t decoded_low;
    int8_t decoded_up;
    while ((lower = fgetc(infile)) != EOF) {
        decoded_low = ham_decode(Ht, lower, &lower_dc);
        if (decoded_low == HAM_ERR) {
            uncorr_err += 1;
        } else if (decoded_low == HAM_CORRECT) {
            corr_err += 1;
        }
        upper = fgetc(infile);
        decoded_up = ham_decode(Ht, upper, &upper_dc);
        if (decoded_up == HAM_ERR) {
            uncorr_err += 1;
        } else if (decoded_up == HAM_CORRECT) {
            corr_err += 1;
        }

        bytes_proc += 2;
        uint8_t packed = pack_byte(upper_dc, lower_dc);
        fputc(packed, outfile);
    }

    err_rate = (float) uncorr_err / (float) bytes_proc;

    if (verbose) {
        fprintf(stderr, "Total bytes processed: %u\n", bytes_proc);
        fprintf(stderr, "Uncorrected errors: %u\n", uncorr_err);
        fprintf(stderr, "Corrected errors: %u\n", corr_err);
        fprintf(stderr, "Error rate: %f\n", err_rate);
    }

    fclose(infile);
    fclose(outfile);
    bm_delete(&Ht);

    return 0;
}
