```java run id=settings
public class Main {
    static final class Settings {
        private volatile String level = "info";

        private Settings() {
            System.out.println("creating the settings");
        }

        // The JVM initializes Holder once, on the first call to get().
        private static final class Holder {
            static final Settings INSTANCE = new Settings();
        }

        static Settings get() {
            return Holder.INSTANCE;
        }

        String level() {
            return level;
        }

        void setLevel(String level) {
            this.level = level;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Settings first = Settings.get();
        System.out.println("main: " + first.level());
        Settings[] seen = new Settings[1];
        Thread worker = new Thread(() -> {
            Settings settings = Settings.get();
            System.out.println("worker sets debug");
            settings.setLevel("debug");
            seen[0] = settings;
        });
        worker.start();
        worker.join();
        System.out.println("main: " + Settings.get().level());
        System.out.println("same instance: " + (seen[0] == first));
    }
}
```

```output
creating the settings
main: info
worker sets debug
main: debug
same instance: true
```

- Verdict: idiom. The holder class idiom: the class `Holder` is initialized the first time code reads `Holder.INSTANCE`, and the JVM runs that initialization once, under a lock. It needs no explicit locking.
- An `enum` with one constant, `enum Settings { INSTANCE; ... }`, is the other usual form. It also resists reflection and serialization, as the node page shows.
- The field `level` is `volatile`, so a change by one thread is visible to the others. The single instance does not make its own state thread-safe.
