```typescript run id=erased
interface Shape {
  kind: string;
  area(): number;
}

class Square implements Shape {
  kind = "square";
  private side: number;
  constructor(side: number) {
    this.side = side;
  }
  area(): number {
    return this.side * this.side;
  }
}

const s: Shape = new Square(3);
console.log("constructor:", s.constructor.name);
console.log("keys:", Object.keys(s));
console.log("typeof area:", typeof s.area);
console.log("instanceof Square:", s instanceof Square);
```

```output for=erased
constructor: Square
keys: [ 'kind', 'side' ]
typeof area: function
instanceof Square: true
```

The interface `Shape` and the type annotations do not exist when the program runs. Reflection sees the JavaScript object: its class, its own keys and its prototype. The `private` field `side` is an ordinary property at run time, so `Object.keys` lists it.

- Verdict: absent. TypeScript erases its types, and type annotations never change the run-time behavior of a program. The reflection that a TypeScript program can use is the reflection of JavaScript.
- A test such as `s instanceof Square` works because `Square` is a class, which exists at run time. `s instanceof Shape` does not compile: the checker reports error TS2693, "'Shape' only refers to a type, but is being used as a value here".
