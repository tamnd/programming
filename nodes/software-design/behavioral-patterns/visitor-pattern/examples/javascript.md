```javascript run id=expr
class Num {
  constructor(value) {
    this.value = value;
  }
  accept(visitor) {
    return visitor.num(this);
  }
}

class Add {
  constructor(left, right) {
    this.left = left;
    this.right = right;
  }
  accept(visitor) {
    return visitor.add(this);
  }
}

class Mul {
  constructor(left, right) {
    this.left = left;
    this.right = right;
  }
  accept(visitor) {
    return visitor.mul(this);
  }
}

// Each visitor is one operation, with a method for each kind of node.
const show = {
  num: (n) => String(n.value),
  add: (n) => `(${n.left.accept(show)} + ${n.right.accept(show)})`,
  mul: (n) => `(${n.left.accept(show)} * ${n.right.accept(show)})`,
};

const evaluate = {
  num: (n) => n.value,
  add: (n) => n.left.accept(evaluate) + n.right.accept(evaluate),
  mul: (n) => n.left.accept(evaluate) * n.right.accept(evaluate),
};

const count = {
  num: () => 1,
  add: (n) => 1 + n.left.accept(count) + n.right.accept(count),
  mul: (n) => 1 + n.left.accept(count) + n.right.accept(count),
};

const tree = new Mul(new Add(new Num(2), new Num(3)), new Num(4));
console.log("show:", tree.accept(show));
console.log("evaluate:", tree.accept(evaluate));
console.log("nodes:", tree.accept(count));
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: idiom. This is the classic double dispatch. `accept` picks the method of the visitor by the class of the node, and the visitor picks the operation.
- A visitor is a plain object of functions, so an operation needs no class. A visitor that lacks a method throws a `TypeError` only when the program reaches a node of that kind.
- Without `accept`, a function can switch on `node.constructor` or on a `kind` field. The TypeScript example uses a `kind` field, so that the compiler can check the cases.
