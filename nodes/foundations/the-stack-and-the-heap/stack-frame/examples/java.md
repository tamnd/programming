```java run id=frames
public class Main {
    static void count(int n) {
        int here = n * 10; // a local: one slot in the frame of each call
        System.out.println("enter n=" + n + " here=" + here);
        if (n > 1) {
            count(n - 1);
        }
        System.out.println("leave n=" + n + " here=" + here);
    }

    public static void main(String[] args) {
        count(3);
    }
}
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call gets a JVM frame. `n` and `here` are slots in the local variable array of that frame.
- The program cannot read the locals of a frame. `StackWalker` gives only the method and the line of each frame.
