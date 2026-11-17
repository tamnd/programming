```bash run id=trace
where() {
  # FUNCNAME holds the functions on the call stack, innermost first; skip where.
  local names=("${FUNCNAME[@]:1}")
  local IFS=,
  echo "${names[*]}" | sed 's/,/ <- /g'
}

c() {
  echo "      stack: $(where)"
}

b() {
  echo "    call c"
  c
  echo "    back in b"
}

a() {
  echo "  call b"
  b
  echo "  back in a"
}

echo "call a"
a
echo "back in main"
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. Bash keeps the names of the active functions in the `FUNCNAME` array. The last element is `main`, the code of the script outside any function.
- The `caller` builtin shows the call stack from the same data.
