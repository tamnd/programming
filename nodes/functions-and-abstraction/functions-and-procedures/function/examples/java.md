```java run id=function-java
public class Main {
    static int rectangleArea(int width, int height) {
        return width * height;
    }

    static String greeting(String name) {
        return "Hello, " + name + "!";
    }

    public static void main(String[] args) {
        System.out.println(rectangleArea(3, 4));
        System.out.println(greeting("Ada"));
    }
}
```

```output for=function-java
12
Hello, Ada!
```

- Verdict: native. Java has no functions outside a class, so each function is a `static` method of the class `Main`.
