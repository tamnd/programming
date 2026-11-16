```java run id=override-tostring
public class Main {
    record Book(String title, int year) {}

    static final class Film {
        final String title;
        final int year;

        Film(String title, int year) {
            this.title = title;
            this.year = year;
        }

        @Override
        public String toString() {
            return title + " (" + year + ")";
        }
    }

    static final class Plain {}

    public static void main(String[] args) {
        System.out.println(new Book("Dune", 1965));
        System.out.println(new Film("Alien", 1979));
        System.out.println("Watch " + new Film("Alien", 1979));
        String s = new Plain().toString();
        System.out.println(s.startsWith("Main$Plain@"));
    }
}
```

```output for=override-tostring
Book[title=Dune, year=1965]
Alien (1979)
Watch Alien (1979)
true
```

- Verdict: native. `Object.toString` gives every object a string form, and `println`, string concatenation and `String.valueOf` call it.
- A record generates `toString` from its components. A class keeps the default of `Object`, the class name, `@` and the hash code in hexadecimal, until it overrides the method.
- Java has one string form, so a class must choose between a form for users and a form for debugging.
