```java run id=thunks
import java.util.function.Supplier;

public class Main {
    static final class Memo<T> implements Supplier<T> {
        private Supplier<T> compute;
        private T value;

        Memo(Supplier<T> compute) {
            this.compute = compute;
        }

        @Override
        public T get() {
            if (compute != null) {
                value = compute.get();
                compute = null;
            }
            return value;
        }
    }

    static int load() {
        System.out.println("loading");
        return 21;
    }

    public static void main(String[] args) {
        Supplier<Integer> plain = Main::load;
        Supplier<Integer> memo = new Memo<>(Main::load);
        System.out.println(plain.get() + plain.get());
        System.out.println(memo.get() + memo.get());
    }
}
```

```output for=thunks
loading
loading
42
loading
42
```

- Verdict: stdlib. `Supplier<T>` is the standard plain thunk, and its documentation does not promise a stored result. `Memo` adds the stored result. It is not safe for threads; a shared memoized value needs a lock or the holder class idiom.
