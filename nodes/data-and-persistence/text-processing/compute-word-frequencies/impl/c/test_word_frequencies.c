// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

#include "word_frequencies.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *path = "word_frequencies_input.txt";

static void write_file(const char *data, size_t n) {
    FILE *f = fopen(path, "wb");
    assert(f != NULL);
    assert(fwrite(data, 1, n, f) == n);
    assert(fclose(f) == 0);
}

// want is a list of words and counts, as "the 2 and 1", or "" for no words.
static void check(const char *name, const char *data, size_t size, size_t n, const char *want) {
    write_file(data, size);
    struct word_freq *words;
    size_t len;
    if (word_frequencies(path, n, &words, &len) != 0) {
        fprintf(stderr, "case %s: error %d\n", name, errno);
        exit(1);
    }
    char got[256] = "";
    for (size_t i = 0; i < len; i++) {
        size_t used = strlen(got);
        snprintf(got + used, sizeof got - used, "%s%s %zu", i > 0 ? " " : "", words[i].word, words[i].count);
    }
    word_frequencies_free(words, len);
    if (strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got \"%s\", want \"%s\"\n", name, got, want);
        exit(1);
    }
}

// sizeof counts the bytes of the literal, but not the final NUL.
#define CASE(name, data, n, want) check(name, data, sizeof(data) - 1, n, want)

static void check_repeated(const char *name, const char *unit, size_t times, size_t n, const char *want) {
    size_t size = strlen(unit) * times;
    char *data = malloc(size + 1);
    assert(data != NULL);
    for (size_t i = 0; i < times; i++) {
        memcpy(data + i * strlen(unit), unit, strlen(unit));
    }
    check(name, data, size, n, want);
    free(data);
}

int main(void) {
    CASE("simple", "the cat and the hat\n", 2, "the 2 and 1");
    CASE("case", "The THE the tHe\n", 5, "the 4");
    CASE("punctuation", "it's well-dressed, isn't it?\n", 10, "it 2 dressed 1 isn 1 s 1 t 1 well 1");
    CASE("empty", "", 3, "");
    CASE("no-letters", "123 ... 42\n-- !!\n", 3, "");
    CASE("ties", "b a c b a c\n", 2, "a 2 b 2");
    CASE("digits", "abc123def 42 x9\n", 10, "abc 1 def 1 x 1");
    CASE("n-zero", "the cat and the hat\n", 0, "");
    CASE("n-above-count", "x y x\n", 10, "x 2 y 1");
    CASE("crlf", "one two\r\ntwo\r\n", 5, "two 2 one 1");
    CASE("non-ascii", "caf\xc3\xa9 na\xc3\xafve cafe\n", 10, "caf 1 cafe 1 na 1 ve 1");
    CASE("leading-separators", "  ...hello, world. Hello\n", 5, "hello 2 world 1");
    CASE("no-final-newline", "dog cat dog", 5, "dog 2 cat 1");
    CASE("not-utf8", "ok \xff\xfe ok\n", 5, "ok 2");
    check_repeated("many-lines", "a b a\n", 100000, 3, "a 200000 b 100000");
    // One line of 100000 words: longer than the buffer of the stream.
    check_repeated("long-line", "word ", 100000, 3, "word 100000");
    // 1000 different words make the table grow more than once.
    char many[8000] = "";
    for (int i = 0; i < 1000; i++) {
        char w[8] = {(char)('a' + i / 100 % 26), (char)('a' + i / 10 % 10), (char)('a' + i % 10), ' ', 0};
        strcat(many, w);
    }
    strcat(many, "aaa");
    check("many-words", many, strlen(many), 2, "aaa 2 aab 1");

    struct word_freq *words;
    size_t len;
    errno = 0;
    if (word_frequencies("no/such/dir/missing.txt", 3, &words, &len) != -1 || errno != ENOENT) {
        fprintf(stderr, "case not-found: no ENOENT error\n");
        exit(1);
    }
    remove(path);
    puts("all cases pass");
    return 0;
}
