```typescript run id=register
interface Users {
  exists(email: string): boolean;
  add(email: string): void;
}

interface Mailer {
  send(to: string, body: string): void;
}

function register(email: string, users: Users, mailer: Mailer): string {
  if (users.exists(email)) {
    return "taken";
  }
  users.add(email);
  mailer.send(email, "Welcome!");
  return "created";
}

// A fake: a class that implements Users with a Set.
class FakeUsers implements Users {
  private emails = new Set<string>();
  exists(email: string): boolean {
    return this.emails.has(email);
  }
  add(email: string): void {
    this.emails.add(email);
  }
}

// A spy: an object literal with the shape of Mailer, and no class at all.
const sent: Array<[string, string]> = [];
const spyMailer: Mailer = {
  send(to, body) {
    sent.push([to, body]);
  },
};

const users = new FakeUsers();
console.log(register("ada@example.com", users, spyMailer));
console.log(register("ada@example.com", users, spyMailer));
console.log(sent);
```

```output
created
taken
[ [ 'ada@example.com', 'Welcome!' ] ]
```

- Verdict: stdlib. TypeScript runs on Node.js, so it has the same `node:test` mocks as JavaScript. This example writes its doubles by hand.
- TypeScript checks types by structure. The object literal `spyMailer` has no class, but it has the shape of `Mailer`, so the compiler accepts it. The parameters `to` and `body` get their types from the interface.
- `class FakeUsers implements Users` asks the compiler to check that the fake has every method of the interface. When the interface gains a method, the fake fails to compile until it has the method too.
- The types are erased before the program runs. A double built at run time, such as a `Proxy`, is checked only by the type that the test gives it.
