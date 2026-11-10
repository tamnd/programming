```java run id=editor
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
    static final class Buffer {
        final StringBuilder text = new StringBuilder();
    }

    interface Command {
        void execute();

        void undo();
    }

    record Append(Buffer buffer, String text) implements Command {
        public void execute() {
            buffer.text.append(text);
        }

        public void undo() {
            buffer.text.setLength(buffer.text.length() - text.length());
        }
    }

    static final class Delete implements Command {
        private final Buffer buffer;
        private final int count;
        private String removed = "";

        Delete(Buffer buffer, int count) {
            this.buffer = buffer;
            this.count = count;
        }

        public void execute() {
            int start = buffer.text.length() - count;
            removed = buffer.text.substring(start);
            buffer.text.setLength(start);
        }

        public void undo() {
            buffer.text.append(removed);
        }
    }

    static final class History {
        private final Deque<Command> undoStack = new ArrayDeque<>();
        private final Deque<Command> redoStack = new ArrayDeque<>();

        void run(Command command) {
            command.execute();
            undoStack.push(command);
            redoStack.clear();
        }

        boolean undo() {
            Command command = undoStack.poll();
            if (command == null) return false;
            command.undo();
            redoStack.push(command);
            return true;
        }

        boolean redo() {
            Command command = redoStack.poll();
            if (command == null) return false;
            command.execute();
            undoStack.push(command);
            return true;
        }
    }

    public static void main(String[] args) {
        Buffer buffer = new Buffer();
        History history = new History();
        history.run(new Append(buffer, "hello")); print("append", buffer);
        history.run(new Append(buffer, " world")); print("append", buffer);
        history.run(new Delete(buffer, 5)); print("delete", buffer);
        history.undo(); print("undo", buffer);
        history.undo(); print("undo", buffer);
        history.redo(); print("redo", buffer);
        history.run(new Append(buffer, "!")); print("append", buffer);
        if (!history.redo()) System.out.println("redo: nothing to redo");
    }

    static void print(String label, Buffer buffer) {
        System.out.println(label + ": \"" + buffer.text + "\"");
    }
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. An interface with `execute` and `undo`. A record for the command whose state does not change. A class for the command that saves the removed text when it runs.
- `ArrayDeque` serves as both stacks. `push` and `poll` work at the head, and `poll` returns `null` on an empty deque.
- A command with no undo is a `Runnable`, and a lambda can implement it. Undo needs a second method, so the command needs its own interface or a pair of lambdas.
- `javax.swing.undo.UndoManager` in the standard library keeps the edits in one list with an index, and trims the edits after the index when a fresh edit arrives.
