```java run id=overloading-java
public class Main {
    static String show(int x) {
        return "int " + x;
    }

    static String show(String s) {
        return "string " + s;
    }

    static String show(int x, int y) {
        return "two ints " + x + " " + y;
    }

    public static void main(String[] args) {
        System.out.println(show(42));
        System.out.println(show("hi"));
        System.out.println(show(3, 4));
    }
}
```

```output for=overloading-java
int 42
string hi
two ints 3 4
```

- Verdict: native. The three methods have the same name and different parameter lists. The compiler chooses one at each call.
