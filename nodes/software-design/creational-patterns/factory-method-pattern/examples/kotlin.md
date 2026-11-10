```kotlin run timeout=30 id=export
interface Writer {
    fun header(): String
    fun row(name: String, count: Int): String
}

class CsvWriter : Writer {
    override fun header() = "name,count"
    override fun row(name: String, count: Int) = "$name,$count"
}

class TextWriter : Writer {
    override fun header() = "Inventory"
    override fun row(name: String, count: Int) = "$name: $count"
}

abstract class Exporter {
    fun export(rows: List<Pair<String, Int>>): String {
        val writer = createWriter() // the factory method
        return (listOf(writer.header()) + rows.map { (name, count) -> writer.row(name, count) })
            .joinToString("\n")
    }

    protected abstract fun createWriter(): Writer
}

class CsvExporter : Exporter() {
    override fun createWriter(): Writer = CsvWriter()
}

class TextExporter : Exporter() {
    override fun createWriter(): Writer = TextWriter()
}

fun main() {
    val rows = listOf("pen" to 3, "book" to 1)
    for (exporter in listOf(CsvExporter(), TextExporter())) {
        println(exporter.export(rows))
    }
}
```

```output
name,count
pen,3
book,1
Inventory
pen: 3
book: 1
```

- Verdict: idiom. Kotlin classes and methods are final unless they are `open` or `abstract`, so `export` cannot be overridden and only `createWriter` varies.
- A constructor parameter of type `() -> Writer`, given `::CsvWriter`, is the form with no creator subclass.
- A factory function at the top level, or in a `companion object`, is the Kotlin form of a static factory. It is not the pattern, because it cannot be overridden.
