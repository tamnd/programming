```javascript run id=kinds
class Checkout {
  total(prices) {
    return this.discount(prices.reduce((a, b) => a + b, 0));
  }
  discount(amount) {
    return amount;
  }
}

class SaleCheckout extends Checkout {
  discount(amount) {
    return Math.floor((amount * 9) / 10);
  }
}

class Till {
  constructor(discount) {
    this.discount = discount;
  }
  total(prices) {
    return this.discount(prices.reduce((a, b) => a + b, 0));
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

- Verdict: native. Functions are values, so the strategy is an arrow function that the till stores in a field.
