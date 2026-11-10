```java run id=scalar-types deps=deps/java/pom.xml
import java.util.Date;
import java.util.LinkedHashMap;
import java.util.Map;
import org.snakeyaml.engine.v2.api.Dump;
import org.snakeyaml.engine.v2.api.DumpSettings;
import org.snakeyaml.engine.v2.api.Load;
import org.snakeyaml.engine.v2.api.LoadSettings;
import org.snakeyaml.engine.v2.schema.CoreSchema;
import org.yaml.snakeyaml.Yaml;

public class Main {
    static void show(String label, Object tree) {
        StringBuilder line = new StringBuilder(label);
        ((Map<?, ?>) tree).forEach((key, value) -> line.append(" ").append(key).append("=")
                .append(value.getClass().getSimpleName()).append(":")
                .append(value instanceof Date ? "(a date)" : value));
        System.out.println(line);
    }

    public static void main(String[] args) {
        String text = "country: NO\nenabled: on\nmode: 010\nsize: 1_000\nport: 12:30\nversion: 1.10\nday: 2026-11-10\n";

        show("SnakeYAML  ", new Yaml().load(text));
        show("Engine     ", new Load(LoadSettings.builder().build()).loadFromString(text));
        show("Engine core", new Load(LoadSettings.builder().setSchema(new CoreSchema()).build()).loadFromString(text));

        Map<String, Object> strings = new LinkedHashMap<>();
        strings.put("country", "NO");
        strings.put("size", "1_000");
        strings.put("port", "12:30");
        System.out.print(new Yaml().dump(strings));
        System.out.print(new Dump(DumpSettings.builder().build()).dumpToString(strings));
    }
}
```

```output for=scalar-types
SnakeYAML   country=Boolean:false enabled=Boolean:true mode=Integer:8 size=Integer:1000 port=Integer:750 version=Double:1.1 day=Date:(a date)
Engine      country=String:NO enabled=String:on mode=String:010 size=String:1_000 port=String:12:30 version=Double:1.1 day=String:2026-11-10
Engine core country=String:NO enabled=String:on mode=Integer:10 size=String:1_000 port=String:12:30 version=Double:1.1 day=String:2026-11-10
{country: 'NO', size: '1_000', port: '12:30'}
{country: NO, size: 1_000, port: '12:30'}
```

```java run id=untrusted deps=deps/java/pom.xml
import java.util.Collections;
import java.util.Map;
import java.util.function.Supplier;
import java.util.logging.LogManager;
import org.snakeyaml.engine.v2.api.Load;
import org.snakeyaml.engine.v2.api.LoadSettings;
import org.yaml.snakeyaml.LoaderOptions;
import org.yaml.snakeyaml.Yaml;

public class Main {
    static void attempt(String label, Supplier<Object> load) {
        try {
            System.out.println(label + ": " + load.get());
        } catch (RuntimeException error) {
            String message = error.getMessage();
            String reason = message.lines().filter(line -> line.contains("duplicate")).findFirst()
                    .orElse(message.lines().findFirst().orElse(""));
            System.out.println(label + ": " + error.getClass().getSimpleName() + ": " + reason.trim());
        }
    }

    public static void main(String[] args) {
        LogManager.getLogManager().reset();

        String repeated = "a: 1\na: 2\n";
        attempt("SnakeYAML", () -> new Yaml().load(repeated));
        LoaderOptions strict = new LoaderOptions();
        strict.setAllowDuplicateKeys(false);
        attempt("SnakeYAML strict", () -> new Yaml(strict).load(repeated));
        attempt("Engine", () -> new Load(LoadSettings.builder().build()).loadFromString(repeated));

        attempt("SnakeYAML tag", () -> new Yaml().load("!!java.io.File [/tmp]"));

        StringBuilder bomb = new StringBuilder("a: &a [x, x, x, x, x, x, x, x, x]\n");
        for (char name = 'b'; name <= 'g'; name++) {
            String alias = "*" + (char) (name - 1);
            bomb.append(name).append(": &").append(name).append(" [")
                    .append(String.join(", ", Collections.nCopies(9, alias))).append("]\n");
        }
        String text = bomb.toString();
        attempt("SnakeYAML aliases", () -> ((Map<?, ?>) new Yaml().load(text)).size());
        attempt("Engine aliases", () -> ((Map<?, ?>) new Load(LoadSettings.builder().build()).loadFromString(text)).size());
    }
}
```

```output for=untrusted
SnakeYAML: {a=2}
SnakeYAML strict: DuplicateKeyException: found duplicate key a
Engine: DuplicateKeyException: found duplicate key a
SnakeYAML tag: ComposerException: Global tag is not allowed: tag:yaml.org,2002:java.io.File
SnakeYAML aliases: YAMLException: Number of aliases for non-scalar nodes exceeds the specified max=50
Engine aliases: YamlEngineException: Number of aliases for non-scalar nodes exceeds the specified max=50
```

- Verdict: library. Java SE has no YAML API. The examples use SnakeYAML 2.7, a YAML 1.1 processor, and SnakeYAML Engine 3.2, a YAML 1.2 processor. Kotlin on the JVM can use the same libraries.
- `new Yaml().load(text)` returns maps, lists and scalars. SnakeYAML resolves plain scalars with the 1.1 rules, so `NO` is `false`, `010` is 8, `12:30` is 750 and `2026-11-10` is a `java.util.Date`.
- `new Load(settings).loadFromString(text)` is the Engine API. With the default settings in this example, the Engine reads `010`, `1_000` and `12:30` as strings. With `setSchema(new CoreSchema())`, it reads `010` as the integer 10, as the core schema says.
- `Yaml.dump` quotes `'NO'`, `'1_000'` and `'12:30'`, because a 1.1 reader needs the quotes. `Dump.dumpToString` writes `NO` and `1_000` with no quotes, which is correct for 1.2 and wrong for a 1.1 reader.
- With the default options, SnakeYAML keeps the last of two repeated keys. `LoaderOptions.setAllowDuplicateKeys(false)` makes it throw `DuplicateKeyException`. The Engine throws by default.
- SnakeYAML 2.0 and later reject global tags such as `!!java.io.File` by default. A `TagInspector` in `LoaderOptions` allows chosen tags. The Engine builds no custom objects.
- Both libraries limit the aliases to collections. In the example, the limit of 50 stops a document that would expand to millions of nodes. `LoaderOptions.setMaxAliasesForCollections` and `LoadSettings.builder().setMaxAliasesForCollections` change it.
