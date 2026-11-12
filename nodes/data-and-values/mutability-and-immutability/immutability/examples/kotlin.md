```kotlin run id=val timeout=30
data class Point(val x: Int, val y: Int)

fun main() {
    val p = Point(1, 2)
    val q = p.copy(y = 5)
    println("$p $q")

    val names: List<String> = listOf("ada", "alan")
    val more = names + "grace"
    println("$names $more")

    val source = mutableListOf(1, 2)
    val view: List<Int> = source
    source.add(3)
    println(view)
}
```

```output for=val
Point(x=1, y=2) Point(x=1, y=5)
[ada, alan] [ada, alan, grace]
[1, 2, 3]
```

- Verdict: native. A `val` is assigned only once. A `var` can be assigned again.
- Each collection type has a read-only interface, such as `List`, and a mutable interface, such as `MutableList`. A read-only interface has no operations that add, remove or update elements.
- A read-only interface is not an immutable collection. Here `view` has the type `List<Int>`, and it shows the element that `source` added.
- The `copy` function of a data class makes a copy with some properties changed. The `+` operator on a `List` returns a fresh list.
- An assignment to a `val` is a compile error:

```kotlin fails expect=compile id=reassign timeout=30
fun main() {
    val count = 0
    count = 1
    println(count)
}
```

`kotlinc` stops with `'val' cannot be reassigned.`
