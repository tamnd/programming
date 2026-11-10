```python run id=player
class State:
    name = ""

    # The default for each event: stay in this state and do nothing.
    def play(self):
        return self, "ignored"

    def pause(self):
        return self, "ignored"

    def stop(self):
        return self, "ignored"


class Stopped(State):
    name = "stopped"

    def play(self):
        return PLAYING, "start at 0:00"


class Playing(State):
    name = "playing"

    def pause(self):
        return PAUSED, "hold the position"

    def stop(self):
        return STOPPED, "rewind"


class Paused(State):
    name = "paused"

    def play(self):
        return PLAYING, "resume"

    def stop(self):
        return STOPPED, "rewind"


# The states hold no data, so one object of each class serves every player.
STOPPED, PLAYING, PAUSED = Stopped(), Playing(), Paused()


class Player:
    def __init__(self):
        self.state = STOPPED

    def play(self):
        self._change("play", *self.state.play())

    def pause(self):
        self._change("pause", *self.state.pause())

    def stop(self):
        self._change("stop", *self.state.stop())

    def _change(self, event, after, action):
        print(f"{event}: {action} ({self.state.name} -> {after.name})")
        self.state = after


player = Player()
player.play()
player.pause()
player.play()
player.stop()
player.pause()
```

```output
play: start at 0:00 (stopped -> playing)
pause: hold the position (playing -> paused)
play: resume (paused -> playing)
stop: rewind (playing -> stopped)
pause: ignored (stopped -> stopped)
```

- Verdict: idiom. A class for each state, with one method for each event. Each method returns the next state and the action, and the player stores the next state.
- The base class `State` gives the default for every event. A state class overrides only the events that it handles.
- The three state objects are module constants. They hold no data, so all players can share them, as the Flyweight note of Design Patterns describes.
- The node page shows the same machine as an `Enum` and a `match` statement.
