```javascript run id=decompose
function splitWords(text) {
  return text.toLowerCase().split(/\s+/);
}

function countWords(words) {
  const counts = new Map();
  for (const w of words) counts.set(w, (counts.get(w) ?? 0) + 1);
  return counts;
}

function top(counts, n) {
  return [...counts].sort((a, b) => b[1] - a[1] || a[0].localeCompare(b[0])).slice(0, n);
}

function report(text, n) {
  for (const [word, c] of top(countWords(splitWords(text)), n)) {
    console.log(`${word.padEnd(5)} ${c}`);
  }
}

report("The cat and the hat and the bat", 2);
```

```output for=decompose
the   3
and   2
```

- Verdict: native. A `function` declaration is hoisted, so it can be called from code above it in the same scope. Modules with `import` and `export` group functions in files.
