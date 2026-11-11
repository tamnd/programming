```bash run id=wait
work() {
  sleep 0.1
  echo "$1" > "result-$1.txt"
  return "$2"
}

work a 0 &
first=$!
work b 3 &
second=$!

wait "$first"
echo "a exited with $? and wrote $(cat result-a.txt)"
wait "$second"
echo "b exited with $?"
```

```output for=wait
a exited with 0 and wrote a
b exited with 3
```

- Verdict: idiom. `&` starts a command in the background, and `$!` is its process ID. `wait` with the ID waits for that process and returns its exit status. The process ID works as a future for the exit status.
- A background process cannot set variables in the shell, so the result goes through a file or a pipe.
