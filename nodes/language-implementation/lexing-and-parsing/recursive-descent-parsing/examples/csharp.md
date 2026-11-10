```csharp run id=parser
using System;
using System.Collections.Generic;

string[] inputs = { "1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2" };
foreach (var src in inputs)
{
    try
    {
        var (text, value) = new Parser(src).Parse();
        Console.WriteLine($"{src} -> {text} = {value}");
    }
    catch (ParseError e)
    {
        Console.WriteLine($"{src} -> error: {e.Message}");
    }
}

record Token(string Kind, string Text, int Col);

class ParseError(string message) : Exception(message);

class Parser
{
    readonly List<Token> tokens = new();
    int i;

    public Parser(string src)
    {
        int p = 0;
        while (p < src.Length)
        {
            char c = src[p];
            if (c == ' ') { p++; }
            else if (char.IsDigit(c))
            {
                int j = p;
                while (j < src.Length && char.IsDigit(src[j])) j++;
                tokens.Add(new Token("num", src[p..j], p));
                p = j;
            }
            else if ("+-*/^()".Contains(c)) { tokens.Add(new Token(c.ToString(), c.ToString(), p)); p++; }
            else throw new ParseError($"unexpected character '{c}' at column {p + 1}");
        }
        tokens.Add(new Token("end", "", src.Length));
    }

    public (string Text, long Value) Parse()
    {
        var result = Expr();
        if (Peek() != "end") throw Fail("an operator");
        return result;
    }

    string Peek() => tokens[i].Kind;
    Token Take() => tokens[i++];
    ParseError Fail(string expected) => new($"expected {expected} at column {tokens[i].Col + 1}");

    static (string, long) Combine(string op, (string Text, long Value) l, (string Text, long Value) r)
    {
        long a = l.Value, b = r.Value;
        long value = op switch
        {
            "+" => a + b,
            "-" => a - b,
            "*" => a * b,
            "/" => a / b,
            _ => (long)Math.Pow(a, b),
        };
        return ($"({l.Text} {op} {r.Text})", value);
    }

    (string Text, long Value) Expr()
    {
        var left = Term();
        while (Peek() is "+" or "-") left = Combine(Take().Kind, left, Term());
        return left;
    }

    (string Text, long Value) Term()
    {
        var left = Unary();
        while (Peek() is "*" or "/") left = Combine(Take().Kind, left, Unary());
        return left;
    }

    (string Text, long Value) Unary()
    {
        if (Peek() != "-") return Power();
        Take();
        var (text, value) = Unary();
        return ($"(-{text})", -value);
    }

    (string Text, long Value) Power()
    {
        var b = Atom();
        if (Peek() != "^") return b;
        Take();
        return Combine("^", b, Unary());
    }

    (string Text, long Value) Atom()
    {
        var tok = tokens[i];
        if (tok.Kind == "num")
        {
            Take();
            return (tok.Text, long.Parse(tok.Text));
        }
        if (tok.Kind == "(")
        {
            Take();
            var inner = Expr();
            if (Peek() != ")") throw Fail("')'");
            Take();
            return inner;
        }
        throw Fail("a number or '('");
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

- Verdict: idiom. One method per rule. Each rule returns a named tuple of the text and the value, which the caller can deconstruct.
- `Peek() is "+" or "-"` is a pattern with `or`. It tests the next token against two constants without repeating the call.
- `ParseError` uses a primary constructor. The `catch` clause handles only errors in the input.
