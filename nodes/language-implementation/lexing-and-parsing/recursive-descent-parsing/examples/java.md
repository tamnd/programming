```java run id=parser
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Token(String kind, String text, int col) {}
    record Result(String text, long value) {}

    static class ParseError extends RuntimeException {
        ParseError(String message) { super(message); }
    }

    static List<Token> tokenize(String src) {
        List<Token> tokens = new ArrayList<>();
        int i = 0;
        while (i < src.length()) {
            char c = src.charAt(i);
            if (c == ' ') {
                i++;
            } else if (Character.isDigit(c)) {
                int j = i;
                while (j < src.length() && Character.isDigit(src.charAt(j))) j++;
                tokens.add(new Token("num", src.substring(i, j), i));
                i = j;
            } else if ("+-*/^()".indexOf(c) >= 0) {
                tokens.add(new Token(String.valueOf(c), String.valueOf(c), i));
                i++;
            } else {
                throw new ParseError("unexpected character '" + c + "' at column " + (i + 1));
            }
        }
        tokens.add(new Token("end", "", src.length()));
        return tokens;
    }

    static long pow(long base, long exp) {
        long result = 1;
        for (long k = 0; k < exp; k++) result *= base;
        return result;
    }

    static Result combine(String op, Result l, Result r) {
        long a = l.value(), b = r.value();
        long value = switch (op) {
            case "+" -> a + b;
            case "-" -> a - b;
            case "*" -> a * b;
            case "/" -> a / b;
            default -> pow(a, b);
        };
        return new Result("(" + l.text() + " " + op + " " + r.text() + ")", value);
    }

    static class Parser {
        private final List<Token> tokens;
        private int i;

        Parser(String src) { tokens = tokenize(src); }

        Result parse() {
            Result result = expr();
            if (!peek().equals("end")) throw fail("an operator");
            return result;
        }

        private String peek() { return tokens.get(i).kind(); }
        private Token take() { return tokens.get(i++); }

        private ParseError fail(String expected) {
            return new ParseError("expected " + expected + " at column " + (tokens.get(i).col() + 1));
        }

        private Result expr() {
            Result left = term();
            while (peek().equals("+") || peek().equals("-")) left = combine(take().kind(), left, term());
            return left;
        }

        private Result term() {
            Result left = unary();
            while (peek().equals("*") || peek().equals("/")) left = combine(take().kind(), left, unary());
            return left;
        }

        private Result unary() {
            if (!peek().equals("-")) return power();
            take();
            Result operand = unary();
            return new Result("(-" + operand.text() + ")", -operand.value());
        }

        private Result power() {
            Result base = atom();
            if (!peek().equals("^")) return base;
            take();
            return combine("^", base, unary());
        }

        private Result atom() {
            Token tok = tokens.get(i);
            if (tok.kind().equals("num")) {
                take();
                return new Result(tok.text(), Long.parseLong(tok.text()));
            }
            if (tok.kind().equals("(")) {
                take();
                Result inner = expr();
                if (!peek().equals(")")) throw fail("')'");
                take();
                return inner;
            }
            throw fail("a number or '('");
        }
    }

    public static void main(String[] args) {
        String[] inputs = {"1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2"};
        for (String src : inputs) {
            try {
                Result r = new Parser(src).parse();
                System.out.println(src + " -> " + r.text() + " = " + r.value());
            } catch (ParseError e) {
                System.out.println(src + " -> error: " + e.getMessage());
            }
        }
    }
}
```

```output for=parser
1 + 2 * 3 -> (1 + (2 * 3)) = 7
1 - 2 - 3 -> ((1 - 2) - 3) = -4
2 ^ 3 ^ 2 -> (2 ^ (3 ^ 2)) = 512
-2 ^ 2 -> (-(2 ^ 2)) = -4
(1 + 2) * 3 -> ((1 + 2) * 3) = 9
1 + * 2 -> error: expected a number or '(' at column 5
(1 + 2 -> error: expected ')' at column 7
1 $ 2 -> error: unexpected character '$' at column 3
```

- Verdict: idiom. One private method per rule, and records for tokens and results.
- `fail` returns the exception, and the caller writes `throw fail(...)`. The compiler then knows that the method does not continue after the call, so `atom` needs no dummy return value.
- `ParseError` extends `RuntimeException`, so the recursive methods need no `throws` clause. The `catch` in `main` handles only parse errors.
