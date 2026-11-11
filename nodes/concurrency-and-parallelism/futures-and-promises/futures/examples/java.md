```java run id=completable
import java.util.concurrent.CompletableFuture;

public class Main {
    public static void main(String[] args) {
        CompletableFuture<Integer> price = CompletableFuture.supplyAsync(() -> 40);
        CompletableFuture<Integer> tax = CompletableFuture.supplyAsync(() -> 2);
        int total = price.thenCombine(tax, Integer::sum).join();
        System.out.println(total);

        CompletableFuture<String> manual = new CompletableFuture<>();
        new Thread(() -> manual.complete("done")).start();
        System.out.println(manual.join());

        String recovered = CompletableFuture.<String>supplyAsync(() -> {
            throw new IllegalStateException("no stock");
        }).exceptionally(e -> "fallback: " + e.getCause().getMessage()).join();
        System.out.println(recovered);
    }
}
```

```output for=completable
42
done
fallback: no stock
```

- Verdict: stdlib. `Future` is the read side: `get` waits for the result. `CompletableFuture` is also the write side: any thread can call `complete` or `completeExceptionally` once.
- `thenApply`, `thenCompose` and `thenCombine` make new futures from old ones without a wait. `join` waits and wraps a failure in an unchecked `CompletionException`.
