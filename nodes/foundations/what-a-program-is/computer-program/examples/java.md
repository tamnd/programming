```java run id=gcd-in-a-class
public class Main {
    static long gcd(long a, long b) {
        while (b != 0) {
            long r = a % b;
            a = b;
            b = r;
            System.out.println(a + " " + b);
        }
        return a;
    }

    public static void main(String[] args) {
        System.out.println(1071 + " " + 462);
        System.out.println("gcd " + gcd(1071, 462));
        System.exit(0);
    }
}
```

```output for=gcd-in-a-class
1071 462
462 147
147 21
21 0
gcd 21
```

- Verdict: native. A Java program is a set of classes, and the Java launcher starts it by calling the `public static void main(String[] args)` method of the class it names. The compiler turns each class into bytecode for the Java Virtual Machine. Java has no multiple assignment, so the loop keeps the remainder in a local variable before it changes `a`. `System.exit` sets the exit status.
