```java run id=list
sealed interface IntList permits Nil, Cons {}

record Nil() implements IntList {}

record Cons(int head, IntList tail) implements IntList {}

public class Main {
    static int total(IntList xs) {
        return switch (xs) {
            case Nil n -> 0;
            case Cons c -> c.head() + total(c.tail());
        };
    }

    static int length(IntList xs) {
        return switch (xs) {
            case Nil n -> 0;
            case Cons c -> 1 + length(c.tail());
        };
    }

    public static void main(String[] args) {
        IntList xs = new Cons(1, new Cons(2, new Cons(3, new Nil())));
        System.out.println("sum: " + total(xs));
        System.out.println("length: " + length(xs));
        System.out.println("record: " + xs);
    }
}
```

```output for=list
sum: 6
length: 3
record: Cons[head=1, tail=Cons[head=2, tail=Cons[head=3, tail=Nil[]]]]
```

- Verdict: native. A field of a class or record type holds a reference, so `Cons` can have a field of type `IntList`, the interface that it implements.
- The sealed interface lists the two cases, so the `switch` needs no `default`.
- The generated `toString` of a record calls `toString` on each field, so it shows the whole list.
