```java run id=tokens
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.List;
import java.util.Map;

public class Main {
    static <T> T create(Class<T> type) throws Exception {
        return type.getDeclaredConstructor().newInstance();
    }

    abstract static class TypeRef<T> {
        final Type type;

        TypeRef() {
            ParameterizedType parent = (ParameterizedType) getClass().getGenericSuperclass();
            type = parent.getActualTypeArguments()[0];
        }
    }

    public static void main(String[] args) throws Exception {
        StringBuilder text = create(StringBuilder.class);
        text.append("made from a Class token");
        System.out.println(text);

        Type type = new TypeRef<Map<String, List<Integer>>>() {}.type;
        System.out.println(type);
    }
}
```

```output for=tokens
made from a Class token
java.util.Map<java.lang.String, java.util.List<java.lang.Integer>>
```

- Verdict: native. The compiler erases type arguments, so an object does not know the type arguments that it was created with.
- A `Class<T>` argument is a type token. It carries the type to the run time, so `create` can make an object of that type.
- `new TypeRef<...>() {}` declares an anonymous subclass. Its class file records the type argument of its superclass, and `getGenericSuperclass` reads it back. This is a super type token.
- The type token works only for a type that is written in the source. Inside a generic method, `new TypeRef<T>() {}` records only the type variable `T`.
