```kotlin run id=parser
data class Token(val kind: String, val text: String, val col: Int)
data class Result(val text: String, val value: Long)

class ParseError(message: String) : Exception(message)

fun tokenize(src: String): List<Token> {
    val tokens = mutableListOf<Token>()
    var i = 0
    while (i < src.length) {
        val c = src[i]
        when {
            c == ' ' -> i++
            c.isDigit() -> {
                var j = i
                while (j < src.length && src[j].isDigit()) j++
                tokens += Token("num", src.substring(i, j), i)
                i = j
            }
            c in "+-*/^()" -> {
                tokens += Token(c.toString(), c.toString(), i)
                i++
            }
            else -> throw ParseError("unexpected character '$c' at column ${i + 1}")
        }
    }
    tokens += Token("end", "", src.length)
    return tokens
}

fun combine(op: String, l: Result, r: Result): Result {
    val (a, b) = l.value to r.value
    val value = when (op) {
        "+" -> a + b
        "-" -> a - b
        "*" -> a * b
        "/" -> a / b
        else -> (1..b).fold(1L) { acc, _ -> acc * a }
    }
    return Result("(${l.text} $op ${r.text})", value)
}

class Parser(src: String) {
    private val tokens = tokenize(src)
    private var i = 0

    private fun peek() = tokens[i].kind
    private fun take() = tokens[i++]
    private fun fail(expected: String): Nothing =
        throw ParseError("expected $expected at column ${tokens[i].col + 1}")

    fun parse(): Result {
        val result = expr()
        if (peek() != "end") fail("an operator")
        return result
    }

    private fun expr(): Result {
        var left = term()
        while (peek() == "+" || peek() == "-") left = combine(take().kind, left, term())
        return left
    }

    private fun term(): Result {
        var left = unary()
        while (peek() == "*" || peek() == "/") left = combine(take().kind, left, unary())
        return left
    }

    private fun unary(): Result {
        if (peek() != "-") return power()
        take()
        val operand = unary()
        return Result("(-${operand.text})", -operand.value)
    }

    private fun power(): Result {
        val base = atom()
        if (peek() != "^") return base
        take()
        return combine("^", base, unary())
    }

    private fun atom(): Result {
        val tok = tokens[i]
        return when (tok.kind) {
            "num" -> {
                take()
                Result(tok.text, tok.text.toLong())
            }
            "(" -> {
                take()
                val inner = expr()
                if (peek() != ")") fail("')'")
                take()
                inner
            }
            else -> fail("a number or '('")
        }
    }
}

fun main() {
    for (src in listOf("1 + 2 * 3", "1 - 2 - 3", "2 ^ 3 ^ 2", "-2 ^ 2", "(1 + 2) * 3", "1 + * 2", "(1 + 2", "1 $ 2")) {
        try {
            val (text, value) = Parser(src).parse()
            println("$src -> $text = $value")
        } catch (e: ParseError) {
            println("$src -> error: ${e.message}")
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

- Verdict: idiom. One private function per rule. `when` used as an expression selects the case in `atom`.
- `fail` has the return type `Nothing`. The compiler knows that it never returns, so it can be a branch of a `when` that must produce a `Result`.
- Destructuring a data class, as in `val (text, value) = ...`, unpacks the result.
