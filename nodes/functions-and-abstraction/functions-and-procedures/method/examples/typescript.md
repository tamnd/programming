```typescript run id=method-typescript
class Rectangle {
  width: number;
  height: number;

  constructor(width: number, height: number) {
    this.width = width;
    this.height = height;
  }

  area(): number {
    return this.width * this.height;
  }

  static square(side: number): Rectangle {
    return new Rectangle(side, side);
  }
}

const r = new Rectangle(3, 4);
console.log(r.area());
console.log(Rectangle.square(5).area());
```

```output for=method-typescript
12
25
```

- Verdict: native. Methods have the same type annotations as functions. The receiver is `this`.
