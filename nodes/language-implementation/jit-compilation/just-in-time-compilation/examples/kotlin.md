```kotlin run id=engine
// Values carry their type, as boxed values do in a dynamic language.
sealed interface Value
data class IntV(val n: Long) : Value { override fun toString() = n.toString() }
data class FloatV(val x: Double) : Value { override fun toString() = x.toString() }

val Value.kind get() = when (this) { is IntV -> "int"; is FloatV -> "float" }

// The body of f(x) = x * x + 1 as a tree.
sealed interface Node
data class Num(val value: Long) : Node
data object Arg : Node
data class Bin(val op: Char, val left: Node, val right: Node) : Node

fun intOp(op: Char): (Long, Long) -> Long = if (op == '+') Long::plus else Long::times

fun Value.toDouble() = when (this) { is IntV -> n.toDouble(); is FloatV -> x }

// Generic arithmetic: check both tags on every operation.
fun arith(op: Char, a: Value, b: Value): Value =
    if (a is IntV && b is IntV) IntV(intOp(op)(a.n, b.n))
    else if (op == '+') FloatV(a.toDouble() + b.toDouble())
    else FloatV(a.toDouble() * b.toDouble())

// The interpreter: walk the tree on every call.
fun interpret(node: Node, arg: Value): Value = when (node) {
    is Num -> IntV(node.value)
    Arg -> arg
    is Bin -> arith(node.op, interpret(node.left, arg), interpret(node.right, arg))
}

// Compiled code for int: closures over raw longs, no tag checks inside.
fun compileInt(node: Node): (Long) -> Long = when (node) {
    is Num -> { val v = node.value; { _ -> v } }
    Arg -> { n -> n }
    is Bin -> {
        val f = intOp(node.op)
        val l = compileInt(node.left)
        val r = compileInt(node.right);
        { n -> f(l(n), r(n)) }
    }
}

// Compiled code for any argument: closures that still check tags.
fun compileGeneric(node: Node): (Value) -> Value = when (node) {
    is Num -> { val boxed = IntV(node.value); { _ -> boxed } }
    Arg -> { a -> a }
    is Bin -> {
        val l = compileGeneric(node.left)
        val r = compileGeneric(node.right);
        { a -> arith(node.op, l(a), r(a)) }
    }
}

const val THRESHOLD = 3

class JitFunction(val name: String, val body: Node) {
    var calls = 0 // calls in the interpreter since the last compilation
    val seen = sortedSetOf<String>() // the profile: the kinds of the arguments
    var code: ((Value) -> Value?)? = null // compiled code; it returns null when the guard fails
    var kind = ""
    val counts = linkedMapOf("interpreter" to 0, "compiled" to 0, "deoptimizations" to 0)

    fun compile() {
        val how: String
        if (seen == setOf("int")) {
            val raw = compileInt(body)
            code = { a -> if (a is IntV) IntV(raw(a.n)) else null } // the guard
            kind = "int"
            how = "specialized for int"
        } else {
            code = compileGeneric(body)
            kind = "generic"
            how = "generic"
        }
        println("* compile $name: arguments seen: ${seen.joinToString(", ")}; $how")
    }

    fun call(arg: Value): Pair<Value, String> {
        if (code == null && calls >= THRESHOLD) compile()
        code?.let { compiled ->
            val result = compiled(arg)
            if (result != null) {
                counts["compiled"] = counts.getValue("compiled") + 1
                return result to "compiled, $kind"
            }
            println("* deoptimize $name: guard failed, $arg is not an int")
            counts["deoptimizations"] = counts.getValue("deoptimizations") + 1
            code = null
            calls = 0
        }
        calls++
        counts["interpreter"] = counts.getValue("interpreter") + 1
        seen.add(arg.kind)
        return interpret(body, arg) to "interpreter, call $calls"
    }
}

fun main() {
    val f = JitFunction("f", Bin('+', Bin('*', Arg, Arg), Num(1)))
    for (arg in listOf(IntV(1), IntV(2), IntV(3), IntV(4), FloatV(2.5), IntV(6), IntV(7), IntV(8))) {
        val (result, tier) = f.call(arg)
        println("f($arg) = $result [$tier]")
    }
    println(f.counts.entries.joinToString(", ") { "${it.key}: ${it.value}" })
}
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: native. On the JVM, Kotlin code runs as bytecode under HotSpot, with the same interpreter and tiered compilers as Java.
- `if (a is IntV)` is the guard. After the test, the smart cast lets the code read `a.n` with no explicit cast.
- `when` over a sealed interface needs no `else` branch, so the compiler reports a missing case in `interpret`, `compileInt` or `compileGeneric`.
