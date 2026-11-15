```bash run id=bash-stream
dir=$(mktemp -d)
seq 0 999999 | awk '{ print $1 % 1000 }' > "$dir/numbers.txt"

# awk reads one line at a time, and keeps only the count, the sum and the maximum.
awk 'NR == 1 || $1 > max { max = $1 } { sum += $1 } END { print "lines:", NR, "sum:", sum, "max:", max }' "$dir/numbers.txt"

rm -rf "${dir:?}"
```

```output
lines: 1000000 sum: 499500000 max: 999
```

`awk` reads one record at a time, and a record is a line. The variables `max` and `sum` are the state, and the `END` action prints the result.
