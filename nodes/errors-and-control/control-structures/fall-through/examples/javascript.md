```javascript run id=fall-through-javascript
function rights(level) {
  const out = [];
  switch (level) {
    case 3:
      out.push("delete");
    // falls through
    case 2:
      out.push("write");
    // falls through
    case 1:
      out.push("read");
      break;
    default:
      out.push("none");
  }
  return out.join(" ");
}

console.log(rights(3));
console.log(rights(2));
console.log(rights(9));
```

```output for=fall-through-javascript
delete write read
write read
none
```

- Verdict: native. A JavaScript case without `break` falls through into the next case, and into `default` too. A comment such as `// falls through` tells the reader that the fall-through is intentional.
