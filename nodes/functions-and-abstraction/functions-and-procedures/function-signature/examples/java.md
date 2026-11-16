```java run id=function-signature-java
public class Main {
    static double average(int a, int b) {
        return (a + b) / 2.0;
    }

    public static void main(String[] args) {
        System.out.println(average(3, 4));
    }
}
```

```output for=function-signature-java
3.5
```

- Verdict: native. The result type comes before the name, and each parameter has a type. The JLS signature for overloading has the name and the parameter types.
