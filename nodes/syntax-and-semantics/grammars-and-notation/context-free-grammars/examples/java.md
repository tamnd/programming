```java run id=generate-sentences
import java.util.List;
import java.util.Map;
import java.util.Random;

public class Main {
    // stmt = name "=" expr ";" | "print" expr ";" .
    // expr = term | term "+" expr .
    // term = name | number | "(" expr ")" .
    static final Map<String, List<List<String>>> GRAMMAR = Map.of(
        "stmt", List.of(List.of("name", "=", "expr", ";"), List.of("print", "expr", ";")),
        "expr", List.of(List.of("term"), List.of("term", "+", "expr")),
        "term", List.of(List.of("name"), List.of("number"), List.of("(", "expr", ")")),
        "name", List.of(List.of("x"), List.of("y"), List.of("total")),
        "number", List.of(List.of("0"), List.of("1"), List.of("42")));

    static void generate(String symbol, Random rnd, StringBuilder out) {
        List<List<String>> alts = GRAMMAR.get(symbol);
        if (alts == null) { // a terminal stands for itself
            out.append(symbol).append(' ');
            return;
        }
        for (String s : alts.get(rnd.nextInt(alts.size()))) {
            generate(s, rnd, out);
        }
    }

    public static void main(String[] args) {
        Random rnd = new Random(7);
        for (int k = 0; k < 6; k++) {
            StringBuilder out = new StringBuilder();
            generate("stmt", rnd, out);
            System.out.println(out.toString().strip());
        }
    }
}
```

```output for=generate-sentences
print y + 1 ;
print x + y + 0 + x + ( 42 + y + ( y ) ) + total + x ;
print ( y ) ;
print y ;
x = total ;
print x + y ;
```

- Verdict: library. ANTLR, JavaCC and parboiled generate parsers from grammars, and the Java Language Specification defines Java with its own grammar notation. A grammar generates sentences as well as describes them: the example starts from `stmt`, picks a random alternative for each nonterminal, and writes terminals as they are. Each output line is a valid statement of the little language. Test tools use this to create inputs for parsers and compilers. A fixed seed makes the output the same on every run.
