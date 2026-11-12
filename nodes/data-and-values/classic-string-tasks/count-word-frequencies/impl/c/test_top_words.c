// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-word-frequencies/.

#include "top_words.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *text, size_t n, const struct word_count *want,
                  size_t want_len) {
    struct word_counts got;
    int ok = top_words(text, n, &got) == 0 && got.len == want_len;
    for (size_t i = 0; ok && i < want_len; i++) {
        ok = strcmp(got.items[i].word, want[i].word) == 0 && got.items[i].count == want[i].count;
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    word_counts_free(&got);
}

int main(void) {
    check("sentence", "the cat and the hat and the bat", 3, ((const struct word_count[]){{"the", 3}, {"and", 2}, {"bat", 1}}), 3);
    check("all-words", "one two two three three three", 10, ((const struct word_count[]){{"three", 3}, {"two", 2}, {"one", 1}}), 3);
    check("punctuation", "Stop. Stop, stop! Go?", 2, ((const struct word_count[]){{"stop", 3}, {"go", 1}}), 2);
    check("lines", "red green\nblue red\ngreen red\n", 2, ((const struct word_count[]){{"red", 3}, {"green", 2}}), 2);
    check("empty", "", 3, NULL, 0);
    check("no-words", " ... ,!? \n", 3, NULL, 0);
    check("case", "Word word WORD wOrD", 1, ((const struct word_count[]){{"word", 4}}), 1);
    check("ties", "pear fig apple fig pear apple", 3, ((const struct word_count[]){{"apple", 2}, {"fig", 2}, {"pear", 2}}), 3);
    check("fold-before-sort", "Zebra apple", 2, ((const struct word_count[]){{"apple", 1}, {"zebra", 1}}), 2);
    check("n-zero", "a b c", 0, NULL, 0);
    check("n-above-count", "a b a", 10, ((const struct word_count[]){{"a", 2}, {"b", 1}}), 2);
    check("non-ascii", "café naïve café 日本語 naïve café", 3, ((const struct word_count[]){{"café", 3}, {"naïve", 2}, {"日本語", 1}}), 3);
    check("code-point-order", "f é e", 3, ((const struct word_count[]){{"e", 1}, {"f", 1}, {"é", 1}}), 3);
    check("apostrophe", "don't stop, don't", 3, ((const struct word_count[]){{"don", 2}, {"t", 2}, {"stop", 1}}), 3);
    check("digits", "route 66 and route 66a 66", 3, ((const struct word_count[]){{"66", 2}, {"route", 2}, {"66a", 1}}), 3);
    check("separators", "tab\there\r\nunder_score-dash", 10, ((const struct word_count[]){{"dash", 1}, {"here", 1}, {"score", 1}, {"tab", 1}, {"under", 1}}), 5);
    return failures == 0 ? 0 : 1;
}
