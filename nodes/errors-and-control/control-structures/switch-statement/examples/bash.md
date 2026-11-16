```bash run id=case-bash
kind() {
  case $1 in
    *.jpg | *.png | *.gif)
      echo "image"
      ;;
    *.txt | *.md)
      echo "text"
      ;;
    "")
      echo "no name"
      ;;
    *)
      echo "other"
      ;;
  esac
}

for name in photo.png notes.md run.sh ""; do
  echo "'$name' $(kind "$name")"
done
```

```output for=case-bash
'photo.png' image
'notes.md' text
'run.sh' other
'' no name
```

- Verdict: native. A Bash `case` matches a word against glob patterns, and `|` joins several patterns. Each clause ends with `;;`, so `case` stops after the first match. The last pattern, `*`, matches each word, so it does the job of `default`.
