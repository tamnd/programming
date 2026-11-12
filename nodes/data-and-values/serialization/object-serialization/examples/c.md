```c run id=explicit-byte-order
#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct reading {
    uint8_t sensor;
    uint32_t value;
    uint16_t flags;
};

/* Write each field byte by byte, little-endian, with no padding. */
static size_t encode(const struct reading *r, unsigned char *out) {
    out[0] = r->sensor;
    for (int i = 0; i < 4; i++) out[1 + i] = (unsigned char)(r->value >> (8 * i));
    for (int i = 0; i < 2; i++) out[5 + i] = (unsigned char)(r->flags >> (8 * i));
    return 7;
}

static struct reading decode(const unsigned char *in) {
    struct reading r = {0};
    r.sensor = in[0];
    for (int i = 0; i < 4; i++) r.value |= (uint32_t)in[1 + i] << (8 * i);
    for (int i = 0; i < 2; i++) r.flags |= (uint16_t)(in[5 + i] << (8 * i));
    return r;
}

int main(void) {
    struct reading r = {7, 1000000, 0x8001};
    unsigned char buf[16];
    size_t n = encode(&r, buf);
    printf("struct in memory: %zu bytes, encoded: %zu bytes\n", sizeof r, n);
    for (size_t i = 0; i < n; i++) printf("%02x%s", buf[i], i + 1 < n ? " " : "\n");
    struct reading back = decode(buf);
    printf("round trip: %d\n", back.sensor == r.sensor && back.value == r.value && back.flags == r.flags);
    return 0;
}
```

```output for=explicit-byte-order
struct in memory: 12 bytes, encoded: 7 bytes
07 40 42 0f 00 01 80
round trip: 1
```

- Verdict: idiom. C has no serialization. Writing a struct with `fwrite` copies its padding and its byte order, which depend on the compiler and the machine; here the struct takes 12 bytes in memory and 7 in the encoded form. The idiom writes each field byte by byte in a fixed order, or uses a library such as Protocol Buffers for C.
