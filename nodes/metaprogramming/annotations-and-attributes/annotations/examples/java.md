```java run id=runtime
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.Method;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    @Retention(RetentionPolicy.RUNTIME)
    @Target(ElementType.METHOD)
    @interface Command {
        String value();
        String help() default "";
    }

    public static class Tool {
        @Command(value = "hello", help = "print a greeting")
        public String hello() {
            return "hello, world";
        }

        @Command("time")
        public String time() {
            return "noon";
        }

        public String helper() {
            return "not a command";
        }
    }

    public static void main(String[] args) throws Exception {
        Tool tool = new Tool();
        Map<String, Method> commands = new TreeMap<>();
        for (Method m : Tool.class.getDeclaredMethods()) {
            Command c = m.getAnnotation(Command.class);
            if (c != null) {
                commands.put(c.value(), m);
            }
        }
        for (Map.Entry<String, Method> e : commands.entrySet()) {
            Method m = e.getValue();
            String help = m.getAnnotation(Command.class).help();
            System.out.println(e.getKey() + " -> " + m.invoke(tool) + " [help: \"" + help + "\"]");
        }
    }
}
```

```output for=runtime
hello -> hello, world [help: "print a greeting"]
time -> noon [help: ""]
```

`@interface` declares an annotation interface. `@Retention(RetentionPolicy.RUNTIME)` keeps the annotation in the class file and makes it visible to reflection. The program finds the annotated methods and calls them by the name in the annotation. `helper` has no annotation, so it is not a command. The `TreeMap` sorts the names, because `getDeclaredMethods` returns the methods in no particular order.

```java wrong id=default-retention
import java.lang.annotation.ElementType;
import java.lang.annotation.Target;
import java.lang.reflect.Method;

public class Main {
    @Target(ElementType.METHOD)
    @interface Command {
        String value();
    }

    public static class Tool {
        @Command("hello")
        public String hello() {
            return "hello, world";
        }
    }

    public static void main(String[] args) {
        int found = 0;
        for (Method m : Tool.class.getDeclaredMethods()) {
            if (m.isAnnotationPresent(Command.class)) {
                found++;
            }
        }
        System.out.println("commands found: " + found);
    }
}
```

```output for=default-retention
commands found: 0
```

The annotation has no `@Retention`, so its retention policy is `CLASS`. The compiler writes it into the class file, but the virtual machine need not keep it at run time, and reflection does not see it. The program compiles, runs and finds no commands.

```java fails expect=compile id=override
public class Main {
    static final class Point {
        final int x;
        final int y;

        Point(int x, int y) {
            this.x = x;
            this.y = y;
        }

        @Override
        public boolean equals(Point other) {
            return x == other.x && y == other.y;
        }
    }

    public static void main(String[] args) {
        System.out.println(new Point(1, 2).equals(new Point(1, 2)));
    }
}
```

`equals(Point)` overloads `equals(Object)` and does not override it. Without the annotation, the program compiles, and code that calls `equals(Object)`, such as a `HashSet`, still uses the method inherited from `Object`. With `@Override`, the compiler reports the mistake.

- Verdict: native. An annotation interface declares the elements, `@Target` limits where the annotation can appear, and `@Retention` chooses `SOURCE`, `CLASS` or `RUNTIME`.
- An annotation has no effect at run time by itself. A tool gives it meaning: the compiler for `@Override`, an annotation processor at compile time, or reflection at run time.
- An annotation processor runs inside the compiler and can generate source files. It needs a separate compilation step, so no processor runs on this page.
