```javascript run id=shop
class Inventory {
  #stock;
  constructor(stock) {
    this.#stock = new Map(Object.entries(stock));
  }
  reserve(item) {
    const n = this.#stock.get(item) ?? 0;
    if (n === 0) return false;
    this.#stock.set(item, n - 1);
    return true;
  }
  release(item) {
    this.#stock.set(item, this.#stock.get(item) + 1);
  }
  count(item) {
    return this.#stock.get(item) ?? 0;
  }
}

class Payments {
  charge(card, _amount) {
    return card !== "expired";
  }
}

class Shipping {
  #parcels = 0;
  schedule(_item) {
    return ++this.#parcels;
  }
}

// The facade: a function that makes the subsystem and returns only two operations.
function createShop(stock) {
  const inventory = new Inventory(stock);
  const payments = new Payments();
  const shipping = new Shipping();
  return {
    placeOrder(item, card, price) {
      if (!inventory.reserve(item)) return `failed: ${item} is out of stock`;
      if (!payments.charge(card, price)) {
        inventory.release(item);
        return `failed: card declined, ${item} back in stock`;
      }
      return `ok: ${item}, paid ${price.toFixed(2)}, parcel ${shipping.schedule(item)}`;
    },
    stock: (item) => inventory.count(item),
  };
}

const shop = createShop({ book: 2, lamp: 0 });
console.log(shop.placeOrder("book", "visa", 12.5));
console.log(shop.placeOrder("book", "expired", 12.5));
console.log(shop.placeOrder("lamp", "visa", 30));
console.log(`stock of book: ${shop.stock("book")}`);
```

```output
ok: book, paid 12.50, parcel 1
failed: card declined, book back in stock
failed: lamp is out of stock
stock of book: 1
```

- Verdict: idiom. A function makes the subsystem objects and returns an object with the operations of the facade. The subsystem objects are local variables, so a client cannot reach them through `shop`.
- In a program with ES modules, the module is the facade. It exports `placeOrder` and `stock` and does not export the classes.
- The facade is a plain object, not a class. A test can replace it with another object that has the same two methods.
