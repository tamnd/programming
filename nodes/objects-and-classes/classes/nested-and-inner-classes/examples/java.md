```java run id=outer-this-and-local-class
import java.util.function.IntSupplier;

public class Main {
  static class Counter {
    private int value = 10;

    class Step {
      private int value = 1; // hides Counter.value inside Step

      void apply() {
        Counter.this.value += value; // Counter.this names the outer instance
      }
    }

    IntSupplier reader(int scale) {
      // A local class: declared in a method; it can use the effectively final parameter scale.
      class Reader implements IntSupplier {
        public int getAsInt() {
          return value * scale;
        }
      }
      return new Reader();
    }
  }

  public static void main(String[] args) {
    Counter c = new Counter();
    Counter.Step step = c.new Step();
    step.apply();
    step.apply();
    IntSupplier r = c.reader(3);
    System.out.println(c.value + " " + r.getAsInt());
    IntSupplier lambda = () -> c.value * 2; // a lambda replaces most one-method anonymous classes
    System.out.println(lambda.getAsInt());
  }
}
```

```output for=outer-this-and-local-class
12 36
24
```

- Verdict: native. Java has all four kinds: static nested classes, inner member classes, local classes and anonymous classes.
- In `Step`, the name `value` means the field of `Step`. `Counter.this.value` reaches the field of the outer object, which the inner object holds a hidden reference to.
- A local class or a lambda can use a local variable or a parameter of the method only if it is final or effectively final: it gets a copy of the value, not the variable.
