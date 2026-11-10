```typescript run id=shared-connection
class Connection {
  #users = 1;
  open = true;

  retain(): this {
    this.#users += 1;
    return this;
  }

  release(): void {
    this.#users -= 1;
    if (this.#users === 0) {
      this.open = false;
      console.log("connection closed");
    }
  }
}

async function work(conn: Connection, id: number): Promise<void> {
  await Promise.resolve();
  console.log(`request ${id} done`);
  conn.release();
}

const conn = new Connection();
await Promise.all([1, 2, 3].map((id) => work(conn.retain(), id)));
console.log("open after requests:", conn.open);
conn.release();
console.log("open after last release:", conn.open);
```

```output
request 1 done
request 2 done
request 3 done
open after requests: true
connection closed
open after last release: false
```

- Verdict: idiom. TypeScript compiles to JavaScript, and the engine frees memory with a tracing collector. A count for a shared resource is ordinary code.
- The types do not track the count. Nothing stops a call of `release` too many times, or a missing one.
- `retain` returns `this`, so the call site takes its reference in the same expression that passes the connection on.
