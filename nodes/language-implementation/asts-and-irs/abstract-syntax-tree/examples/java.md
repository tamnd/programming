```java run id=parse-walk
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Main {
    sealed interface Expr permits Num, Var, Bin {}
    record Num(int value) implements Expr {}
    record Var(String name) implements Expr {}
    record Bin(char op, Expr left, Expr right) implements Expr {}

    static final class Parser {
        private final String src;
        private int pos;

        Parser(String src) { this.src = src; }

        private char peek() {
            while (pos < src.length() && src.charAt(pos) == ' ') pos++;
            return pos < src.length() ? src.charAt(pos) : '\0';
        }

        Expr expr() {
            Expr left = term();
            while (peek() == '+' || peek() == '-') {
                char op = src.charAt(pos++);
                left = new Bin(op, left, term());
            }
            return left;
        }

        Expr term() {
            Expr left = factor();
            while (peek() == '*') {
                pos++;
                left = new Bin('*', left, factor());
            }
            return left;
        }

        Expr factor() {
            char c = peek();
            if (c == '(') {
                pos++;
                Expr inner = expr();
                if (peek() != ')') throw new IllegalStateException("expected ) at " + pos);
                pos++;
                return inner;
            }
            int start = pos;
            if (Character.isDigit(c)) {
                while (pos < src.length() && Character.isDigit(src.charAt(pos))) pos++;
                return new Num(Integer.parseInt(src.substring(start, pos)));
            }
            if (Character.isLetter(c)) {
                while (pos < src.length() && Character.isLetter(src.charAt(pos))) pos++;
                return new Var(src.substring(start, pos));
            }
            throw new IllegalStateException("unexpected '" + c + "' at " + pos);
        }
    }

    static void show(Expr e, int depth) {
        String pad = "  ".repeat(depth);
        switch (e) {
            case Num n -> System.out.println(pad + "Num " + n.value());
            case Var v -> System.out.println(pad + "Var " + v.name());
            case Bin b -> {
                System.out.println(pad + "Bin " + b.op());
                show(b.left(), depth + 1);
                show(b.right(), depth + 1);
            }
        }
    }

    static int count(Expr e) {
        return switch (e) {
            case Num n -> 1;
            case Var v -> 1;
            case Bin b -> 1 + count(b.left()) + count(b.right());
        };
    }

    static void names(Expr e, List<String> out) {
        switch (e) {
            case Num n -> { }
            case Var v -> out.add(v.name());
            case Bin b -> {
                names(b.left(), out);
                names(b.right(), out);
            }
        }
    }

    static int eval(Expr e, Map<String, Integer> env) {
        return switch (e) {
            case Num n -> n.value();
            case Var v -> env.get(v.name());
            case Bin b -> switch (b.op()) {
                case '+' -> eval(b.left(), env) + eval(b.right(), env);
                case '-' -> eval(b.left(), env) - eval(b.right(), env);
                default -> eval(b.left(), env) * eval(b.right(), env);
            };
        };
    }

    public static void main(String[] args) {
        Expr tree = new Parser("1 + 2 * (3 - x)").expr();
        show(tree, 0);
        System.out.println("nodes: " + count(tree));
        List<String> found = new ArrayList<>();
        names(tree, found);
        System.out.println("names: " + found);
        System.out.println("value at x = 1: " + eval(tree, Map.of("x", 1)));
    }
}
```

```output for=parse-walk
Bin +
  Num 1
  Bin *
    Num 2
    Bin -
      Num 3
      Var x
nodes: 7
names: [x]
value at x = 1: 5
```

- Verdict: idiom. A sealed interface lists the kinds of node, and records give each kind its fields. A `switch` over the sealed interface needs no `default`, because the compiler knows every case.
- The parser has one method per precedence level. Each method returns the node for the text that it read, and `factor` returns the inner node of a parenthesized expression, so the tree has no node for the parentheses.

The JDK also exposes its own parser. `JavacTask.parse` returns trees from `com.sun.source.tree`, and a `TreeScanner` walks them. Unlike the hand-written tree, `javac` keeps a `PARENTHESIZED` node.

```java run id=javac-tree
import com.sun.source.tree.BinaryTree;
import com.sun.source.tree.CompilationUnitTree;
import com.sun.source.tree.IdentifierTree;
import com.sun.source.tree.LiteralTree;
import com.sun.source.tree.ParenthesizedTree;
import com.sun.source.util.JavacTask;
import com.sun.source.util.TreeScanner;
import java.net.URI;
import java.util.List;
import javax.tools.JavaCompiler;
import javax.tools.SimpleJavaFileObject;
import javax.tools.ToolProvider;

public class Main {
    static class Source extends SimpleJavaFileObject {
        final String code;

        Source(String code) {
            super(URI.create("string:///F.java"), Kind.SOURCE);
            this.code = code;
        }

        @Override
        public CharSequence getCharContent(boolean ignoreEncodingErrors) {
            return code;
        }
    }

    public static void main(String[] args) throws Exception {
        String code = "class F { int f(int x) { return 1 + 2 * (3 - x); } }";
        JavaCompiler compiler = ToolProvider.getSystemJavaCompiler();
        JavacTask task = (JavacTask) compiler.getTask(null, null, null, null, null, List.of(new Source(code)));
        for (CompilationUnitTree unit : task.parse()) {
            unit.accept(new TreeScanner<Void, Integer>() {
                @Override
                public Void visitBinary(BinaryTree node, Integer depth) {
                    System.out.println("  ".repeat(depth) + node.getKind());
                    scan(node.getLeftOperand(), depth + 1);
                    return scan(node.getRightOperand(), depth + 1);
                }

                @Override
                public Void visitParenthesized(ParenthesizedTree node, Integer depth) {
                    System.out.println("  ".repeat(depth) + node.getKind());
                    return scan(node.getExpression(), depth + 1);
                }

                @Override
                public Void visitLiteral(LiteralTree node, Integer depth) {
                    System.out.println("  ".repeat(depth) + node.getKind() + " " + node.getValue());
                    return null;
                }

                @Override
                public Void visitIdentifier(IdentifierTree node, Integer depth) {
                    System.out.println("  ".repeat(depth) + node.getKind() + " " + node.getName());
                    return null;
                }
            }, 0);
        }
    }
}
```

```output for=javac-tree
PLUS
  INT_LITERAL 1
  MULTIPLY
    INT_LITERAL 2
    PARENTHESIZED
      MINUS
        INT_LITERAL 3
        IDENTIFIER x
```

- `com.sun.source.tree` is in the module `jdk.compiler`. It ships with the JDK but not with every Java runtime, and it is not part of the Java SE API.
