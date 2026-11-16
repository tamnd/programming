```bash run id=infinite
naturals_from() {
  local n=$1
  while true; do
    echo "$n"
    n=$((n + 1))
  done
}

keep_primes() {
  local n d prime
  while read -r n; do
    prime=1
    for ((d = 2; d * d <= n; d++)); do
      if ((n % d == 0)); then
        prime=0
        break
      fi
    done
    if ((prime)); then
      echo "$n"
    fi
  done
}

naturals_from 2 | keep_primes | head -n 10 | paste -sd ' '
yes ab | head -n 3 | paste -sd ' '
```

```output for=infinite
2 3 5 7 11 13 17 19 23 29
ab ab ab
```

- Verdict: idiom. A pipeline is lazy by flow control: each command runs as its own process, and a write blocks when the pipe is full. `naturals_from` and `yes` never end on their own.
- When `head` has its lines, it exits. The next write to its pipe fails with `SIGPIPE`, which stops `keep_primes`, and in turn `naturals_from`.
