```javascript run id=static-keyword
class Order {
  static TAX_RATE = 0.2; // a static field, on the class object
  static created = 0;

  constructor(amount) {
    Order.created++;
    this.id = Order.created;
    this.amount = amount;
  }

  static fromText(text) {
    return new this(Number(text)); // this is the class that the call used
  }

  total() {
    return this.amount * (1 + Order.TAX_RATE);
  }
}

class RushOrder extends Order {}

const a = new Order(100);
const b = RushOrder.fromText("50");
console.log(a.id, b.id, Order.created, b.constructor.name);
console.log(a.total(), a.created, RushOrder.TAX_RATE);
```

```output for=static-keyword
1 2 2 RushOrder
120 undefined 0.2
```

- Verdict: native. `static` puts a field or method on the class object, not on the prototype, so `a.created` is `undefined`.
- In a static method, `this` is the class that the call used, so `RushOrder.fromText` makes a `RushOrder`. This is close to a Python class method.
- Static members are inherited: `RushOrder.TAX_RATE` finds `Order.TAX_RATE` through the prototype chain of the class objects.
