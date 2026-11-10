```kotlin run timeout=30 id=hierarchy
interface Shape {
    fun area(): Double
    fun scaled(factor: Double): Shape
}

open class Square(val side: Double) : Shape {
    override fun area() = side * side
    override fun scaled(factor: Double): Square = Square(side * factor)
}

class Tile(side: Double, val colour: String) : Square(side)

fun total(shapes: List<Shape>): Double = shapes.sumOf { it.area() }

fun main() {
    val squares: List<Square> = listOf(Square(1.0), Tile(2.0, "red"))
    println(total(squares))
    val big: Square = Square(1.0).scaled(3.0)
    println(big.side)
    for (shape in squares) {
        println(shape::class.simpleName)
    }
    val name: String = "Ada"
    val maybe: String? = name
    println(maybe?.length)
    val numbers: Array<Int> = arrayOf(1, 2)
    val anything: Array<out Any> = numbers
    println(anything.joinToString())
}
```

```output for=hierarchy
5.0
3.0
Square
Tile
3
1, 2
```

- Verdict: native. Subtyping is nominal. A class is final unless it is `open`, so a class can have a subclass only when its author allows it.
- `List<out E>` is covariant, so `total` accepts a `List<Square>` where it expects a `List<Shape>`.
- `Array<T>` is invariant. The projection `Array<out Any>` gives a view of `Array<Int>` that can only be read.
- An override can return a subtype, as `Square.scaled` does. A `String` is assignable to a `String?`.
