```javascript run id=tosorted-and-freeze
const scores = [58, 12, 90, 31];
const copy = scores.toSorted((a, b) => a - b); // ES2023: returns a new array
console.log("toSorted:", copy, "original:", scores);

const same = scores.sort((a, b) => a - b); // sorts in place and returns the same array
console.log("sort:", same, "same object:", same === scores);

const user = Object.freeze({ name: "Ada", tags: ["math"] });
user.name = "Alan"; // ignored outside strict mode
user.tags.push("poet"); // freeze is shallow
console.log(user);

let calls = 0;
const doubled = [1, 2, 3].map((x) => { calls++; return x * 2; });
console.log(doubled, "callback ran", calls, "times");
```

```output for=tosorted-and-freeze
toSorted: [ 12, 31, 58, 90 ] original: [ 58, 12, 90, 31 ]
sort: [ 12, 31, 58, 90 ] same object: true
{ name: 'Ada', tags: [ 'math', 'poet' ] }
[ 2, 4, 6 ] callback ran 3 times
```

- Verdict: native. JavaScript allows effects anywhere. ECMAScript 2023 added copying versions of the methods that change arrays: `toSorted`, `toReversed`, `toSpliced` and `with`. `Object.freeze` stops changes to the properties of one object, but it is shallow, so the array in `tags` still changes. Outside strict mode, a write to a frozen property fails with no error; in strict mode and in modules it throws a `TypeError`. A callback of `map` should only compute; `forEach` is the method for effects.
