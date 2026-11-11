```bash run id=awk
printf '%s\n' "pen 3 2" "book 1 12" "ink 2 5" "pen 4 2" |
    awk '$2 > 1 { total[$1] += $2 * $3 }
         END    { for (k in total) print k, total[k] }' |
    sort
```

```output for=awk
ink 10
pen 14
```

- Verdict: idiom. An awk program is a list of patterns and actions: for each line that matches the pattern, awk runs the action. The program does not write the loop over the lines. Tools such as `find`, `make` and `sort` also take a description of what to do.
