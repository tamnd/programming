```typescript run id=kinds
type Discount = (amount: number) => number;

const sum = (prices: number[]): number => prices.reduce((a, b) => a + b, 0);

abstract class Checkout {
  total(prices: number[]): number {
    return this.discount(sum(prices));
  }
  protected abstract discount(amount: number): number;
}

class SaleCheckout extends Checkout {
  protected discount(amount: number): number {
    return Math.floor((amount * 9) / 10);
  }
}

class Till {
  discount: Discount;
  constructor(discount: Discount) {
    this.discount = discount;
  }
  total(prices: number[]): number {
    return this.discount(sum(prices));
  }
}

const prices = [40, 60];
console.log("class pattern:", new SaleCheckout().total(prices));
const till = new Till((amount) => Math.floor((amount * 9) / 10));
console.log("object pattern:", till.total(prices));
till.discount = (amount) => amount - 20;
console.log("changed at run time:", till.total(prices));
```

```output
class pattern: 90
object pattern: 90
changed at run time: 80
```

- Verdict: native. The type `Discount` is a function type, so the compiler checks every strategy that is given to the till. The step of the class pattern is an abstract method.
