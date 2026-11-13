```javascript run id=iterable-protocol
// Any object with a [Symbol.iterator] method is a sequence for for...of, spread and Array.from.
const countdown = {
  from: 3,
  [Symbol.iterator]() {
    let n = this.from;
    return { next: () => (n > 0 ? { value: n--, done: false } : { value: undefined, done: true }) };
  },
};
console.log([...countdown], Array.from(countdown, (x) => x * 10));

// A generator makes a lazy sequence. This one never ends, so the program takes only a few items.
function* naturals() {
  for (let n = 0; ; n++) yield n;
}
const taken = [];
for (const n of naturals()) {
  if (n >= 4) break;
  taken.push(n);
}
console.log(taken);

// Arrays, strings, Maps and Sets are iterable. Plain objects are not.
console.log([..."hé!"], [...new Set([2, 2, 1])], typeof {}[Symbol.iterator]);

// next() gives one item at a time. A generator object is one pass: after it ends, it gives nothing more.
const once = naturals();
console.log(once.next().value, once.next().value);
function* pair() {
  yield 1;
  yield 2;
}
const g = pair();
console.log([...g], [...g], [...pair()]);
```

```output
[ 3, 2, 1 ] [ 30, 20, 10 ]
[ 0, 1, 2, 3 ]
[ 'h', 'é', '!' ] [ 2, 1 ] undefined
0 1
[ 1, 2 ] [] [ 1, 2 ]
```

- Verdict: native. An object with a Symbol.iterator method works with for...of, spread and Array.from. A generator makes a lazy sequence, and break stops an infinite one. Arrays, strings and Sets are iterable, and a plain object is not. A generator object is one pass, so a second spread gives an empty array.
