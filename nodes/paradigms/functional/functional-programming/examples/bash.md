```bash run id=pipes
printf '%s\n' map filter reduce fold zip |
    awk 'length($0) > 3' |
    tr 'a-z' 'A-Z' |
    sort
```

```output for=pipes
FILTER
FOLD
REDUCE
```

- Verdict: idiom. A pipeline composes programs that read input and write output, much as function composition joins functions. Each stage is a filter or a map over lines. The shell has no functions as values and no closures.
