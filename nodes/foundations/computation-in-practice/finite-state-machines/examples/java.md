```java run id=enum-and-switch-expression
import java.util.List;
import java.util.regex.Pattern;

public class Main {
    enum S { START, SIGN, INT, DOT, FRAC, DEAD }

    static S step(S s, char ch) {
        boolean digit = ch >= '0' && ch <= '9';
        return switch (s) { // the compiler checks that every state has a case
            case START -> ch == '+' || ch == '-' ? S.SIGN : digit ? S.INT : S.DEAD;
            case SIGN -> digit ? S.INT : S.DEAD;
            case INT -> digit ? S.INT : ch == '.' ? S.DOT : S.DEAD;
            case DOT, FRAC -> digit ? S.FRAC : S.DEAD;
            case DEAD -> S.DEAD;
        };
    }

    static boolean accepts(String text) {
        S s = S.START;
        for (char ch : text.toCharArray()) s = step(s, ch);
        return s == S.INT || s == S.FRAC;
    }

    public static void main(String[] args) {
        List<String> inputs = List.of("42", "-3.5", "1.", "+", "007", ".5", "4.2.1");
        for (String t : inputs) System.out.println(t + " " + accepts(t));
        Pattern p = Pattern.compile("[+-]?[0-9]+(\\.[0-9]+)?");
        System.out.println("regex agrees: "
            + inputs.stream().allMatch(t -> p.matcher(t).matches() == accepts(t)));
    }
}
```

```output for=enum-and-switch-expression
42 true
-3.5 true
1. false
+ false
007 true
.5 false
4.2.1 false
regex agrees: true
```

- Verdict: idiom. A Java `enum` names the states, and a `switch` expression over it must cover every constant, so the compiler rejects the method if a new state has no case. `java.util.regex` matches the same language.
