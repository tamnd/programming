```python run id=event-loop-pattern
import heapq
from collections import deque


class EventLoop:
    """One thread takes ready callbacks from a queue and runs each one to the end."""

    def __init__(self):
        self.ready = deque()  # callbacks to run in this turn or the next
        self.timers = []  # heap of (due time, order, callback)
        self.now = 0  # a counter clock, so the output does not depend on the machine
        self.order = 0
        self.turns = 0

    def call_soon(self, callback):
        self.ready.append(callback)

    def call_later(self, delay, callback):
        self.order += 1
        heapq.heappush(self.timers, (self.now + delay, self.order, callback))

    def run(self):
        while self.ready or self.timers:
            if not self.ready:
                self.now = self.timers[0][0]  # a real loop sleeps or waits for I/O here
            while self.timers and self.timers[0][0] <= self.now:
                self.ready.append(heapq.heappop(self.timers)[2])
            self.turns += 1
            for _ in range(len(self.ready)):  # callbacks added now wait for the next turn
                self.ready.popleft()()


loop = EventLoop()
log = []


def say(text):
    def callback():
        log.append(f"time {loop.now}: {text}")
    return callback


def first():
    say("first callback")()
    loop.call_soon(say("callback that the first callback added"))


loop.call_later(20, say("timer of 20"))
loop.call_later(10, say("timer of 10"))
loop.call_soon(first)
loop.call_soon(say("second callback"))
loop.run()

print("order of the callbacks:")
for line in log:
    print(" ", line)
print("turns of the loop:", loop.turns)
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

- Verdict: idiom. A small loop class with a `deque` of ready callbacks and a `heapq` of timers. In real code, `asyncio` gives `loop.call_soon` and `loop.call_later`.
