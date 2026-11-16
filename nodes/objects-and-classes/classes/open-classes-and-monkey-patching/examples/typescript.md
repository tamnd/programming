```typescript run id=prototype-method-with-merged-interface
class Money {
  cents: number;
  constructor(cents: number) {
    this.cents = cents;
  }
}

// An interface with the same name merges with the class, so the compiler knows the new method.
interface Money {
  format(): string;
}

Money.prototype.format = function (this: Money): string {
  return `$${(this.cents / 100).toFixed(2)}`;
};

const prices = [new Money(1234), new Money(5)];
console.log(prices.map((p) => p.format()).join(" "));
```

```output for=prototype-method-with-merged-interface
$12.34 $0.05
```

The run-time code is the JavaScript code: an assignment to `Money.prototype.format`. TypeScript needs one more step, because the compiler does not know about the new method. An interface with the same name as the class merges with it, and the handbook uses the same idea for module augmentation: "Although JavaScript modules do not support merging, you can patch existing objects by importing and then updating them." For a class that another module exports, put the interface in a `declare module "<path>"` block.
