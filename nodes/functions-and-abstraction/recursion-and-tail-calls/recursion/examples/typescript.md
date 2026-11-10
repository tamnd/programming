```typescript run id=tree
type Tree = number | Tree[];

function total(item: Tree): number {
  if (typeof item === "number") return item;
  return item.reduce((sum: number, child: Tree) => sum + total(child), 0);
}

type Json = null | boolean | number | string | Json[] | { [key: string]: Json };

function countLeaves(value: Json): number {
  if (Array.isArray(value)) return value.reduce((n: number, v: Json) => n + countLeaves(v), 0);
  if (value !== null && typeof value === "object") {
    return Object.values(value).reduce((n: number, v: Json) => n + countLeaves(v), 0);
  }
  return 1;
}

const tree: Tree = [1, [2, 3], [[4], 5]];
console.log(total(tree));
console.log(countLeaves({ name: "x", tags: ["a", "b"], size: { w: 1, h: 2 } }));
```

```output for=tree
15
5
```

- Verdict: native. Recursive functions work as in JavaScript, and the run time behaviour is the same.
- A type alias can refer to itself. `Tree` is a number or an array of trees, and `Json` describes any JSON value. The recursive function follows the shape of the recursive type.
- The type checker narrows `item` to `number` after the `typeof` test, so each branch of the function handles one case of the type.
