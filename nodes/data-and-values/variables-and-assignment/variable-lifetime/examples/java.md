```java run id=kinds-of-variables
public class Main {
    static int created;
    int id;

    Main() {
        created++;
        id = created;
    }

    public static void main(String[] args) {
        for (int i = 0; i < 3; i++) {
            Main m = new Main();
            System.out.println(m.id);
        }
        System.out.println(created);
    }
}
```

```output for=kinds-of-variables
1
2
3
3
```

- Verdict: native. `created` is a class variable: it lives while the class is loaded and counts all the objects. `id` is an instance variable: each object has its own, which lives as long as the object. `m` and `i` are local variables of the loop, and each pass gets a new `m`.
