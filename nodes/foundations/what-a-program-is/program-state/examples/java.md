```java run id=enum-state-machine
public class Main {
    enum State { IDLE, RUNNING, PAUSED, DONE }

    static State next(State s, String event) {
        return switch (s) {
            case IDLE -> event.equals("start") ? State.RUNNING : fail(s, event);
            case RUNNING -> switch (event) {
                case "pause" -> State.PAUSED;
                case "finish" -> State.DONE;
                default -> fail(s, event);
            };
            case PAUSED -> event.equals("resume") ? State.RUNNING : fail(s, event);
            case DONE -> fail(s, event);
        };
    }

    static State fail(State s, String event) {
        throw new IllegalStateException(event + " is not allowed in " + s);
    }

    public static void main(String[] args) {
        State s = State.IDLE;
        for (String e : new String[] {"start", "pause", "resume", "finish", "start"}) {
            try {
                State t = next(s, e);
                System.out.println(s + " --" + e + "--> " + t);
                s = t;
            } catch (IllegalStateException ex) {
                System.out.println("rejected: " + ex.getMessage());
            }
        }
    }
}
```

```output for=enum-state-machine
IDLE --start--> RUNNING
RUNNING --pause--> PAUSED
PAUSED --resume--> RUNNING
RUNNING --finish--> DONE
rejected: start is not allowed in DONE
```

- Verdict: native. An enum names the modes of an object, and a `switch` expression gives the next mode for each pair of mode and event. The compiler checks that the `switch` covers every enum constant, and the method throws `IllegalStateException` for an event that is not valid in the current mode. Sealed interfaces with records give each mode its own data, in the style of a sum type.
