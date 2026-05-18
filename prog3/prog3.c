#include <stdio.h>
#include <string.h>

// ═══════════════════════════════════════════════════════
// FUNCTION 1: Print all bits of an integer
// ═══════════════════════════════════════════════════════
void printBits(int n) {
    printf("Bits of %d : ", n);
    for (int i = (sizeof(int) * 8) - 1; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
        if (i % 8 == 0) printf(" ");   // space every byte
    }
    printf("\n");
}

// ═══════════════════════════════════════════════════════
// FUNCTION 2: Count number of 1-bits (set bits)
// ═══════════════════════════════════════════════════════
int countOneBits(int n) {
    int count = 0;
    // Use unsigned to handle negative numbers correctly
    unsigned int un = (unsigned int)n;
    while (un != 0) {
        count += (un & 1);
        un >>= 1;
    }
    return count;
}

// ═══════════════════════════════════════════════════════
// FUNCTION 3: Check Little Endian or Big Endian
// ═══════════════════════════════════════════════════════
void checkEndianness() {
    unsigned int x = 1;
    char *c = (char *)&x;

    printf("\n─── ENDIANNESS ───────────────────────────────\n");
    if (*c == 1)
        printf("This system is : LITTLE ENDIAN\n");
    else
        printf("This system is : BIG ENDIAN\n");

    // Show memory layout of 0x12345678
    unsigned int val = 0x12345678;
    unsigned char *p = (unsigned char *)&val;
    printf("Memory layout of 0x12345678 (byte by byte):\n");
    for (int i = 0; i < sizeof(val); i++) {
        printf("  Address[%d] = 0x%02X\n", i, p[i]);
    }
    printf("Little Endian stores LSB first (0x78 at lowest address)\n");
    printf("Big    Endian stores MSB first (0x12 at lowest address)\n");
}

// ═══════════════════════════════════════════════════════
// FUNCTION 4: Bits in a NEGATIVE integer (2's complement)
// ═══════════════════════════════════════════════════════
void negativeBits() {
    printf("\n─── NEGATIVE INTEGER BITS ────────────────────\n");

    int pos =  5;
    int neg = -5;

    printBits(pos);
    printBits(neg);

    printf("\nHow -5 is stored (2's complement):\n");
    printf("  Step 1: Binary of  5  = 00000000 00000000 00000000 00000101\n");
    printf("  Step 2: Flip all bits  = 11111111 11111111 11111111 11111010\n");
    printf("  Step 3: Add 1         = 11111111 11111111 11111111 11111011\n");
    printf("  MSB (leftmost) = 1 means NEGATIVE\n");
    printf("  MSB (leftmost) = 0 means POSITIVE\n");

    printf("\n  +5  has %d one-bits\n", countOneBits(pos));
    printf("  -5  has %d one-bits\n", countOneBits(neg));
}

// ═══════════════════════════════════════════════════════
// FUNCTION 5: Print bits of a FLOAT (IEEE 754 format)
// ═══════════════════════════════════════════════════════
void printFloatBits(float f) {
    unsigned int bits;
    memcpy(&bits, &f, sizeof(bits));   // copy float bits to unsigned int

    printf("Float value     : %f\n", f);
    printf("Bits (IEEE 754) : ");

    for (int i = 31; i >= 0; i--) {
        printf("%d", (bits >> i) & 1);
        if (i == 31) printf(" | ");   // sign bit separator
        if (i == 23) printf(" | ");   // exponent separator
        if (i % 8 == 0 && i != 0 && i != 31 && i != 23)
            printf(" ");
    }
    printf("\n");

    // Extract IEEE 754 fields
    unsigned int sign     = (bits >> 31) & 0x1;
    unsigned int exponent = (bits >> 23) & 0xFF;
    unsigned int mantissa =  bits & 0x7FFFFF;

    printf("Sign bit        : %u  (%s)\n", sign, sign ? "Negative" : "Positive");
    printf("Exponent (8bit) : %u  (biased, actual = %d)\n", exponent, (int)exponent - 127);
    printf("Mantissa (23bit): %u\n\n", mantissa);
}

// ═══════════════════════════════════════════════════════
// MAIN
// ═══════════════════════════════════════════════════════
int main() {

    // ── 1. Print bits of a positive integer ──────────
    printf("═══ 1. PRINT BITS OF INTEGER ═════════════════\n");
    printBits(0);
    printBits(5);
    printBits(255);
    printBits(1024);

    // ── 2. Count 1-bits ───────────────────────────────
    printf("\n═══ 2. COUNT 1-BITS ══════════════════════════\n");
    int nums[] = {0, 5, 7, 255, -1, -5};
    int size   = sizeof(nums) / sizeof(nums[0]);
    for (int i = 0; i < size; i++) {
        printf("  %6d  →  %d one-bits\n", nums[i], countOneBits(nums[i]));
    }

    // ── 3. Endianness ─────────────────────────────────
    checkEndianness();

    // ── 4. Negative integer bits ──────────────────────
    printf("\n═══ 4. NEGATIVE INTEGER BITS ═════════════════\n");
    negativeBits();

    // ── 5. Float bits (positive) ──────────────────────
    printf("\n═══ 5. BITS IN A FLOAT (IEEE 754) ════════════\n");
    printf("Format: [Sign 1bit] | [Exponent 8bits] | [Mantissa 23bits]\n\n");
    printFloatBits(0.0f);
    printFloatBits(1.0f);
    printFloatBits(5.75f);

    // ── 6. Negative float bits ────────────────────────
    printf("═══ 6. BITS IN A NEGATIVE FLOAT ══════════════\n");
    printf("Format: [Sign 1bit] | [Exponent 8bits] | [Mantissa 23bits]\n\n");
    printFloatBits(-1.0f);
    printFloatBits(-5.75f);

    printf("\nKey observations:\n");
    printf("  Positive float : Sign bit = 0\n");
    printf("  Negative float : Sign bit = 1\n");
    printf("  Mantissa and Exponent are SAME for +5.75 and -5.75\n");
    printf("  Only the sign bit flips!\n");

    return 0;
}