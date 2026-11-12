```javascript run id=around
function withConnection(name, block) {
  console.log("open", name);
  try {
    return block(name); // the caller's work
  } finally {
    console.log("close", name); // runs even if block throws
  }
}

const result = withConnection("db", (conn) => {
  const rows = ["a", "b", "c"];
  console.log(`query: ${rows.length} rows`);
  return rows.length;
});
console.log("result:", result);
```

```output for=around
open db
query: 3 rows
close db
result: 3
```

- Verdict: idiom. A function takes the block as a callback and closes the resource in `finally`.
