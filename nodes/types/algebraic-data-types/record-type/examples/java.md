```java run id=record
public class Main {
    record Person(String name, int age) {
        Person withAge(int newAge) {
            return new Person(name, newAge);
        }
    }

    static String yn(boolean b) {
        return b ? "yes" : "no";
    }

    public static void main(String[] args) {
        Person ada = new Person("Ada", 36);
        System.out.println("name: " + ada.name() + " age: " + ada.age());

        Person older = ada.withAge(ada.age() + 1);
        System.out.println("copy: " + older);
        System.out.println("original: " + ada);
        System.out.println("same fields equal: " + yn(ada.equals(new Person("Ada", 36))));
    }
}
```

```output for=record
name: Ada age: 36
copy: Person[name=Ada, age=37]
original: Person[name=Ada, age=36]
same fields equal: yes
```

- Verdict: native. A `record` gets a private final field and a public accessor for each component, and generated `equals`, `hashCode` and `toString`.
- Java has no copy-with-change syntax, so the record declares a `withAge` method that calls the constructor.
- The fields are final, so a record cannot change after construction.
