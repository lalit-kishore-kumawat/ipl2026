#include <stdio.h>
#include <string.h>

// ─── 1. print bits of an integer ───────────────────────────────────────────

void print_bits_int(int n) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0) printf(" ");
    }
    printf("\n");
}

// ─── 2. count number of 1 bits ─────────────────────────────────────────────

int count_ones(int n) {
    int count = 0;
    // brian kernighan's trick - clears lowest set bit each time
    unsigned int un = (unsigned int)n; // use unsigned so right shift is safe
    while (un) {
        un = un & (un - 1);
        count++;
    }
    return count;
}

// ─── 3. check endianness ───────────────────────────────────────────────────

void check_endian() {
    int x = 1;
    char *ptr = (char*)&x;
    if (*ptr == 1)
        printf("this machine is LITTLE ENDIAN (least significant byte first)\n");
    else
        printf("this machine is BIG ENDIAN (most significant byte first)\n");

    // show how 0x12345678 sits in memory
    unsigned int val = 0x12345678;
    unsigned char *bytes = (unsigned char*)&val;
    printf("memory layout of 0x12345678 → ");
    for (int i = 0; i < 4; i++)
        printf("[0x%02X] ", bytes[i]);
    printf("\n");
}

// ─── 4. print bits of a float ──────────────────────────────────────────────

void print_bits_float(float f) {
    unsigned int temp;
    memcpy(&temp, &f, sizeof(f)); // safe way to reinterpret float as int

    printf("sign | exponent | mantissa\n");
    printf("  ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (temp >> i) & 1);
        if (i == 31) printf("  |  ");       // after sign bit
        else if (i == 23) printf("  |  ");  // after exponent
        else if (i > 0 && (i) % 4 == 0 && i != 24) printf(" "); // group mantissa
    }
    printf("\n");
}

// ─── main ──────────────────────────────────────────────────────────────────

int main() {

    // --- integer bits ---
    printf("======= PRINT BITS (integer) =======\n");
    int a = 5;
    printf("%d  → ", a);
    print_bits_int(a);

    int b = 100;
    printf("%d → ", b);
    print_bits_int(b);

    // --- count 1 bits ---
    printf("\n======= COUNT 1-BITS =======\n");
    printf("5   → %d one-bits\n", count_ones(5));
    printf("100 → %d one-bits\n", count_ones(100));
    printf("255 → %d one-bits\n", count_ones(255)); // should be 8

    // --- endianness ---
    printf("\n======= ENDIANNESS =======\n");
    check_endian();

    // --- negative integer bits (two's complement) ---
    printf("\n======= NEGATIVE INTEGER BITS (two's complement) =======\n");
    int pos = 5;
    int neg = -5;
    printf(" 5  → ");
    print_bits_int(pos);
    printf("-5  → ");
    print_bits_int(neg);
    printf("notice: to get -5, flip all bits of 5 then add 1\n");
    printf("MSB (leftmost bit) is 1 which means it's negative\n");
    printf("count of 1-bits in -5: %d\n", count_ones(neg));

    // --- float bits ---
    printf("\n======= FLOAT BITS (IEEE 754) =======\n");
    float f1 = 5.0f;
    printf("5.0 bits:\n");
    print_bits_float(f1);
    printf("structure: [1 sign bit] [8 exponent bits] [23 mantissa bits]\n");
    printf("5.0 = 1.01 x 2^2  →  exponent stored = 2+127 = 129 = 10000001\n");

    // --- negative float bits ---
    printf("\n======= NEGATIVE FLOAT BITS =======\n");
    float f2 = -5.0f;
    printf(" 5.0 bits:\n  ");
    print_bits_float(f1);
    printf("-5.0 bits:\n  ");
    print_bits_float(f2);
    printf("only the sign bit flips — floats do NOT use two's complement\n");

    return 0;
}