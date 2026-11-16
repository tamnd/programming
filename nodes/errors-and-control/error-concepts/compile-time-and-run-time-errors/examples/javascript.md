```javascript run id=when-javascript
const items = [10, 20, 30];
for (const i of [1, 5]) {
  console.log(`items[${i}] = ${items[i]}`);
}
```

```output for=when-javascript
items[1] = 20
items[5] = undefined
```

- Verdict: absent. JavaScript finds a syntax error when it parses the script, and throws a `TypeError` at run time for an operation on a value of the wrong type. An index past the end is not an error: the result is `undefined`, and the program goes on.
