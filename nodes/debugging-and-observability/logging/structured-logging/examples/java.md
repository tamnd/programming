```java run id=records
import java.util.Locale;
import java.util.logging.Formatter;
import java.util.logging.Handler;
import java.util.logging.Level;
import java.util.logging.LogRecord;
import java.util.logging.Logger;
import java.util.logging.StreamHandler;

public class Main {
    // Writes the level, the logger and the message, with no timestamp.
    static class KeyValueFormatter extends Formatter {
        @Override
        public String format(LogRecord r) {
            return "level=" + r.getLevel().getName() + " logger=" + r.getLoggerName()
                + " msg=\"" + formatMessage(r) + "\"" + System.lineSeparator();
        }
    }

    public static void main(String[] args) {
        Locale.setDefault(Locale.US); // MessageFormat formats numbers by locale
        // Replace the default ConsoleHandler, which writes to System.err with a timestamp.
        Logger root = Logger.getLogger("");
        for (Handler h : root.getHandlers()) {
            root.removeHandler(h);
        }
        Handler out = new StreamHandler(System.out, new KeyValueFormatter());
        out.setLevel(Level.ALL);
        root.addHandler(out);
        root.setLevel(Level.FINE);

        // java.util.logging directly
        Logger jul = Logger.getLogger("shop");
        jul.finer("cart loaded"); // below FINE: dropped
        jul.log(Level.INFO, "order placed order={0} total_cents={1}", new Object[] {1042, 1999});
        jul.log(Level.INFO, "order placed order={0,number,#} total_cents={1,number,#}", new Object[] {1042, 1999});

        // System.Logger, the platform API; here it forwards to java.util.logging
        System.Logger sys = System.getLogger("shop.stock");
        sys.log(System.Logger.Level.TRACE, "scan started"); // maps to FINER: dropped
        sys.log(System.Logger.Level.DEBUG, "cache hit sku={0}", "A-17"); // maps to FINE
        sys.log(System.Logger.Level.WARNING, "stock low sku={0} left={1}", "A-17", 2);
        out.flush();
    }
}
```

```output for=records
level=INFO logger=shop msg="order placed order=1,042 total_cents=1,999"
level=INFO logger=shop msg="order placed order=1042 total_cents=1999"
level=FINE logger=shop.stock msg="cache hit sku=A-17"
level=WARNING logger=shop.stock msg="stock low sku=A-17 left=2"
```

- Verdict: stdlib. `java.util.logging` has levels, named loggers, handlers and formatters. `System.Logger` is the platform logging API, and by default it sends records to `java.util.logging`.
- The default `ConsoleHandler` writes to `System.err` with a timestamp, so the program replaces it with a handler that writes to `System.out` with a fixed format.
- `System.Logger` levels map to `java.util.logging` levels: TRACE to FINER and DEBUG to FINE.
- Neither API has key-value pairs. The fields go in the message, and `MessageFormat` formats a number by locale, so `{0}` gives `1,042`. Use `{0,number,#}` for a value that a machine reads.
