```c run id=posix-regex
#include <regex.h>
#include <stdio.h>

int main(void)
{
    regex_t re;
    if (regcomp(&re, "([0-9]{4})-([0-9]{2})-([0-9]{2})", REG_EXTENDED) != 0)
        return 1;

    const char *p = "from 2024-01-31 to 2025-12-01";
    regmatch_t m[4];
    while (regexec(&re, p, 4, m, 0) == 0) {
        printf("%.*s\n", (int)(m[1].rm_eo - m[1].rm_so), p + m[1].rm_so);
        p += m[0].rm_eo;
    }
    regfree(&re);

    printf("[%-6s|%6.2f]\n", "pen", 3.14159);
    return 0;
}
```

```output for=posix-regex
2024
2025
[pen   |  3.14]
```

- Verdict: idiom. C is imperative, but it has small declarative languages inside its libraries: the POSIX regular expressions of `<regex.h>` and the format strings of `printf`, which state the layout of the output.
