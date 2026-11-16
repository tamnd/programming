```c run id=packed
#include <stdio.h>

static unsigned bcd_add_byte(unsigned a, unsigned b, int *carry) {
    unsigned s = a + b;
    if ((s & 0x0Fu) > 9 || (a & 0x0Fu) + (b & 0x0Fu) > 0x0Fu) {
        s += 0x06;
    }
    if (s > 0x99) {
        s += 0x60;
    }
    *carry = s > 0xFF;
    return s & 0xFFu;
}

static unsigned to_bcd_byte(unsigned n) {
    return (n / 10) << 4 | (n % 10);
}

static unsigned from_bcd_byte(unsigned b) {
    return (b >> 4) * 10 + (b & 0x0Fu);
}

int main(void) {
    int carry = 0;
    unsigned seconds = to_bcd_byte(59);
    printf("59 as a BCD byte: 0x%02X\n", seconds);
    printf("0x%02X read as BCD: %u, as binary: %u\n", seconds, from_bcd_byte(seconds), seconds);
    unsigned r = bcd_add_byte(0x09, 0x08, &carry);
    printf("09 + 08: %02X carry %s\n", r, carry ? "yes" : "no");
    r = bcd_add_byte(0x99, 0x01, &carry);
    printf("99 + 01: %02X carry %s\n", r, carry ? "yes" : "no");
    printf("0x09 + 0x08 without the correction: %02X\n", 0x09 + 0x08);
    return 0;
}
```

```output for=packed
59 as a BCD byte: 0x59
0x59 read as BCD: 59, as binary: 89
09 + 08: 17 carry no
99 + 01: 00 carry yes
0x09 + 0x08 without the correction: 11
```

- Verdict: idiom. C has no BCD type. The block converts one byte, as a driver does for the BCD time registers of a real-time clock, and adds two packed bytes with the add-6 correction.
- The x86 `DAA` instruction does the correction in hardware, but it is not valid in 64-bit mode, so the code does it in C.
