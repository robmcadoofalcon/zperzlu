#include <stdio.h>
#include <inttypes.h>
#include <emmintrin.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <smmintrin.h>
#include <sys/time.h>

#define BUFFER_SIZE 1024*1024*20

uint8_t buffer[BUFFER_SIZE*2];

extern int offsets[65535];
extern char shuffle_control_masks[65535*16];

void jumble(size_t *, uint8_t *);

int main(int argc, char **argv) {
    size_t bytes_read = 0;
    struct timeval t1, t2;

    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    if (argc < 2) {
        printf("Usage: puzzler <input>\n");
        return 1;
    }
    FILE *fd = fopen(argv[1], "r");

    setvbuf(fd, (char *)NULL, _IONBF, 0);
    if(!fd) {
        printf("Could not open input file. Exiting.\n");
        return 2;
    }
    _mm_mfence();
    uint64_t before = __rdtsc();
    do {
        bytes_read = fread(buffer, sizeof(char), BUFFER_SIZE, fd);
        jumble(&bytes_read, buffer);
        printf("%.*s", (int) bytes_read, buffer);
    } while(__builtin_expect(bytes_read == BUFFER_SIZE, 0));
    uint64_t taken = __rdtsc() - before;
    //printf("Cycles taken: %llu\n", taken);
    return 0;
}

void jumble(size_t *bytes_read, uint8_t *buffer) {
    __m128i v_in, is_alpha, is_number, is_alphanum;
    __m128i to_upper = _mm_set1_epi8(0x20);

    for(size_t i = 0; i < *bytes_read; i+=16) {
        // Is this an alpha character?
        v_in = _mm_load_si128((__m128i *)&buffer[i]);        
        is_alpha = _mm_or_si128(v_in, to_upper);
        is_alpha = _mm_and_si128(
                        _mm_cmpgt_epi8(is_alpha, _mm_set1_epi8(0x60)),
                        _mm_cmplt_epi8(is_alpha, _mm_set1_epi8(0x7B)));
        // Is it a number?
        is_number = _mm_and_si128(
                _mm_cmpgt_epi8(v_in, _mm_set1_epi8(0x2F)),
                _mm_cmplt_epi8(v_in, _mm_set1_epi8(0x3A)));
        // Is it an alphanumeric?
        is_alphanum = _mm_or_si128(is_alpha, is_number);
        // Lookup the mask in the lookup table and load the shuffle control mask.
        int res = _mm_movemask_epi8(is_alphanum);
        __m128i shuffle_cm = _mm_load_si128((__m128i *) &shuffle_control_masks[res*16]);
        __m128i shuffled = _mm_shuffle_epi8(v_in, shuffle_cm);
        _mm_storeu_si128((__m128i *)&buffer[i], shuffled);
    }
}
