// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

#include "reformat_dates.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name, *text, *from_sep, *to_sep, *want;
};

static const struct test_case cases[] = {
    {"one-date", "Due 2024-01-02.", "-", "/", "Due 02/01/2024."},
    {"two-dates", "From 2024-01-02 to 2024-12-31", "-", "/", "From 02/01/2024 to 31/12/2024"},
    {"no-date", "no dates here", "-", "/", "no dates here"},
    {"empty-text", "", "-", "/", ""},
    {"empty-to-sep", "2024-01-02", "-", "", "02012024"},
    {"whole-text", "2024-01-02", "-", "/", "02/01/2024"},
    {"adjacent", "2024-01-02,2024-01-03", "-", "/", "02/01/2024,03/01/2024"},
    {"digit-before", "12024-01-02", "-", "/", "12024-01-02"},
    {"digit-after", "2024-01-023", "-", "/", "2024-01-023"},
    {"date-after-rejected", "2024-01-0234-05-06", "-", "/", "2024-01-06/05/0234"},
    {"chained", "2024-01-02-03", "-", "/", "02/01/2024-03"},
    {"other-separator", "2024/01/02", "-", ".", "2024/01/02"},
    {"dot-separator", "2024x01x02 2024.01.02", ".", "-", "2024x01x02 02-01-2024"},
    {"backslash-separator", "2024\\01\\02", "\\", "-", "02-01-2024"},
    {"plus-separator", "2024+01+02 202401102", "+", "-", "02-01-2024 202401102"},
    {"dollar-in-replacement", "2024-01-02", "-", "$1", "02$101$12024"},
    {"backslash-in-replacement", "2024-01-02", "-", "\\1\\", "02\\1\\01\\1\\2024"},
    {"digit-in-replacement", "2024-01-02", "-", "0", "0200102024"},
    {"whole-match-in-replacement", "2024-01-02", "-", "$&", "02$&01$&2024"},
    {"unicode-digits", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 2024-01-02", "-", "/", "٢٠٢٤-٠١-٠٢ and ２０２４-01-02 and 02/01/2024"},
    {"non-ascii-text", "Café ☕ 2024-01-02 🙂", "-", "/", "Café ☕ 02/01/2024 🙂"},
    {"non-ascii-separator", "2024–01–02", "–", "·", "02·01·2024"},
    {"multiline", "2024-01-02\n2024-01-03\n", "-", "/", "02/01/2024\n03/01/2024\n"},
    {"not-a-calendar-date", "2024-13-45", "-", "/", "45/13/2024"},
};

// Each date is written as "year-month-day", and the dates are joined by spaces.
struct find_case {
    const char *name, *text, *sep, *want;
};

static const struct find_case find_cases[] = {
    {"two-dates", "From 2024-01-02 to 2024-12-31", "-", "2024-01-02 2024-12-31"},
    {"none", "no dates here", "-", ""},
    {"empty", "", "-", ""},
    {"date-after-rejected", "2024-01-0234-05-06", "-", "0234-05-06"},
    {"dot-separator", "2024x01x02 2024.01.02", ".", "2024-01-02"},
    {"unicode-digits", "٢٠٢٤-٠١-٠٢ 2024-01-02", "-", "2024-01-02"},
};

int main(void) {
    int failed = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char *got = reformat_dates(c->text, c->from_sep, c->to_sep);
        if (got == nullptr || strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: got \"%s\"\n", c->name, got ? got : "(null)");
            failed++;
        }
        free(got);
    }
    for (size_t i = 0; i < sizeof find_cases / sizeof find_cases[0]; i++) {
        const struct find_case *c = &find_cases[i];
        struct date *dates = nullptr;
        size_t n = 0;
        char got[256] = "";
        if (find_dates(c->text, c->sep, &dates, &n) != 0) {
            fprintf(stderr, "FAIL %s: error\n", c->name);
            failed++;
            continue;
        }
        for (size_t k = 0; k < n; k++) {
            size_t used = strlen(got);
            snprintf(got + used, sizeof got - used, "%s%s-%s-%s", k ? " " : "", dates[k].year, dates[k].month,
                     dates[k].day);
        }
        if (strcmp(got, c->want) != 0) {
            fprintf(stderr, "FAIL %s: got \"%s\"\n", c->name, got);
            failed++;
        }
        free(dates);
    }
    if (failed > 0) {
        return 1;
    }
    printf("ok\n");
    return 0;
}
