#include <cstdint>
#include <stddef.h>


constexpr void swap(uint8_t *arr, size_t l, size_t r) {
    int t = arr[l];
    arr[l] = arr[r];
    arr[r] = t;
}

// This random table can be eliminated in favour of passing the state as an inout to shuffle.
template<size_t N, size_t W>
struct RandomNumberTable {
    constexpr RandomNumberTable() : values() {
        __uint128_t g_lehmer64_state = 0;
        for(auto c = 0; c < 8; c++) {
            g_lehmer64_state |= __DATE__[c];
            g_lehmer64_state <<= 1;
        }
        for (size_t i = 0; i < N; i++) {
            for (size_t j = 0; j < W; j++) {
                g_lehmer64_state *= 0xda942042e4dd58b5;
                values[i][j] = g_lehmer64_state >> 64;
            }
        }
    }
    size_t values[N][W];
};

constexpr auto randomTable = RandomNumberTable<65536,16>();

constexpr void shuffle(uint8_t *arr, size_t row, size_t l, size_t r) {
   for(int i = (16*row)+l; i < r; i++) {
        int rand = (randomTable.values[row][i] % (r - l)) + l;
        swap(arr, i, rand);
   }
}

constexpr size_t max(uint8_t x, uint8_t y) {
    return x ^ ((x ^ y) & -(x < y));
}

template<size_t N, size_t W>
struct LookupTable {
    constexpr LookupTable() : values() {
      const int yoDawg[2] = {0x0000, 0xFFFF};
        for (size_t i = 0; i < N; i++) {
            uint8_t left = 0;
            uint8_t right = 0;
          size_t iVal = i;
            for (size_t j = 0; j < W; j++) {
                const size_t bit = iVal & 1;
                right = (15-j) & yoDawg[bit];
              values[i][j] = (uint8_t) 15-j;
                //shuffle(values[i], i, left, right);
                left = max(left, right);
                iVal >>= 1;
            }
        }
    }
    uint8_t values[N][W];
};


constexpr LookupTable<65536,16> lookupTable = LookupTable<65536,16>(); 


