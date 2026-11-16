```bash run id=fall-through-bash
rights() {
  local out=""
  case $1 in
    3) out+=" delete" ;&
    2) out+=" write" ;&
    1) out+=" read" ;;
    *) out+=" none" ;;
  esac
  echo "$1:$out"
}

rights 3
rights 2
rights 0

kind() {
  case $1 in
    [0-9]*) printf 'starts with a digit' ;;&
    *0) printf ', ends with 0' ;;&
    *) printf '\n' ;;
  esac
}

kind 10
kind 17
```

```output for=fall-through-bash
3: delete write read
2: write read
0: none
starts with a digit, ends with 0
starts with a digit
```

- Verdict: native. A clause that ends with `;;` does not fall through. A clause that ends with `;&` runs the commands of the next clause. A clause that ends with `;;&` tests the patterns of the next clauses, and runs each clause that matches.
