```java fails expect=exception:ArrayIndexOutOfBoundsException id=index
public class Main {
    static void setScore(int[] scores, int player, int score) {
        scores[player] = score;
    }

    public static void main(String[] args) {
        int[] scores = new int[4];
        setScore(scores, 2, 10);
        System.out.println("player 2: " + scores[2]);
        setScore(scores, 4, 99);
    }
}
```

```output for=index stream=stderr
Exception in thread "main" java.lang.ArrayIndexOutOfBoundsException: Index 4 out of bounds for length 4
	at Main.setScore(Main.java:3)
	at Main.main(Main.java:10)
```

```java run id=check
public class Main {
    static boolean setScore(int[] scores, int player, int score) {
        if (player < 0 || player >= scores.length) {
            return false;
        }
        scores[player] = score;
        return true;
    }

    public static void main(String[] args) {
        int[] scores = new int[4];
        setScore(scores, 2, 10);
        System.out.println("player 2: " + scores[2]);
        if (!setScore(scores, 4, 99)) {
            System.out.println("refused: player 4 is out of range, the size is " + scores.length);
        }
    }
}
```

```output for=check
player 2: 10
refused: player 4 is out of range, the size is 4
```

- Verdict: native. The Java Language Specification says that all array accesses are checked at run time, and an index less than zero or not less than the length throws `ArrayIndexOutOfBoundsException`.
- The exception stops the write. An uncaught exception is a denial of service, so check an index that comes from input.
- Native code that the program calls through JNI has no such check.
