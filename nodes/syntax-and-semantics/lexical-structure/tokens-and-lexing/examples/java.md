```java run id=unicode-escapes-and-text-blocks
public class Main {
    public static void main(String[] args) {
        int i = 1, j = 10;
        int k = i+++j; // lexed as i ++ + j
        System.out.println(k + " " + i);

        System.out.println(1_000_000 + " " + 0b1010 + " " + 017 + " " + 0xff + " " + 10L);
        // Unicode escapes are replaced before lexing: \u0041 is the letter A
        char a = '\u0041';
        System.out.println(a);
        String block = """
            two lines,
            indent removed""";
        System.out.println(block);
        var record = 5; // record is a contextual keyword, so it can be a name
        System.out.println(record);
    }
}
```

```output for=unicode-escapes-and-text-blocks
11 2
1000000 10 15 255 10
A
two lines,
indent removed
5
```

- Verdict: library. The JDK has no public API for the tokens of Java source, so tools use libraries such as JavaParser or the compiler tree API of the `jdk.compiler` module. Java translates Unicode escapes such as `\u0041` before it forms tokens, so they work anywhere in the source, even outside strings. Text blocks remove the common indentation, and contextual keywords such as `record` and `var` remain usable as names.
