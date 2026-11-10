```typescript run id=strict
type User = { name: string; email: string | null };

function domain(user: User): string {
  if (user.email === null) {
    return "none";
  }
  return user.email.split("@")[1];
}

const users: User[] = [
  { name: "Ada", email: "ada@example.org" },
  { name: "Bob", email: null },
];

for (const u of users) {
  console.log(u.name, domain(u));
}

const found = users.find((u) => u.name === "Eve");
console.log(found?.name ?? "not found");
```

```output
Ada example.org
Bob none
not found
```

- Verdict: native. Under `strictNullChecks`, `null` and `undefined` have their own types. `string` does not include them, so a value that can be absent has a union type such as `string | null`.
- The type checker narrows the type. After the test `user.email === null` returns, `user.email` has the type `string`. Without the test, the checker reports an error at `user.email.split`.
- `Array.prototype.find` returns `User | undefined`, so the result needs `?.` or a test.
- The checks exist only at compile time. The runner strips the types and does not run the checker. With `strictNullChecks` off, `null` and `undefined` belong to every type and the checker reports nothing.
- The postfix `!` operator removes `null` and `undefined` from a type without a check at run time.
