```java run id=consumer-words
import java.util.ArrayDeque;
import java.util.Deque;
import java.util.function.Consumer;

public class Main {
    // A word changes the stack. then() puts one word after another.
    interface Word extends Consumer<Deque<Integer>> {
        default Word then(Word next) {
            return s -> { accept(s); next.accept(s); };
        }
    }

    static Word push(int n) { return s -> s.push(n); }

    static final Word DUP = s -> s.push(s.peek());
    static final Word ADD = s -> s.push(s.pop() + s.pop());
    static final Word MUL = s -> s.push(s.pop() * s.pop());
    static final Word SWAP = s -> { int b = s.pop(), a = s.pop(); s.push(b); s.push(a); };

    public static void main(String[] args) {
        Word square = DUP.then(MUL);
        Word sumOfSquares = square.then(SWAP).then(square).then(ADD);

        Deque<Integer> stack = new ArrayDeque<>();
        push(3).then(push(4)).then(sumOfSquares).accept(stack);
        System.out.println(stack);
    }
}
```

```output for=consumer-words
[25]
```

- Verdict: absent. Java applies methods to arguments. `Consumer.andThen` and `Function.andThen` compose functions, and the `then` method above builds programs from words in the same way. The JVM is a stack machine: its bytecode, such as `iload`, `imul` and `iadd`, is a concatenative program, and the verifier checks the stack effect of each method, the number and the types of the values that it takes and leaves.
