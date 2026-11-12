```java run id=static-and-dynamic-binding
import java.util.ArrayList;
import java.util.List;
import java.util.function.Supplier;

public class Main {
    static class Animal {
        String kind = "animal";
        String sound() { return "..."; }
        static String group() { return "Animal"; }
    }

    static class Dog extends Animal {
        String kind = "dog";               // hides the field; it does not override it
        @Override String sound() { return "woof"; }
        static String group() { return "Dog"; }
    }

    public static void main(String[] args) {
        Animal a = new Dog();
        // A method call binds at run time to the method of the object's class.
        // A field access and a static method bind at compile time to the declared type.
        System.out.println(a.sound() + " " + a.kind + " " + a.group());

        // final fixes the binding; the list itself can still change.
        final List<Integer> xs = new ArrayList<>();
        xs.add(1);
        System.out.println(xs);

        // A lambda can use a local variable only if the variable is never rebound.
        int base = 10;
        Supplier<Integer> s = () -> base + 1;
        System.out.println(s.get());
        // base = 11;  // with this line, the lambda does not compile
    }
}
```

```output for=static-and-dynamic-binding
woof animal Animal
[1]
11
```

- Verdict: native. A call of an instance method binds at run time to the method of the class of the object, so `a.sound()` calls the method of `Dog`. A field access and a call of a static method bind at compile time to the declared type, so `a.kind` reads the field of `Animal`. `final` fixes the binding but not the object. A lambda can use a local variable only if the variable is final or effectively final.
