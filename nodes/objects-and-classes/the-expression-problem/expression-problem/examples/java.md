```java run id=visitor
// The visitor pattern: the cases are fixed, and each operation is a new class.
interface Expr {
    <R> R accept(Visitor<R> v);
}

interface Visitor<R> {
    R num(int value);
    R add(Expr left, Expr right);
}

record Num(int value) implements Expr {
    public <R> R accept(Visitor<R> v) { return v.num(value); }
}

record Add(Expr left, Expr right) implements Expr {
    public <R> R accept(Visitor<R> v) { return v.add(left, right); }
}

class Eval implements Visitor<Integer> {
    public Integer num(int value) { return value; }
    public Integer add(Expr l, Expr r) { return l.accept(this) + r.accept(this); }
}

// A new operation: one new class, no change to Expr, Num or Add.
class Show implements Visitor<String> {
    public String num(int value) { return Integer.toString(value); }
    public String add(Expr l, Expr r) {
        return "(" + l.accept(this) + " + " + r.accept(this) + ")";
    }
}

public class Main {
    public static void main(String[] args) {
        Expr e = new Add(new Num(1), new Add(new Num(2), new Num(3)));
        String text = e.accept(new Show());
        int value = e.accept(new Eval());
        System.out.println(text + " = " + value);
    }
}
```

```output for=visitor
(1 + (2 + 3)) = 6
```

```java fails expect=compile id=sealed-switch
// A sealed interface lists every case; a switch over it must cover them all.
sealed interface Expr permits Num, Add, Neg {}

record Num(int value) implements Expr {}
record Add(Expr left, Expr right) implements Expr {}
record Neg(Expr inner) implements Expr {}  // a new case

public class Main {
    static int evaluate(Expr e) {
        return switch (e) {
            case Num n -> n.value();
            case Add a -> evaluate(a.left()) + evaluate(a.right());
            // no case for Neg
        };
    }

    public static void main(String[] args) {
        System.out.println(evaluate(new Add(new Num(1), new Neg(new Num(5)))));
    }
}
```

- Verdict: idiom. Java gives both halves of the problem. With classes and methods, a new case is a new class. With the visitor pattern, a new operation is a new class that implements `Visitor`, but a new case must add a method to `Visitor` and to every class that implements it. With a `sealed` interface and records, a `switch` over the cases needs no visitor, and the compiler checks that the `switch` covers every permitted case.
- The failing block adds `Neg` to the sealed interface without a branch for it. The compiler rejects the `switch` because it does not cover all possible input values, so the programmer finds every operation to update. The check works only inside the code that declares the sealed interface, and code outside it cannot add a case at all.
