```java run id=player
public class Main {
    record Transition(State next, String action) {}

    enum State {
        STOPPED {
            @Override
            Transition play() {
                return new Transition(PLAYING, "start at 0:00");
            }
        },
        PLAYING {
            @Override
            Transition pause() {
                return new Transition(PAUSED, "hold the position");
            }

            @Override
            Transition stop() {
                return new Transition(STOPPED, "rewind");
            }
        },
        PAUSED {
            @Override
            Transition play() {
                return new Transition(PLAYING, "resume");
            }

            @Override
            Transition stop() {
                return new Transition(STOPPED, "rewind");
            }
        };

        // The defaults: each event is ignored unless a constant overrides it.
        Transition play() {
            return new Transition(this, "ignored");
        }

        Transition pause() {
            return new Transition(this, "ignored");
        }

        Transition stop() {
            return new Transition(this, "ignored");
        }
    }

    static final class Player {
        private State state = State.STOPPED;

        void play() {
            change("play", state.play());
        }

        void pause() {
            change("pause", state.pause());
        }

        void stop() {
            change("stop", state.stop());
        }

        private void change(String event, Transition t) {
            System.out.printf("%s: %s (%s -> %s)%n", event, t.action(), name(state), name(t.next()));
            state = t.next();
        }

        private static String name(State s) {
            return s.name().toLowerCase();
        }
    }

    public static void main(String[] args) {
        Player player = new Player();
        player.play();
        player.pause();
        player.play();
        player.stop();
        player.pause();
    }
}
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. An enum constant can have a class body, and the body can override the methods of the enum. Each constant is then a state object with its own behaviour.
- The enum has exactly one object for each constant, so the state objects are shared, and `==` compares them.
- The methods in the class bodies refer to the other constants. This is allowed, because a method body runs after the class has made all its constants.
- A `switch` over the enum is the other usual form. A `switch` expression that has a case for each constant is exhaustive and needs no `default`.
