// This program calculates a bias table for the Azrael hash function
//
// Searches for the different input bytes are parallelized with multithreading.

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ---
// The following defines are taken from
// https://github.com/jmedina2099/HashUniforme1.0/commit/b4a5e47aff8366002c663f76d37ee2fbbea86224
// ---

/*
 * Copyright (C) 2007-2020 Jorge Medina - <medinarosas.jorgealberto@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#define C0(x1,x2,x3,x4,x5) ((( x1 + x2 ) ^ ( x3 ^ x4 )) ^ x5)
#define C1(x1,x2,x3,x4,x5) ((( x1 & x2 ) ^ ( x3 + x4 )) ^ x5)
#define C2(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) + ( x3 + x4 )) ^ x5)
#define C3(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) ^ ( x3 + x4 )) ^ x5)
#define C4(x1,x2,x3,x4,x5) ((( x1 & x2 ) + ( x3 + x4 )) + x5)
#define C5(x1,x2,x3,x4,x5) ((( x1 & x2 ) + ( x3 + x4 )) ^ x5)
#define C6(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) ^ ( x3 + x4 )) ^ x5)
#define C7(x1,x2,x3,x4,x5) ((( x1 | x2 ) ^ ( x3 + x4 )) ^ x5)
#define C8(x1,x2,x3,x4,x5) ((  x1 | x2 ) | ((x3 + x4 )  ^ x5))
#define C9(x1,x2,x3,x4,x5) ((( x1 + x2 ) + ( x3 + x4 )) ^ x5)
#define CA(x1,x2,x3,x4,x5) ((( x1 + x2 ) & ( x3 + x4 )) ^ x5)
#define CB(x1,x2,x3,x4,x5) ((( x1 ^ x2 ) + ( x3 ^ x4 )) ^ x5)
#define CC(x1,x2,x3,x4,x5) ((( x1 | x2 ) ^ ( x3 ^ x4 )) ^ x5)
#define CD(x1,x2,x3,x4,x5) ((( x1 + x2 ) + ( x3 ^ x4 )) ^ x5)
#define CE(x1,x2,x3,x4,x5) ((( x1 + x2 ) ^ ( x3 + x4 )) ^ x5)
#define CF(x1,x2,x3,x4,x5) ((  x1 + x2 ) & ((x3 + x4 )  + x5))

#define COMPRESS_320(x1,x2,x3,x4,x5) \
  + C0(x1,x2,x3,x4,x5) + C1(x1,x2,x3,x4,x5) + C2(x1,x2,x3,x4,x5) + C3(x1,x2,x3,x4,x5) \
  + C4(x1,x2,x3,x4,x5) + C5(x1,x2,x3,x4,x5) + C6(x1,x2,x3,x4,x5) + C7(x1,x2,x3,x4,x5) \
  + C8(x1,x2,x3,x4,x5) + C9(x1,x2,x3,x4,x5) + CA(x1,x2,x3,x4,x5) + CB(x1,x2,x3,x4,x5) \
  + CC(x1,x2,x3,x4,x5) + CD(x1,x2,x3,x4,x5) + CE(x1,x2,x3,x4,x5) + CF(x1,x2,x3,x4,x5)

// ---
// end
// ---

// // alternative 5:1 compression function based on SplitMix64:
//
// //a state of the art hash function for individual 64-bit values
// static uint64_t splitmix(uint64_t x) {
//     x ^= x >> 30;
//     x *= 0xbf58476d1ce4e5b9;
//     x ^= x >> 27;
//     x *= 0x94d049bb133111eb;
//     x ^= x >> 31;
//     return x;
// }
//
// static uint64_t compress_320_splitmix(uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4, uint64_t x5) {
//     // feed one input at a time into the state to ensure that order matters
//     uint64_t state = 0;
//     state = splitmix(state ^ x1);
//     state = splitmix(state ^ x2);
//     state = splitmix(state ^ x3);
//     state = splitmix(state ^ x4);
//     state = splitmix(state ^ x5);
//     return state;
// }
//
// #define COMPRESS_320(x1, x2, x3, x4, x5) (compress_320_splitmix(x1, x2, x3, x4, x5) & 0xFF)

/// zero all entries
static void initialize_flips_table(size_t flips[5][8][8]) {
  for (int i_input = 0; i_input < 5; i_input++)
    for (int i_bit = 0; i_bit < 8; i_bit++)
      for (int i_out = 0; i_out < 8; i_out++)
        flips[i_input][i_bit][i_out] = 0;
}

/// Given the pattern x, flip all bits in the i'th byte and count the flips
static void update_table_with_flips(int i_input, size_t flips[8][8], uint8_t x[5]) {
    uint8_t initial = x[i_input];

    uint8_t y1=x[0],y2=x[1],y3=x[2],y4=x[3],y5=x[4];

    x[0] += y1;
    x[1] += x[2];
    x[2] += x[3];
    x[3] += initial;
    x[4] += y2;
    y5 += y4;
    y4 += y3;
    y3 += y2;
    y2 += y1;
    uint8_t y_initial = COMPRESS_320( x[0], x[1], x[2], x[3], x[4] );

    for (int i_bit = 0; i_bit < 8; i_bit++) {
        // set up the input with the flipped bit
        x[i_input] = initial ^ (1 << i_bit);  // flip the bit

    	x[0] += y1;
    	x[1] += x[2];
    	x[2] += x[3];
    	x[3] += x[i_input];
    	x[4] += y2;
        y5 += y4;
        y4 += y3;
        y3 += y2;
        y2 += y1;
        y1 += COMPRESS_320( x[0], x[1], x[2], x[3], x[4] );

        // increment the flip counter if a bit was flipped
        for (int i = 0; i < 8; i++)
            flips[i_bit][i] += ((y_initial ^ y1) >> i) & 1;
    }
}

struct WorkerArgs {
    size_t (*flips)[8][8];
    int i_input;
};

#define N_BIT_PATTERNS 18
static const uint8_t PATTERNS[N_BIT_PATTERNS] = {
    // all-zero
    0b00000000,
    // single-bit
    0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00010000,
    0b00100000, 0b01000000, 0b10000000,
    // sets of 2 / alternating
    0b01010101, 0b10101010,
    // sets of 3
    0b00100100, 0b01001001, 0b10010010,
    0b01101101, 0b11011011, 0b10110110,
    // all-one
    0b11111111,
};

// the function that is executed per thread: count flips for a given input byte
static void* worker(void* args) {
    struct WorkerArgs* worker_args = args;
    size_t i_input = worker_args->i_input;
    assert(0 <= i_input && i_input < 5);

    // assemble various bit patterns, fairly ugly code
    for (unsigned xi = 0; xi <= 0xFF; xi++) {
        for (unsigned xa = 0; xa < N_BIT_PATTERNS; xa++) {
          for (unsigned xb = 0; xb < N_BIT_PATTERNS; xb++) {
            for (unsigned xc = 0; xc < N_BIT_PATTERNS; xc++) {
              for (unsigned xd = 0; xd < N_BIT_PATTERNS; xd++) {
                  uint8_t i_pattern[4] = {xa, xb, xc, xd};
                  uint8_t x[5] = {0};
                  for (int i = 0; i < 5; i++) {
                      if (i < i_input)
                          x[i] = PATTERNS[i_pattern[i]];
                      else if (i == i_input)
                          x[i] = xi;
                      else
                          x[i] = PATTERNS[i_pattern[i-1]];
                  }
                  update_table_with_flips(i_input, *worker_args->flips, x);
              }
            }
          }
        }
    }

    return NULL;
}

int main(int argc, char** argv) {
    size_t flips[5][8][8];
    initialize_flips_table(flips);

    // test all 8-bit numbers
    pthread_t threads[5];
    struct WorkerArgs args[5];
    for (int i_input = 0; i_input < 5; i_input++) {
        struct WorkerArgs a = {.flips = &flips[i_input],
                               .i_input = i_input};
        args[i_input] = a;
        int err = pthread_create(&threads[i_input], NULL, worker, &args[i_input]);
        if (err) {
            fprintf(stderr, "ERROR starting threads\n");
            for (int i = 0; i < i_input; i++)
                pthread_join(threads[i], NULL);
            exit(1);
        }
    }

    // stop the threads
    for (int i = 0; i < 5; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err) {
            fprintf(stderr, "ERROR joining threads\n");
            exit(1);
        }
    }

    // print the table
    printf("counts:\n");
    for (int i_input = 0; i_input < 5; i_input++) {
        for (int i_bit = 0; i_bit < 8; i_bit++) {
            size_t* row = flips[i_input][i_bit];
            printf("%d %d : %15zu %15zu %15zu %15zu %15zu %15zu %15zu %15zu\n",
                   i_input, i_bit,
                   row[7], row[6], row[5], row[4], row[3], row[2], row[1], row[0]);
        }
    }

    printf("probabilities:\n");
    printf("| in   | out 7 | out 6 | out 5 | out 4 | out 3 | out 2 | out 1 | out 0 |\n");
    size_t total_flips_per_bit = 0x100 * N_BIT_PATTERNS * N_BIT_PATTERNS * N_BIT_PATTERNS * N_BIT_PATTERNS;
    for (int i_input = 0; i_input < 5; i_input++) {
        for (int i_bit = 0; i_bit < 8; i_bit++) {
            size_t* row = flips[i_input][i_bit];
            float probs[8];
            for (int i = 0; i < 8; i++)
                probs[i] = (float) row[i] / total_flips_per_bit;
            printf("| %d[%d] | %.3f | %.3f | %.3f | %.3f | %.3f | %.3f | %.3f | %.3f |\n",
                   i_input, i_bit, probs[7], probs[6], probs[5], probs[4], probs[3],
                   probs[2], probs[1], probs[0]);
        }
    }
}
