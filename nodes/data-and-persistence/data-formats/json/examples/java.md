```java run id=parse-emit deps=deps/java/pom.xml
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.ToNumberPolicy;
import com.google.gson.reflect.TypeToken;
import java.util.List;
import java.util.Map;

public class Main {
    record Order(long id, double total, List<String> tags, boolean paid) {}

    public static void main(String[] args) {
        String text = "{\"id\": 9007199254740993, \"total\": 0.10, \"tags\": [\"new\"], \"paid\": false}";
        Gson gson = new Gson();

        Order order = gson.fromJson(text, Order.class);
        System.out.println(order);
        System.out.println(gson.toJson(order));

        Map<String, Object> loose = gson.fromJson(text, new TypeToken<Map<String, Object>>() {}.getType());
        System.out.println(loose.get("id"));

        Gson exact = new GsonBuilder().setObjectToNumberStrategy(ToNumberPolicy.LONG_OR_DOUBLE).create();
        Map<String, Object> kept = exact.fromJson(text, new TypeToken<Map<String, Object>>() {}.getType());
        System.out.println(kept.get("id"));
    }
}
```

```output for=parse-emit
Order[id=9007199254740993, total=0.1, tags=[new], paid=false]
{"id":9007199254740993,"total":0.1,"tags":["new"],"paid":false}
9.007199254740992E15
9007199254740993
```

```java run id=round-trip deps=deps/java/pom.xml
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonParser;
import com.google.gson.JsonSyntaxException;
import com.google.gson.reflect.TypeToken;
import java.util.Map;

public class Main {
    static class Invoice {
        long total;
        String currency;
    }

    public static void main(String[] args) {
        Gson gson = new Gson();
        String repeated = "{\"total\": 1, \"total\": 2}";

        try {
            gson.fromJson(repeated, new TypeToken<Map<String, Object>>() {}.getType());
        } catch (JsonSyntaxException error) {
            System.out.println(error.getMessage());
        }
        System.out.println(JsonParser.parseString(repeated));

        Invoice empty = gson.fromJson("{}", Invoice.class);
        System.out.println(empty.total + " " + empty.currency);

        System.out.println(gson.toJson("<a href='x'>"));
        System.out.println(new GsonBuilder().disableHtmlEscaping().create().toJson("<a href='x'>"));

        try {
            gson.toJson(Double.NaN);
        } catch (IllegalArgumentException error) {
            System.out.println(error.getClass().getSimpleName());
        }
    }
}
```

```output for=round-trip
duplicate key: total
{"total":2}
0 null
"\u003ca href\u003d\u0027x\u0027\u003e"
"<a href='x'>"
IllegalArgumentException
```

- Verdict: library. Java SE has no JSON API. The examples use Gson 2.10.1. Jackson (`ObjectMapper`) is the other common library, and Jakarta JSON Processing is the API of Jakarta EE.
- `Gson.fromJson` maps text to a class or a record, and `Gson.toJson` writes it. `JsonParser.parseString` gives a tree of `JsonElement` values.
- A `long` field keeps the integer exactly. In a `Map<String, Object>`, Gson makes every number a `Double` by default, so `9007199254740993` loses its last digit. `ToNumberPolicy.LONG_OR_DOUBLE` keeps integers as `Long`.
- A repeated name makes the map adapter throw `duplicate key`. The tree parser keeps the last value.
- A missing member leaves the field at its default, `0` or `null`. Gson does not report it.
- By default Gson escapes `<`, `>`, `=`, `&` and `'` so the output is safe inside HTML. `disableHtmlEscaping()` turns this off.
- `NaN` and the infinities throw, unless the builder calls `serializeSpecialFloatingPointValues()`.
