```kotlin run id=interpreter
sealed interface Expr
data class Num(val value: Long) : Expr
data class Var(val name: String) : Expr
data class Bin(val op: Char, val left: Expr, val right: Expr) : Expr

sealed interface Stmt
data class Let(val name: String, val init: Expr) : Stmt
data class Assign(val name: String, val value: Expr) : Stmt
data class Block(val body: List<Stmt>) : Stmt
data class While(val cond: Expr, val body: Stmt) : Stmt
data class Print(val value: Expr) : Stmt

class Env(val parent: Env? = null) {
    val vars = mutableMapOf<String, Long>()
}

class Interpreter {
    var nodes = 0
    var lookups = 0
    var links = 0

    private fun find(env: Env, name: String): Env {
        lookups++
        var e: Env? = env
        while (e != null) {
            if (name in e.vars) return e
            links++
            e = e.parent
        }
        error("undefined variable '$name'")
    }

    fun evaluate(node: Expr, env: Env): Long {
        nodes++
        return when (node) {
            is Num -> node.value
            is Var -> find(env, node.name).vars.getValue(node.name)
            is Bin -> {
                val a = evaluate(node.left, env)
                val b = evaluate(node.right, env)
                when (node.op) {
                    '+' -> a + b
                    '-' -> a - b
                    '*' -> a * b
                    '<' -> if (a < b) 1 else 0
                    else -> error("unknown operator ${node.op}")
                }
            }
        }
    }

    fun execute(node: Stmt, env: Env) {
        nodes++
        when (node) {
            is Let -> env.vars[node.name] = evaluate(node.init, env)
            is Assign -> {
                val result = evaluate(node.value, env)
                find(env, node.name).vars[node.name] = result
            }
            is Block -> {
                val inner = Env(env)
                for (stmt in node.body) execute(stmt, inner)
            }
            is While -> while (evaluate(node.cond, env) != 0L) execute(node.body, env)
            is Print -> println(evaluate(node.value, env))
        }
    }
}

fun main() {
    val program = Block(listOf(
        Let("n", Num(5)),
        Let("acc", Num(1)),
        While(Bin('<', Num(0), Var("n")), Block(listOf(
            Let("step", Var("n")),
            Assign("acc", Bin('*', Var("acc"), Var("step"))),
            Assign("n", Bin('-', Var("n"), Num(1))),
        ))),
        Print(Var("acc")),
    ))

    val interp = Interpreter()
    interp.execute(program, Env())
    println("nodes visited: ${interp.nodes}")
    println("variable lookups: ${interp.lookups}, parent links followed: ${interp.links}")
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Sealed interfaces and data classes model the tree. `when` with `is` checks the type, and smart casts give access to the fields of each case.
- `when` used as an expression over a sealed interface must cover every subtype, or the compiler rejects it.
- `error(...)` throws an `IllegalStateException`. Its return type is `Nothing`, so `find` needs no return after the loop.
