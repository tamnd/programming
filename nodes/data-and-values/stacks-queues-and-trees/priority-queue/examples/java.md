```java run id=pq
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

public class Main {
    record Task(int priority, String name) {}

    public static void main(String[] args) {
        PriorityQueue<Task> tasks = new PriorityQueue<>(Comparator.comparingInt(Task::priority));
        tasks.add(new Task(3, "update docs"));
        tasks.add(new Task(1, "fix outage"));
        tasks.add(new Task(2, "review patch"));
        tasks.add(new Task(4, "plan sprint"));

        System.out.println("peek: " + tasks.peek().name());
        while (!tasks.isEmpty()) {
            Task task = tasks.poll();
            System.out.println(task.priority() + " " + task.name());
        }

        PriorityQueue<Integer> maxHeap = new PriorityQueue<>(Comparator.reverseOrder());
        maxHeap.addAll(List.of(3, 1, 2, 4));
        StringBuilder order = new StringBuilder();
        while (!maxHeap.isEmpty()) {
            order.append(" ").append(maxHeap.poll());
        }
        System.out.println("largest first:" + order);
    }
}
```

```output for=pq
peek: fix outage
1 fix outage
2 review patch
3 update docs
4 plan sprint
largest first: 4 3 2 1
```

- Verdict: stdlib. `PriorityQueue` with a `Comparator` orders the records by priority. `peek` reads the head and `poll` removes it.
- `Comparator.reverseOrder()` makes a max-heap.
