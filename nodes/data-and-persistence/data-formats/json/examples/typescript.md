```typescript run id=parse-emit
interface Order {
  id: number;
  total: number;
  tags: string[];
}

function isOrder(value: unknown): value is Order {
  if (typeof value !== "object" || value === null) {
    return false;
  }
  const record = value as Record<string, unknown>;
  return (
    typeof record.id === "number" &&
    typeof record.total === "number" &&
    Array.isArray(record.tags) &&
    record.tags.every((tag) => typeof tag === "string")
  );
}

for (const text of ['{"id": 7, "total": 0.1, "tags": ["new"]}', '{"id": "7", "total": 0.1}']) {
  const data: unknown = JSON.parse(text);
  if (isOrder(data)) {
    console.log("order", data.id, JSON.stringify(data));
  } else {
    console.log("rejected", text);
  }
}
```

```output for=parse-emit
order 7 {"id":7,"total":0.1,"tags":["new"]}
rejected {"id": "7", "total": 0.1}
```

```typescript run id=round-trip
interface Meeting {
  title: string;
  at: Date;
}

const meeting: Meeting = { title: "review", at: new Date(Date.UTC(2026, 10, 10)) };
const copy = JSON.parse(JSON.stringify(meeting)) as Meeting;
console.log(typeof copy.at);
console.log(copy.at instanceof Date);

const nothing: string = JSON.stringify(undefined);
console.log(nothing === undefined);
```

```output for=round-trip
string
false
true
```

- Verdict: native. TypeScript uses the `JSON` object of JavaScript, with the same rules for numbers, `NaN`, `undefined` and repeated names.
- `JSON.parse` returns `any`. A type assertion such as `as Meeting` checks nothing at run time. In the second example the type says `Date`, but the value is a string.
- Assign the result to `unknown` and check it with a type guard, or with a schema validator, before the code uses it as a typed value.
- The declared return type of `JSON.stringify` is `string`, but the call returns `undefined` for `undefined`, a function or a symbol.
