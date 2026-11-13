/* SPDX-License-Identifier: MIT */
/* Part of https://programming.tamnd.com/embed-a-file-in-a-program/. */

#include "embed_file.h"

#include <string.h>

/* The preprocessor reads lines.txt, relative to this source file, and puts
   its bytes here as a list of integers. A compiler with no #embed does not
   define __has_embed, so it uses the same list from a header. */
static const unsigned char data[] = {
#if defined(__has_embed)
#embed "lines.txt"
#else
#include "lines_txt.h"
#endif
};

const unsigned char *embedded_data(size_t *len) {
    *len = sizeof data;
    return data;
}

const char *embedded_line(long n, size_t *len) {
    const char *p = (const char *)data;
    const char *end = p + sizeof data;
    if (n < 1) {
        return NULL;
    }
    /* Skip the first line, the license header of the file. */
    const char *header_end = memchr(p, '\n', sizeof data);
    if (header_end == NULL) {
        return NULL;
    }
    p = header_end + 1;
    for (long i = 1; p < end; i++) {
        const char *nl = memchr(p, '\n', (size_t)(end - p));
        if (nl == NULL) {
            nl = end; /* a last line with no "\n" */
        }
        if (i == n) {
            *len = (size_t)(nl - p);
            return p;
        }
        p = nl + 1;
    }
    return NULL;
}
