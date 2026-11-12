```java run id=public-static-void-main
public class Main {
    public static void main(String[] args) {
        System.out.println("Hello, world!");
        System.out.printf("%s has %d letters%n", "world", 5);
        System.out.println("arguments: " + args.length);
    }
}
```

```output for=public-static-void-main
Hello, world!
world has 5 letters
arguments: 0
```

- Verdict: native. A Java program starts at a `main` method with a `String[]` parameter for the arguments. In the classic form it is `public static` and lives in a class, and the class name usually matches the file name. `System.out.println` writes a line, and `printf` formats, with `%n` for the newline of the platform. The `java` launcher compiles and runs a single source file in one step, as the example does.
- Newer versions of Java also accept a compact source file with a plain `void main()` and no class declaration, which makes the first program shorter.
