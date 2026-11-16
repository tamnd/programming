```python run id=locale-module
import locale

n = 1234567
x = 1234567.891

print(locale.setlocale(locale.LC_NUMERIC))
print(f"{n:n}", f"{n:,}", locale.format_string("%.2f", x, grouping=True))

locale.setlocale(locale.LC_NUMERIC, "en_US.UTF-8")
conv = locale.localeconv()
print(repr(conv["decimal_point"]), repr(conv["thousands_sep"]), conv["grouping"])
print(f"{n:n}", locale.format_string("%.2f", x, grouping=True))
print(locale.atof("1,234.5"))

locale.setlocale(locale.LC_NUMERIC, "C")
print(f"{n:n}", f"{x:.2f}")
```

```output for=locale-module
C
1234567 1,234,567 1234567.89
'.' ',' [3, 0]
1,234,567 1,234,567.89
1234.5
1234567 1234567.89
```

- Verdict: stdlib. The module `locale` sets a global locale from the POSIX database, and only `format_string`, `atof`, `atoi`, `str` and the format type `n` use it. The option `,` always gives commas. The program needs the locale `en_US.UTF-8` on the system. For the CLDR data of many locales without a global state, a program uses the library Babel.
