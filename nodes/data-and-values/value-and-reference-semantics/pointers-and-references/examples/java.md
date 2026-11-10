```java run id=change
import java.util.ArrayList;
import java.util.List;

public class Main {
    static void rebind(List<Integer> items) {
        items = new ArrayList<>(List.of(0, 0, 0));
    }

    static void append(List<Integer> items) {
        items.add(4);
    }

    public static void main(String[] args) {
        List<Integer> numbers = new ArrayList<>(List.of(1, 2, 3));
        rebind(numbers);
        System.out.println("after rebind: " + numbers);
        append(numbers);
        System.out.println("after append: " + numbers);

        List<Integer> alias = numbers;
        alias.set(0, 100);
        System.out.println("numbers: " + numbers);
        System.out.println("alias == numbers: " + (alias == numbers));

        List<Integer> copy = new ArrayList<>(numbers);
        System.out.println("copy == numbers: " + (copy == numbers));
        System.out.println("copy.equals(numbers): " + copy.equals(numbers));
    }
}
```

```output for=change
after rebind: [1, 2, 3]
after append: [1, 2, 3, 4]
numbers: [100, 2, 3, 4]
alias == numbers: true
copy == numbers: false
copy.equals(numbers): true
```

- Verdict: native. A variable of a class, interface or array type holds a reference to an object, or the null reference. Primitive types such as `int` hold the value itself.
- A call initializes each parameter with the value of the argument. For an object, this value is the reference, so `rebind` changes only its own parameter.
- `==` on two references is true when they refer to the same object. `equals` compares the contents.
- Java has no pointer arithmetic, no address operator and no call by reference.
