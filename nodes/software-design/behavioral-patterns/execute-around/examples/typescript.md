```typescript run id=around
function withConnection<T>(name: string, block: (conn: string) => T): T {
  console.log("open", name);
  try {
    return block(name); // the caller's work
  } finally {
    console.log("close", name); // runs even if block throws
  }
}

const result: number = withConnection("db", (conn) => {
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

- Verdict: idiom. A generic parameter `T` gives the type of the value that the block returns.
