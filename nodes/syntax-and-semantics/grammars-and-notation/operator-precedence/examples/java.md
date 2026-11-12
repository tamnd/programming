```java run id=concatenation-and-shifts
public class Main {
    public static void main(String[] args) {
        // + groups to the left: number addition first, then string concatenation.
        System.out.println(1 + 2 + "3");
        System.out.println("1" + 2 + 3);
        System.out.println("1" + (2 + 3));

        // Shift binds less tightly than +, so this is 1 << 3.
        System.out.println(1 << 2 + 1);

        // == binds more tightly than &, so x & 1 == 0 would be x & (1 == 0), which does not compile for an int x.
        int x = 6;
        System.out.println((x & 1) == 0);

        // The conditional operator groups to the right.
        int n = 15;
        String s = n % 15 == 0 ? "FizzBuzz" : n % 3 == 0 ? "Fizz" : n % 5 == 0 ? "Buzz" : String.valueOf(n);
        System.out.println(s);

        // Assignment groups to the right, and a compound assignment casts its result.
        int a, b;
        a = b = 7;
        byte c = 10;
        c += 300; // c = (byte) (c + 300)
        System.out.println(a + " " + b + " " + c);
    }
}
```

```output for=concatenation-and-shifts
33
123
15
8
true
FizzBuzz
7 7 54
```

- Verdict: native. `+` groups to the left, so the position of a string changes whether numbers are added or joined. `+` binds more tightly than `<<`. `==` binds more tightly than `&`, so the mask needs parentheses, and without them the code does not compile for an `int`. The conditional operator and assignment group to the right, and a compound assignment such as `c += 300` includes a cast to the type of `c`.
