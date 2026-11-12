```java run id=processor
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

interface Command {
    String name();
    void execute();
    void undo();
}

class Insert implements Command {
    private final List<String> words;
    private final String word;

    Insert(List<String> words, String word) {
        this.words = words;
        this.word = word;
    }

    public String name() { return "insert " + word; }
    public void execute() { words.add(word); }
    public void undo() { words.remove(words.size() - 1); }
}

class Processor {
    private final Deque<Command> done = new ArrayDeque<>();

    void doIt(Command command) {
        System.out.println("do: " + command.name());
        command.execute();
        done.push(command);
    }

    void undoIt() {
        Command command = done.pop();
        System.out.println("undo: " + command.name());
        command.undo();
    }
}

public class Main {
    public static void main(String[] args) {
        List<String> words = new ArrayList<>();
        Processor processor = new Processor();
        processor.doIt(new Insert(words, "hello"));
        processor.doIt(new Insert(words, "world"));
        System.out.println("text: " + String.join(" ", words));
        processor.undoIt();
        System.out.println("text: " + String.join(" ", words));
    }
}
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. `Command` is an interface, and the processor keeps the executed commands in an `ArrayDeque` used as a stack.
