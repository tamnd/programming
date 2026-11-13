```java run id=event-loop-pattern
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;

public class Main {
    record Timer(int due, int order, Runnable callback) {}

    // One thread takes ready callbacks from a queue and runs each one to the end.
    static class EventLoop {
        final ArrayDeque<Runnable> ready = new ArrayDeque<>(); // callbacks to run in this turn or the next
        final PriorityQueue<Timer> timers =
                new PriorityQueue<>(Comparator.comparingInt(Timer::due).thenComparingInt(Timer::order));
        int now = 0; // a counter clock, so the output does not depend on the machine
        int order = 0;
        int turns = 0;

        void callSoon(Runnable callback) {
            ready.addLast(callback);
        }

        void callLater(int delay, Runnable callback) {
            timers.add(new Timer(now + delay, ++order, callback));
        }

        void run() {
            while (!ready.isEmpty() || !timers.isEmpty()) {
                if (ready.isEmpty()) now = timers.peek().due(); // a real loop sleeps or waits for I/O here
                while (!timers.isEmpty() && timers.peek().due() <= now) ready.addLast(timers.poll().callback());
                turns++;
                int count = ready.size(); // callbacks added now wait for the next turn
                for (int i = 0; i < count; i++) ready.pollFirst().run();
            }
        }
    }

    static final EventLoop loop = new EventLoop();
    static final List<String> log = new ArrayList<>();

    static Runnable say(String text) {
        return () -> log.add("time " + loop.now + ": " + text);
    }

    public static void main(String[] args) {
        loop.callLater(20, say("timer of 20"));
        loop.callLater(10, say("timer of 10"));
        loop.callSoon(() -> {
            say("first callback").run();
            loop.callSoon(say("callback that the first callback added"));
        });
        loop.callSoon(say("second callback"));
        loop.run();

        System.out.println("order of the callbacks:");
        for (String line : log) System.out.println("  " + line);
        System.out.println("turns of the loop: " + loop.turns);
    }
}
```

```output
order of the callbacks:
  time 0: first callback
  time 0: second callback
  time 0: callback that the first callback added
  time 10: timer of 10
  time 20: timer of 20
turns of the loop: 4
```

- Verdict: idiom. A small loop class with an `ArrayDeque` of `Runnable` callbacks and a `PriorityQueue` of timer records.
