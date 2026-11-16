```bash run id=function-signature-bash
sum() {
  echo $(( $1 + $2 ))
}

sum 3 4
```

```output for=function-signature-bash
7
```

- Verdict: absent. A Bash function has no signature: the parentheses in the definition are always empty, and the body reads its arguments as `$1` and `$2`. Bash arithmetic has only integers, so this example adds the two numbers.
