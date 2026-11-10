```kotlin run id=parse-walk
sealed interface Expr
data class Num(val value: Int) : Expr
data class Var(val name: String) : Expr
data class Bin(val op: Char, val left: Expr, val right: Expr) : Expr

class Parser(private val src: String) {
    private var pos = 0

    private fun peek(): Char {
        while (pos < src.length && src[pos] == ' ') pos++
        return if (pos < src.length) src[pos] else '\u0000'
    }

    fun expr(): Expr {
        var left = term()
        while (peek() == '+' || peek() == '-') {
            val op = src[pos++]
            left = Bin(op, left, term())
        }
        return left
    }

    private fun term(): Expr {
        var left = factor()
        while (peek() == '*') {
            pos++
            left = Bin('*', left, factor())
        }
        return left
    }

    private fun factor(): Expr {
        val c = peek()
        if (c == '(') {
            pos++
            val inner = expr()
            check(peek() == ')') { "expected ) at $pos" }
            pos++
            return inner
        }
        val start = pos
        return when {
            c.isDigit() -> {
                while (pos < src.length && src[pos].isDigit()) pos++
                Num(src.substring(start, pos).toInt())
            }
            c.isLetter() -> {
                while (pos < src.length && src[pos].isLetter()) pos++
                Var(src.substring(start, pos))
            }
            else -> error("unexpected '$c' at $pos")
        }
    }
}

fun show(e: Expr, depth: Int = 0) {
    val pad = "  ".repeat(depth)
    when (e) {
        is Num -> println("${pad}Num ${e.value}")
        is Var -> println("${pad}Var ${e.name}")
        is Bin -> {
            println("${pad}Bin ${e.op}")
            show(e.left, depth + 1)
            show(e.right, depth + 1)
        }
    }
}

fun nodes(e: Expr): Sequence<Expr> = sequence {
    yield(e)
    if (e is Bin) {
        yieldAll(nodes(e.left))
        yieldAll(nodes(e.right))
    }
}

fun eval(e: Expr, env: Map<String, Int>): Int = when (e) {
    is Num -> e.value
    is Var -> env.getValue(e.name)
    is Bin -> {
        val l = eval(e.left, env)
        val r = eval(e.right, env)
        when (e.op) {
            '+' -> l + r
            '-' -> l - r
            else -> l * r
        }
    }
}

fun main() {
    val tree = Parser("1 + 2 * (3 - x)").expr()
    show(tree)
    println("nodes: ${nodes(tree).count()}")
    println("names: ${nodes(tree).filterIsInstance<Var>().map { it.name }.toList()}")
    println("value at x = 1: ${eval(tree, mapOf("x" to 1))}")
    println(tree)
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
Bin(op=+, left=Num(value=1), right=Bin(op=*, left=Num(value=2), right=Bin(op=-, left=Num(value=3), right=Var(name=x))))
```

- Verdict: idiom. A sealed interface and data classes describe the nodes, and `when` with `is` checks selects the case. A `when` used as an expression must cover every subclass.
- Data classes give structural `toString` and `equals`, so the last line prints the whole tree, and two trees with the same shape compare equal.
- On the JVM, Kotlin programs can also call the JDK parser in `com.sun.source.tree`, as the Java example does. It parses Java, not Kotlin.
