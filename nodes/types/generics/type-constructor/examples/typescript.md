```typescript run id=alias-interface
// A generic type alias: Pair takes two types and gives a tuple type.
type Pair<A, B> = [A, B];

// A generic interface: Tree takes one type and refers to itself.
interface Tree<T> {
  value: T;
  children: Tree<T>[];
}

const p: Pair<string, number> = ["age", 42];
const t: Tree<string> = {
  value: "root",
  children: [{ value: "leaf", children: [] }],
};

// Built-in constructors: Array<T> is the same type as T[], and Map takes two types.
const counts: Map<string, Array<number>> = new Map([["a", [1, 2]]]);

console.log(p, t.children[0].value, counts.get("a"));
```

```output for=alias-interface
[ 'age', 42 ] leaf [ 1, 2 ]
```

- Verdict: native. `Pair` and `Tree` are type constructors that the program declares. `Pair<string, number>` and `Tree<string>` are the types made from them. `Array` and `Map` are built in. The types are erased when the program runs, so the output shows only the values.
