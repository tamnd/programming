```java run id=counter-module
final class Counter {
    private static int privateCounter = 0;

    private Counter() {} // no instances: the class is the module

    private static void changeBy(int val) {
        privateCounter += val;
    }

    static void increment() { changeBy(1); }
    static void decrement() { changeBy(-1); }
    static int value() { return privateCounter; }
}

public class Main {
    public static void main(String[] args) {
        System.out.println("value: " + Counter.value());
        Counter.increment();
        Counter.increment();
        System.out.println("value: " + Counter.value());
        Counter.decrement();
        System.out.println("value: " + Counter.value());
    }
}
```

```output
value: 0
value: 2
value: 1
```

- Verdict: idiom. A final class with a private constructor and only static members stands for a module. Packages and the Java module system control access at a larger scale.
