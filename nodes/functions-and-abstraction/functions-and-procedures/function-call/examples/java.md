```java run id=function-call-java
public class Main {
    static int rectangleArea(int width, int height) {
        System.out.println("computing " + width + " by " + height);
        return width * height;
    }

    public static void main(String[] args) {
        int area = rectangleArea(3, 4);
        System.out.println("area " + area);
    }
}
```

```output for=function-call-java
computing 3 by 4
area 12
```

- Verdict: native. A call of a static method writes the arguments in parentheses after the method name.
