```java run id=todo
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Item(String title, boolean done) {}

    static final class TodoModel {
        private final List<Item> items = new ArrayList<>();
        private final List<Runnable> observers = new ArrayList<>();

        void subscribe(Runnable observer) { observers.add(observer); }
        List<Item> items() { return List.copyOf(items); }

        void add(String title) {
            if (title.isEmpty()) throw new IllegalArgumentException("a title is required");
            items.add(new Item(title, false));
            changed();
        }

        void complete(int number) {
            if (number < 1 || number > items.size()) throw new IllegalArgumentException("no item " + number);
            items.set(number - 1, new Item(items.get(number - 1).title(), true));
            changed();
        }

        private void changed() { observers.forEach(Runnable::run); }
    }

    static final class ListView {
        ListView(TodoModel model) {
            model.subscribe(() -> {
                List<Item> items = model.items();
                for (int i = 0; i < items.size(); i++) {
                    Item item = items.get(i);
                    System.out.println("  [" + (item.done() ? "x" : " ") + "] " + (i + 1) + " " + item.title());
                }
            });
        }
    }

    static final class SummaryView {
        SummaryView(TodoModel model) {
            model.subscribe(() -> {
                List<Item> items = model.items();
                long done = items.stream().filter(Item::done).count();
                System.out.println("  summary: " + (items.size() - done) + " open, " + done + " done");
            });
        }
    }

    static final class Controller {
        private final TodoModel model;
        Controller(TodoModel model) { this.model = model; }

        void handle(String line) {
            System.out.println("> " + line);
            String[] parts = line.split(" ", 2);
            String argument = parts.length > 1 ? parts[1] : "";
            try {
                if (parts[0].equals("add")) model.add(argument);
                else if (parts[0].equals("done") && argument.matches("\\d+")) model.complete(Integer.parseInt(argument));
                else System.out.println("  error: cannot read '" + line + "'");
            } catch (IllegalArgumentException err) {
                System.out.println("  error: " + err.getMessage());
            }
        }
    }

    public static void main(String[] args) {
        TodoModel model = new TodoModel();
        new ListView(model);
        new SummaryView(model);
        Controller controller = new Controller(model);
        for (String line : List.of("add buy milk", "add write report", "done 1", "done 7", "rename 2")) {
            controller.handle(line);
        }
    }
}
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. Each view subscribes a `Runnable` to the model in its constructor. The model holds `Runnable`s, so it has no dependency on any view class.
- `items()` returns `List.copyOf`, an unmodifiable copy, so a view cannot change the model.
- Swing follows the same split: a `JTable` view shows a `TableModel` and listens for its events. That library is part of the JDK but needs a display, so this program uses text.
