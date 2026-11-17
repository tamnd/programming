```bash run id=deep
# depth sets result to n, with one function call for each step.
depth() {
  local n=$1
  if (( n == 0 )); then
    result=0
    return
  fi
  depth $((n - 1))
  result=$((result + 1))
}

loop() {
  local n=$1 count=0
  while (( n > 0 )); do # one call for the whole loop
    n=$((n - 1))
    count=$((count + 1))
  done
  result=$count
}

depth 100
echo "depth(100) = $result"
if ( FUNCNEST=1000; depth 100000 ) 2>/dev/null; then
  echo "depth(100000) finished"
else
  echo "depth(100000): stopped by FUNCNEST=1000"
fi
loop 100000
echo "loop(100000) = $result"
```

```output for=deep
depth(100) = 100
depth(100000): stopped by FUNCNEST=1000
loop(100000) = 100000
```

- Verdict: native. With `FUNCNEST` set, a call past the nesting level aborts the command, and here the subshell ends with a failure. Without it, very deep recursion can crash Bash.
- The example uses 100000, not 1000000, because a loop in Bash is slow.
