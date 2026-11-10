```bash run id=countdown
items=(three two one)
for item in "${items[@]}"; do
  echo "$item"
done

printf '%s\n' 3 2 1 | while IFS= read -r line; do
  echo "line $line"
done
```

```output for=countdown
three
two
one
line 3
line 2
line 1
```

- Verdict: idiom. Bash has no iterator objects and no way to define one. The `for` loop expands its word list in full and then runs the body once for each word.
- To process a stream one item at a time, read lines in a `while read` loop. Each `read` takes the next line from the pipe.
- The quotes in `"${items[@]}"` keep each element as one word. Without them, an element with a space becomes two words.
- Each command of a pipeline runs in its own subshell, so variables that the `while` loop sets are lost when the loop ends. The `lastpipe` shell option, with job control off, lets the shell run the last command itself.
