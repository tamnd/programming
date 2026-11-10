```java run id=undo
import java.util.ArrayDeque;
import java.util.Deque;

public class Main {
    /** The originator. Only it reads and writes the fields of its mementos. */
    static final class Editor {
        /** The memento. Its fields and constructor are private, but Editor encloses it. */
        static final class Memento {
            private final String text;
            private final int cursor;

            private Memento(String text, int cursor) {
                this.text = text;
                this.cursor = cursor;
            }
        }

        private String text = "";
        private int cursor = 0;

        void type(String chars) {
            text = text.substring(0, cursor) + chars + text.substring(cursor);
            cursor += chars.length();
        }

        void moveTo(int position) {
            cursor = Math.max(0, Math.min(position, text.length()));
        }

        Memento save() {
            return new Memento(text, cursor);
        }

        void restore(Memento memento) {
            text = memento.text;
            cursor = memento.cursor;
        }

        String show() {
            return text.substring(0, cursor) + "|" + text.substring(cursor);
        }
    }

    /** The caretaker. It keeps mementos and cannot read them. */
    static final class History {
        private final Editor editor;
        private final Deque<Editor.Memento> undo = new ArrayDeque<>();

        History(Editor editor) {
            this.editor = editor;
        }

        void save() {
            undo.push(editor.save());
        }

        boolean undo() {
            Editor.Memento memento = undo.poll();
            if (memento == null) return false;
            editor.restore(memento);
            return true;
        }
    }

    public static void main(String[] args) {
        Editor editor = new Editor();
        History history = new History(editor);

        history.save();
        editor.type("Hello");
        System.out.println(editor.show());
        history.save();
        editor.type(" world");
        System.out.println(editor.show());
        history.save();
        editor.moveTo(5);
        editor.type(",");
        System.out.println(editor.show());

        for (int i = 0; i < 4; i++) {
            System.out.println(history.undo() ? "undo: " + editor.show() : "nothing to undo");
        }
    }
}
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. A static nested class with `private` fields and a `private` constructor. Java allows access to a `private` member from anywhere in the body of the top level class that encloses it. Here that class is `Main`, so `History` could read the fields too. In a real program, `Editor` is a top level class in its own file, and only `Editor` can read them.
- `String` is immutable, so the memento stores the reference and needs no copy.
- A `record` is a poor memento: its components have public accessor methods, so the caretaker could read the state.
- `ArrayDeque.push` and `poll` give a stack. `poll` returns `null` on an empty deque, where `pop` would throw.
