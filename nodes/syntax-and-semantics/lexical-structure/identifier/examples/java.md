```java run id=java-letters-and-contextual-keywords
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // Java names can use letters of any script, digits after the first character, _ and $.
        int größe = 3, $count = 4, π2 = 5;
        System.out.println(größe + $count + π2);

        // var and record are contextual keywords, so they can still name a variable.
        int var = 1;
        String record = "r";
        System.out.println(var + record);

        // A Unicode escape is translated before the lexer finds names, so these are one variable.
        int abc = 7;
        System.out.println(\u0061bc);

        // _ alone is a keyword, and it declares a variable that the code does not use.
        for (var _ : List.of(1, 2, 3)) {
            System.out.print("*");
        }
        System.out.println();

        // Character checks only the form of a name, so it accepts the keywords _ and class.
        for (String s : List.of("x1", "1x", "a-b", "_", "class")) {
            boolean ok = Character.isJavaIdentifierStart(s.charAt(0))
                    && s.chars().skip(1).allMatch(Character::isJavaIdentifierPart);
            System.out.println(s + " " + ok);
        }
    }
}
```

```output for=java-letters-and-contextual-keywords
12
1r
7
***
x1 true
1x false
a-b false
_ true
class true
```

- Verdict: native. Java names can use letters of any script, `$` and `_`. `var` and `record` are contextual keywords, so they can name variables. The compiler translates a Unicode escape before it finds names, so the escape for `a` and the letter are one variable. `_` alone is a keyword and declares an unused variable. `Character.isJavaIdentifierStart` and `isJavaIdentifierPart` check only the form, so they accept `_` and `class`.
