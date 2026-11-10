```java run id=parse-emit deps=deps/java/pom.xml
import org.tomlj.Toml;
import org.tomlj.TomlArray;
import org.tomlj.TomlParseResult;

public class Main {
    public static void main(String[] args) {
        String text = """
                # Billing service
                name = "billing"
                port = 8080 # the public port
                started = 2026-11-10T09:30:00+07:00
                cutoff = 2026-11-10T17:00:00
                holiday = 2026-12-25
                backup = 02:30:00

                [database]
                url = 'postgres://db.local/billing'

                [[worker]]
                queue = "invoices"
                """;

        TomlParseResult config = Toml.parse(text);
        System.out.println(config.hasErrors());
        System.out.println(config.getString("name") + " " + config.getLong("port"));
        System.out.println(config.getOffsetDateTime("started").toInstant());
        System.out.println(config.getLocalDateTime("cutoff") + " " + config.getLocalDate("holiday") + " " + config.getLocalTime("backup"));
        System.out.println(config.getString("database.url"));
        TomlArray workers = config.getArray("worker");
        System.out.println(workers.getTable(0).getString("queue"));
        System.out.println(config.toJson());

        config.set("port", 9090L);
        System.out.print(config.toToml());
    }
}
```

```output
false
billing 8080
2026-11-10T02:30:00Z
2026-11-10T17:00 2026-12-25 02:30
postgres://db.local/billing
invoices
{
  "name" : "billing",
  "port" : 8080,
  "started" : "2026-11-10T09:30:00+07:00",
  "cutoff" : "2026-11-10T17:00:00",
  "holiday" : "2026-12-25",
  "backup" : "02:30:00",
  "database" : {
    "url" : "postgres://db.local/billing"
  },
  "worker" : [{
    "queue" : "invoices"
  }]
}

# Billing service
name = "billing"
port = 9090 # the public port
started = 2026-11-10T09:30:00+07:00
cutoff = 2026-11-10T17:00:00
holiday = 2026-12-25
backup = 02:30:00

[database]
url = 'postgres://db.local/billing'

[[worker]]
queue = "invoices"
```

```java run id=errors deps=deps/java/pom.xml
import org.tomlj.Toml;
import org.tomlj.TomlParseResult;
import org.tomlj.TomlVersion;

public class Main {
    public static void main(String[] args) {
        TomlParseResult bad = Toml.parse("[fruit]\napple = 1\n[fruit]\npear = 2\nlimit = 9223372036854775808\n");
        System.out.println(bad.hasErrors());
        bad.errors().forEach(error -> System.out.println(error));

        String newer = "point = {\n  x = 1,\n  y = 2,\n}\n";
        System.out.println(Toml.parse(newer).errors().size());
        Toml.parse(newer, TomlVersion.V1_0_0).errors().forEach(error -> System.out.println(error));
    }
}
```

```output
true
fruit previously defined at line 1, column 1 (line 3, column 1)
Integer is too large (line 5, column 9)
0
Newlines are not allowed in an inline table (TOML versions before 1.1.0) (line 1, column 10)
```

- Verdict: library. Java SE has no TOML API. The examples use TomlJ 2.2.0.
- `Toml.parse` never throws on bad input. It returns a `TomlParseResult` that holds the table and a list of `TomlParseError` values, each with a line and a column. One parse can report more than one error.
- The getters take a dotted key, as in `getString("database.url")`. The date and time types map to `OffsetDateTime`, `LocalDateTime`, `LocalDate` and `LocalTime` from `java.time`.
- `toToml` writes the table again, and it keeps the comments and the literal strings of the input. After `set("port", 9090L)`, the comment at the end of the line stays. `toJson` writes the same data as JSON.
- An integer outside the 64-bit range is an error, as TOML 1.0.0 requires.
- TomlJ parses TOML 1.1.0 by default. `TomlVersion.V1_0_0` rejects the forms that 1.1.0 added, such as an inline table over several lines.
