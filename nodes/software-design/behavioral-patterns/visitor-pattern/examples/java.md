```java run id=classic
public class Main {
    // The classic form: each node has accept, and each operation is a Visitor.
    interface Visitor<R> {
        R num(Num n);

        R add(Add n);

        R mul(Mul n);
    }

    sealed interface Expr permits Num, Add, Mul {
        <R> R accept(Visitor<R> v);
    }

    record Num(int value) implements Expr {
        public <R> R accept(Visitor<R> v) {
            return v.num(this);
        }
    }

    record Add(Expr left, Expr right) implements Expr {
        public <R> R accept(Visitor<R> v) {
            return v.add(this);
        }
    }

    record Mul(Expr left, Expr right) implements Expr {
        public <R> R accept(Visitor<R> v) {
            return v.mul(this);
        }
    }

    static final class Show implements Visitor<String> {
        public String num(Num n) {
            return Integer.toString(n.value());
        }

        public String add(Add n) {
            return "(" + n.left().accept(this) + " + " + n.right().accept(this) + ")";
        }

        public String mul(Mul n) {
            return "(" + n.left().accept(this) + " * " + n.right().accept(this) + ")";
        }
    }

    static final class Evaluate implements Visitor<Integer> {
        public Integer num(Num n) {
            return n.value();
        }

        public Integer add(Add n) {
            return n.left().accept(this) + n.right().accept(this);
        }

        public Integer mul(Mul n) {
            return n.left().accept(this) * n.right().accept(this);
        }
    }

    static final class Count implements Visitor<Integer> {
        public Integer num(Num n) {
            return 1;
        }

        public Integer add(Add n) {
            return 1 + n.left().accept(this) + n.right().accept(this);
        }

        public Integer mul(Mul n) {
            return 1 + n.left().accept(this) + n.right().accept(this);
        }
    }

    public static void main(String[] args) {
        Expr tree = new Mul(new Add(new Num(2), new Num(3)), new Num(4));
        System.out.println("show: " + tree.accept(new Show()));
        System.out.println("evaluate: " + tree.accept(new Evaluate()));
        System.out.println("nodes: " + tree.accept(new Count()));
    }
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

With a sealed interface, a `switch` with record patterns does the same work without `accept`. The compiler checks that the cases cover every permitted subtype:

```java run id=switch
public class Main {
    sealed interface Expr permits Num, Add, Mul {}

    record Num(int value) implements Expr {}

    record Add(Expr left, Expr right) implements Expr {}

    record Mul(Expr left, Expr right) implements Expr {}

    static String show(Expr e) {
        return switch (e) {
            case Num(int v) -> Integer.toString(v);
            case Add(Expr l, Expr r) -> "(" + show(l) + " + " + show(r) + ")";
            case Mul(Expr l, Expr r) -> "(" + show(l) + " * " + show(r) + ")";
        };
    }

    static int evaluate(Expr e) {
        return switch (e) {
            case Num(int v) -> v;
            case Add(Expr l, Expr r) -> evaluate(l) + evaluate(r);
            case Mul(Expr l, Expr r) -> evaluate(l) * evaluate(r);
        };
    }

    static int count(Expr e) {
        return switch (e) {
            case Num n -> 1;
            case Add(Expr l, Expr r) -> 1 + count(l) + count(r);
            case Mul(Expr l, Expr r) -> 1 + count(l) + count(r);
        };
    }

    public static void main(String[] args) {
        Expr tree = new Mul(new Add(new Num(2), new Num(3)), new Num(4));
        System.out.println("show: " + show(tree));
        System.out.println("evaluate: " + evaluate(tree));
        System.out.println("nodes: " + count(tree));
    }
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: idiom. The first program is the double dispatch of Design Patterns. `accept` selects the visitor method by the class of the node, and the visitor object selects the operation.
- The generic parameter `R` lets each visitor return its own type, so no visitor keeps its result in a field.
- The second program has no visitor. The `switch` has no `default`, and it compiles because `Expr` is sealed and the cases cover `Num`, `Add` and `Mul`. A fourth record in `permits` makes each of these switches a compile error.
- `java.nio.file.FileVisitor` with `Files.walkFileTree` is a visitor in the standard library. The walk calls the visitor before and after each directory and for each file.
