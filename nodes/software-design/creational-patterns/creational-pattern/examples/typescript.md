```typescript run id=language-features
interface Shape {
  area(): number;
}

class Circle implements Shape {
  r: number;
  constructor(r: number) {
    this.r = r;
  }
  area(): number {
    return Math.round(Math.PI * this.r ** 2 * 100) / 100;
  }
}

class Square implements Shape {
  s: number;
  constructor(s: number) {
    this.s = s;
  }
  area(): number {
    return this.s ** 2;
  }
}

// A constructor type lets a function create an object of the class it gets.
type ShapeClass = new (size: number) => Shape;
function create(Kind: ShapeClass, size: number): Shape {
  return new Kind(size);
}
console.log("factory:", create(Circle, 1).area(), create(Square, 2).area());

// Defaults and a Partial type do the work of a builder.
interface Options {
  method: string;
  timeout: number;
  retries: number;
}
const defaults: Options = { method: "GET", timeout: 30, retries: 0 };
function request(url: string, options: Partial<Options> = {}) {
  return { url, ...defaults, ...options };
}
console.log("builder:", JSON.stringify(request("https://example.com/", { retries: 2 })));

// structuredClone makes a deep copy of plain data: a prototype for data.
const template = { tags: ["a"], size: 1 };
const copy = structuredClone(template);
copy.tags.push("b");
console.log("prototype:", template.tags.join(","), "|", copy.tags.join(","));
```

```output
factory: 3.14 4
builder: {"url":"https://example.com/","method":"GET","timeout":30,"retries":2}
prototype: a | a,b
```

- Verdict: idiom. The forms are those of JavaScript, with types. A constructor type such as `new (size: number) => Shape` lets a factory take a class as an argument and still check it. `Partial<Options>` types an options object with defaults, which replaces most builders. `structuredClone` makes a deep copy of plain data.
