```typescript run id=duration
class Duration {
  private static zero: Duration | null = null;

  private readonly seconds: number;

  private constructor(seconds: number) {
    this.seconds = seconds;
  }

  static ofSeconds(seconds: number): Duration {
    if (seconds === 0) {
      Duration.zero ??= new Duration(0);
      return Duration.zero; // the same instance every time
    }
    return new Duration(seconds);
  }

  static ofMinutes(minutes: number): Duration {
    return Duration.ofSeconds(minutes * 60);
  }

  static parse(text: string): Duration | null {
    const m = /^(\d+)([sm])$/.exec(text);
    if (m === null) return null; // a constructor cannot return "no object"
    return Duration.ofSeconds(Number(m[1]) * (m[2] === "m" ? 60 : 1));
  }

  toString(): string {
    return `${this.seconds} s`;
  }
}

console.log(`ofMinutes(2) = ${Duration.ofMinutes(2)}`);
console.log(`ofSeconds(90) = ${Duration.ofSeconds(90)}`);
console.log(`zero is cached: ${Duration.ofSeconds(0) === Duration.ofMinutes(0)}`);
console.log(`parse("2m") = ${Duration.parse("2m")}`);
console.log(`parse("2h") = ${Duration.parse("2h")}`);
// new Duration(5) is a compile error: the constructor is private.
```

```output
ofMinutes(2) = 120 s
ofSeconds(90) = 90 s
zero is cached: true
parse("2m") = 120 s
parse("2h") = null
```

- Verdict: idiom. TypeScript allows a `private constructor`, so the compiler rejects `new Duration(5)` outside the class. Only the static methods can build an instance.
- The check is at compile time only. The JavaScript that the compiler emits has a normal constructor.
