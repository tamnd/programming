```java run id=fixed-operators-and-compound-assignment
public class Main {
    public static void main(String[] args) {
        // + adds numbers and joins strings; Java has no other overloading of operators.
        System.out.println(1 + 2 + "x" + 1 + 2);

        // Integer / truncates toward zero, and % has the sign of the left operand.
        System.out.println((-7 / 2) + " " + (-7 % 2));

        // >> keeps the sign, and >>> shifts in zeros.
        int n = -16;
        System.out.println((n >> 2) + " " + (n >>> 28));

        // A compound assignment casts back to the type of the left side.
        byte b = 120;
        b += 10;  // b = (byte) (b + 10)
        System.out.println("b = " + b);

        // == compares references for objects; equals compares values.
        String s1 = new String("hi");
        String s2 = new String("hi");
        System.out.println((s1 == s2) + " " + s1.equals(s2));

        // instanceof with a pattern tests and binds in one operator.
        Object o = "text";
        if (o instanceof String s && s.length() > 2) {
            System.out.println("length " + s.length());
        }

        // The conditional operator ?: is the only ternary operator.
        int age = 20;
        System.out.println(age >= 18 ? "adult" : "minor");
    }
}
```

```output for=fixed-operators-and-compound-assignment
3x12
-3 -1
-4 15
b = -126
false true
length 4
adult
```

- Verdict: native. Java has a fixed set of operators, and `+` on a `String` is the only overloading. `+` groups from the left, so `1 + 2` is added before the strings are joined. Integer `/` truncates toward zero. `>>>` shifts in zeros. A compound assignment casts back to `byte`, so `b` wraps to `-126`. `==` compares references.
