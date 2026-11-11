```bash run id=functions
split_words() {
    tr 'A-Z' 'a-z' <<< "$1" | tr ' ' '\n'
}

count_words() {
    sort | uniq -c | sort -k1,1nr -k2,2
}

report() {
    local text=$1 n=$2
    split_words "$text" | count_words | head -n "$n" |
        while read -r c w; do printf '%-5s %d\n' "$w" "$c"; done
}

report "The cat and the hat and the bat" 2
```

```output for=functions
the   3
and   2
```

- Verdict: native. Shell functions take arguments as `$1`, `$2` and so on, and return text on standard output and a status code. Variables are global unless the function declares them `local`.
