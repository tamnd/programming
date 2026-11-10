```typescript run id=shop
class Inventory {
  private readonly stock: Map<string, number>;
  constructor(stock: Record<string, number>) {
    this.stock = new Map(Object.entries(stock));
  }
  reserve(item: string): boolean {
    const n = this.stock.get(item) ?? 0;
    if (n === 0) return false;
    this.stock.set(item, n - 1);
    return true;
  }
  release(item: string): void {
    this.stock.set(item, (this.stock.get(item) ?? 0) + 1);
  }
  count(item: string): number {
    return this.stock.get(item) ?? 0;
  }
}

class Payments {
  charge(card: string, _amount: number): boolean {
    return card !== "expired";
  }
}

class Shipping {
  private parcels = 0;
  schedule(_item: string): number {
    return ++this.parcels;
  }
}

// The interface that clients see. It names no subsystem class.
interface Shop {
  placeOrder(item: string, card: string, price: number): string;
  stock(item: string): number;
}

class ShopFacade implements Shop {
  private readonly inventory: Inventory;
  private readonly payments = new Payments();
  private readonly shipping = new Shipping();

  constructor(stock: Record<string, number>) {
    this.inventory = new Inventory(stock);
  }

  placeOrder(item: string, card: string, price: number): string {
    if (!this.inventory.reserve(item)) return `failed: ${item} is out of stock`;
    if (!this.payments.charge(card, price)) {
      this.inventory.release(item);
      return `failed: card declined, ${item} back in stock`;
    }
    return `ok: ${item}, paid ${price.toFixed(2)}, parcel ${this.shipping.schedule(item)}`;
  }

  stock(item: string): number {
    return this.inventory.count(item);
  }
}

const shop: Shop = new ShopFacade({ book: 2, lamp: 0 });
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

- Verdict: idiom. The interface `Shop` is the facade's contract, and `ShopFacade` implements it. Clients depend on `Shop` and import nothing from the subsystem.
- Design Patterns suggests an abstract facade for this reason: a client that holds the interface can work with another implementation of the subsystem.
- `private` is checked only by `tsc`. At run time the fields are normal properties, so the JavaScript example uses closures to hide them.
