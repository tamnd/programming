```typescript run id=interface-merging
interface Box {
  height: number;
  width: number;
}
interface Box {
  scale: number; // merged with the first declaration
}

class Order {
  count: number;
  price: number;
  constructor(count: number, price: number) {
    this.count = count;
    this.price = price;
  }
}
interface Order {
  total(): number; // adds a member to the type of the class
}
Order.prototype.total = function (this: Order) {
  return this.count * this.price;
};

const box: Box = { height: 5, width: 6, scale: 10 };
const o = new Order(3, 40);
console.log(Object.keys(box).length, o.total());
```

```output for=interface-merging
3 120
```

- Verdict: absent. Two `interface Box` declarations merge into one type, but two classes with the same name do not compile.
- `interface Order` adds `total` to the type of the class `Order`, so the compiler accepts `o.total()`. The method itself must still go on `Order.prototype` at run time; the interface only tells the compiler about it.
