```bash run id=procedure-bash
greet() {
  echo "Hello, $1!"
}

greet "Ada"
greet "Alan"
echo "status $?"
```

```output for=procedure-bash
Hello, Ada!
Hello, Alan!
status 0
```

- Verdict: native. A Bash function gives back only an exit status, so a function that prints text or sets variables is a procedure.
