```java run id=parse-print deps=deps/java/pom.xml
import java.io.StringReader;
import java.io.StringWriter;
import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVParser;
import org.apache.commons.csv.CSVPrinter;
import org.apache.commons.csv.CSVRecord;

public class Main {
    public static void main(String[] args) throws Exception {
        String text = "id,name,note\r\n7,\"Smith, Ann\",\"said \"\"hi\"\"\r\nand left\"\r\n8,Bo,\r\n";
        CSVFormat format = CSVFormat.RFC4180.builder().setHeader().setSkipHeaderRecord(true).get();
        try (CSVParser parser = CSVParser.parse(new StringReader(text), format)) {
            for (CSVRecord record : parser) {
                String note = record.get("note").replace("\r\n", "\\r\\n");
                System.out.println(record.get("id") + " | " + record.get("name") + " | " + note);
            }
        }

        StringWriter out = new StringWriter();
        try (CSVPrinter printer = new CSVPrinter(out, CSVFormat.RFC4180)) {
            printer.printRecord("7", "Smith, Ann", "said \"hi\"");
            printer.printRecord("8", " Bo", "");
        }
        System.out.print(out.toString().replace("\r\n", "\\r\\n\n"));
    }
}
```

```output for=parse-print
7 | Smith, Ann | said "hi"\r\nand left
8 | Bo |
7,"Smith, Ann","said ""hi"""\r\n
8," Bo",\r\n
```

```java run id=dialects deps=deps/java/pom.xml
import java.io.StringReader;
import org.apache.commons.csv.CSVFormat;
import org.apache.commons.csv.CSVParser;
import org.apache.commons.csv.CSVRecord;

public class Main {
    public static void main(String[] args) throws Exception {
        CSVFormat excelFr = CSVFormat.EXCEL.builder().setDelimiter(';').get();
        try (CSVParser parser = CSVParser.parse(new StringReader("name;price\r\nTea;3,50\r\n"), excelFr)) {
            for (CSVRecord record : parser) {
                System.out.println(record.toList());
            }
        }

        try (CSVParser parser = CSVParser.parse(new StringReader("a,b\r\n1,2,3\r\n"), CSVFormat.RFC4180)) {
            for (CSVRecord record : parser) {
                System.out.println(record.size() + " " + record.toList());
            }
        }

        try (CSVParser parser = CSVParser.parse(new StringReader("7,\"Smith\" Ann\r\n"), CSVFormat.RFC4180)) {
            for (CSVRecord record : parser) {
                System.out.println(record.toList());
            }
        } catch (Exception e) {
            System.out.println(e.getClass().getSimpleName() + ": " + e.getMessage());
        }
    }
}
```

```output for=dialects
[name, price]
[Tea, 3,50]
2 [a, b]
3 [1, 2, 3]
UncheckedIOException: org.apache.commons.csv.CSVException: Invalid character between encapsulated token and delimiter at line: 1, position: 11
```

- Verdict: library. Java SE has no CSV API. The examples use Apache Commons CSV 1.14.1.
- `CSVFormat` holds the dialect. `RFC4180`, `EXCEL`, `MYSQL`, `POSTGRESQL_CSV` and `TDF` are predefined, and the builder changes them.
- The Commons CSV documentation says that the delimiter of Excel depends on the locale. The second example reads a file from a French system with `;`.
- The parser does not check the field count. The ragged row comes back with 3 fields and no error.
