```bash run id=lengths
word=$(printf 'caf\xc3\xa9')
printf '%s' "$word" | od -An -tx1 | head -n 1

LC_ALL=C
echo "C locale: ${#word}"
LC_ALL=en_US.UTF-8
echo "UTF-8 locale: ${#word}"

printf '%s' "$word" | wc -c | tr -d ' '
```

```output for=lengths
           63  61  66  c3  a9
C locale: 5
UTF-8 locale: 4
5
```

- Verdict: idiom. Bash passes bytes through, and it does not validate them.
- The expansion `${#word}` gives the length in characters. The variable `LC_CTYPE`, or `LC_ALL`, decides how Bash interprets characters. In the C locale each byte is a character, so "café" has five. In a UTF-8 locale it has four.
- `wc -c` counts bytes in each locale.
- The name of the UTF-8 locale differs between systems, such as `en_US.UTF-8` or `C.UTF-8`. The command `locale -a` lists the locales of the system.
