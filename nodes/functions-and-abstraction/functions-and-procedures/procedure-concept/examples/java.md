```java run id=procedure-java
public class Main {
    static void greet(String name) {
        System.out.println("Hello, " + name + "!");
    }

    public static void main(String[] args) {
        greet("Ada");
        greet("Alan");
    }
}
```

```output for=procedure-java
Hello, Ada!
Hello, Alan!
```

- Verdict: native. A method with the result type `void` is a procedure. The compiler rejects code that uses the result of its call.
