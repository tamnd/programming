```javascript run id=shared-connection
class Connection {
  #users = 1;
  open = true;

  retain() {
    this.#users += 1;
    return this;
  }

  release() {
    this.#users -= 1;
    if (this.#users === 0) {
      this.open = false;
      console.log("connection closed");
    }
  }
}

async function work(conn, id) {
  await Promise.resolve();
  console.log(`request ${id} done`);
  conn.release();
}

const conn = new Connection();
const requests = [1, 2, 3].map((id) => work(conn.retain(), id));
await Promise.all(requests);
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

- Verdict: idiom. JavaScript engines free memory with a tracing collector, and the language has no reference counts. A program can count the users of a resource itself.
- This program runs on one thread, so a plain number is enough for the count.
- Each request takes its reference before it starts to wait. The last `release` closes the connection, whatever the order in which the requests end.
- A `FinalizationRegistry` callback is not a replacement, because the collector decides when it runs.
