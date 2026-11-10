```java run id=numeric
public class Main {
    public static void main(String[] args) {
        int code = 'A' + 1;
        long wide = code;
        double half = 7 / 2;
        System.out.println(code + " " + wide + " " + half + " " + 7 / 2.0);
        System.out.println((int) 3.99 + " " + (int) -3.99);
        System.out.println((byte) 200 + " " + (short) 70000);
        System.out.println((int) 3.0e10 + " " + (int) Double.NaN);
        System.out.println((float) 16_777_217);
        System.out.println(Integer.parseInt("-42") + 1);
        Object value = "text";
        System.out.println(value instanceof Integer n ? n + 1 : "not an Integer");
    }
}
```

```output for=numeric
66 66 3.0 3.5
3 -3
-56 4464
2147483647 0
1.6777216E7
-41
not an Integer
```

- Verdict: native. Widening primitive conversions are implicit. Narrowing conversions need a cast, except for an `int` constant that fits the target.
- A narrowing integer cast keeps the low bits, so `(byte) 200` is -56. A cast from `double` to `int` truncates toward zero, maps NaN to 0 and saturates a value out of range.
- `7 / 2` is integer division. The conversion to `double` happens after it, so `half` is 3.0.
- A cast of a reference is checked at run time and can throw `ClassCastException`. The `instanceof` pattern tests and converts in one step.
