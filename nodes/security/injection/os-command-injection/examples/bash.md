```bash wrong id=eval
look_up() {
  eval "echo looking up $1"
}

look_up "report.txt"
look_up "report.txt; echo INJECTED"
```

```output for=eval
looking up report.txt
looking up report.txt
INJECTED
```

```bash run id=quoted
look_up() {
  echo "looking up $1"
}

look_up "report.txt"
look_up "report.txt; echo INJECTED"

name='report.txt; echo INJECTED'
printf '%s\n' "$name"
bash -c 'echo "looking up $1"' look-up "$name"
```

```output for=quoted
looking up report.txt
looking up report.txt; echo INJECTED
report.txt; echo INJECTED
looking up report.txt; echo INJECTED
```

- Verdict: native. A shell script parses each line once. The value of a variable does not go through the parse again, so `;` in a variable is a character, not a separator. The weakness comes when the script makes the shell parse the value a second time: with `eval`, with `sh -c "..."` that contains the value, or with a command line written to a file and run later.
- To pass a value into `bash -c`, give it as a separate argument and read it as `$1`, as in the last line of the second example. The command text stays fixed.
- An unquoted `$name` does not run a second command, but the shell splits it at spaces and expands glob characters, so one value can become many arguments. ShellCheck rule SC2086 reports unquoted variables. Write `"$name"`.
- Put `--` before values that could start with `-`, for commands that support it: `rm -- "$file"`.
