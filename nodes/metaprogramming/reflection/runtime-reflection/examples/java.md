```java run id=inspect
import java.lang.reflect.Method;
import java.lang.reflect.RecordComponent;

public class Main {
    record Point(int x, int y) {
        int sum() {
            return x + y;
        }
    }

    public static void main(String[] args) throws Exception {
        Object p = new Point(3, 4);
        Class<?> c = p.getClass();
        System.out.println("class: " + c.getSimpleName());
        for (RecordComponent rc : c.getRecordComponents()) {
            Object value = rc.getAccessor().invoke(p);
            System.out.println(rc.getName() + " : " + rc.getType() + " = " + value);
        }
        Method sum = c.getDeclaredMethod("sum");
        System.out.println("sum() = " + sum.invoke(p));
    }
}
```

```output for=inspect
class: Point
x : int = 3
y : int = 4
sum() = 7
```

The program never names `x`, `y` or `sum` in a call that the compiler checks. It finds the components and the method at run time, from the `Class` object of the value. `getRecordComponents` returns the components in the order of declaration. `getDeclaredFields` and `getDeclaredMethods` promise no order.

```java fails expect=exception:NoSuchMethodException id=typo
public class Main {
    record Point(int x, int y) {
        int sum() {
            return x + y;
        }
    }

    public static void main(String[] args) throws Exception {
        Object p = new Point(3, 4);
        System.out.println(p.getClass().getDeclaredMethod("summ").invoke(p));
    }
}
```

The name of the method is a string, so the compiler cannot check it. The typo `summ` compiles, and the program fails when it runs. A direct call `((Point) p).summ()` would not compile.

```java fails expect=exception:InaccessibleObjectException id=encapsulation
import java.lang.reflect.Field;

public class Main {
    public static void main(String[] args) throws Exception {
        Field value = String.class.getDeclaredField("value");
        value.setAccessible(true);
        System.out.println(value.get("abc"));
    }
}
```

`setAccessible(true)` turns off the access checks for a private member, but only if the package of the member is open to the caller. `java.lang` in the module `java.base` is not open to the unnamed module of this program, so the call throws.

```java run id=private-own
import java.lang.reflect.Field;

public class Main {
    static class Account {
        private int balance = 100;
    }

    public static void main(String[] args) throws Exception {
        Account a = new Account();
        Field f = Account.class.getDeclaredField("balance");
        f.setAccessible(true);
        f.setInt(a, -5);
        System.out.println("balance = " + a.balance);
    }
}
```

```output for=private-own
balance = -5
```

Within its own module, a program can open and change a private field. The class has no method that sets a negative balance, but reflection goes around it. This is the "exposure of internals" that the Java tutorial warns about.

- Verdict: native. `java.lang.Class` and the package `java.lang.reflect` describe classes, fields, methods, constructors and record components at run time, and they can read fields, set fields and call methods.
- Access control applies to reflection too. `setAccessible` can suppress it only for members of the caller's own module or of packages that are open to the caller.
- `Method.invoke` wraps an exception from the called method in `InvocationTargetException`.
