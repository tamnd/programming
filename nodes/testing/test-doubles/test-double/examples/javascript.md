```javascript test id=register
import { test, mock } from "node:test";
import assert from "node:assert/strict";

function register(email, users, mailer) {
  if (users.exists(email)) {
    return "taken";
  }
  users.add(email);
  mailer.send(email, "Welcome!");
  return "created";
}

class FakeUsers {
  #emails = new Set();
  exists(email) {
    return this.#emails.has(email);
  }
  add(email) {
    this.#emails.add(email);
  }
}

test("an unused email is stored and welcomed", () => {
  const users = new FakeUsers();
  const mailer = { send: mock.fn() };
  assert.equal(register("ada@example.com", users, mailer), "created");
  assert.ok(users.exists("ada@example.com"));
  assert.equal(mailer.send.mock.callCount(), 1);
  assert.deepEqual(mailer.send.mock.calls[0].arguments, ["ada@example.com", "Welcome!"]);
});

test("a taken email gets no mail", () => {
  const users = new FakeUsers();
  users.add("ada@example.com");
  const mailer = { send: mock.fn() };
  assert.equal(register("ada@example.com", users, mailer), "taken");
  assert.equal(mailer.send.mock.callCount(), 0);
});
```

```output
✔ an unused email is stored and welcomed ($TIME)
✔ a taken email gets no mail ($TIME)
ℹ tests 2
ℹ suites 0
ℹ pass 2
ℹ fail 0
ℹ cancelled 0
ℹ skipped 0
ℹ todo 0
ℹ duration_ms $TIME
```

- Verdict: stdlib. `node:test` has a `mock` object. `mock.fn()` makes a spy function, and `mock.method()` replaces a method of an object for the length of a test.
- The spy records each call in `mock.calls`, with its `arguments`, its `result` and its `this` value. `mock.callCount()` gives the number of calls.
- Any object with the right methods works as a double, because JavaScript checks no types. The fake store keeps its data in a private field `#emails`.
- `node:test` also has `mock.timers`, a fake for `setTimeout`, `setInterval` and `Date`, so a test can move the clock without waiting.
