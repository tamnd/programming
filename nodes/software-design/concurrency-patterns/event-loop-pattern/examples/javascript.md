```javascript run id=event-loop-pattern
// One thread takes ready callbacks from a queue and runs each one to the end.
// JavaScript already runs on an event loop; this small one shows its parts.
class EventLoop {
  constructor() {
    this.ready = []; // callbacks to run in this turn or the next
    this.timers = []; // [due time, order, callback], kept sorted
    this.now = 0; // a counter clock, so the output does not depend on the machine
    this.order = 0;
    this.turns = 0;
  }

  callSoon(callback) {
    this.ready.push(callback);
  }

  callLater(delay, callback) {
    this.order++;
    this.timers.push([this.now + delay, this.order, callback]);
    this.timers.sort((a, b) => a[0] - b[0] || a[1] - b[1]);
  }

  run() {
    while (this.ready.length > 0 || this.timers.length > 0) {
      if (this.ready.length === 0) this.now = this.timers[0][0]; // a real loop sleeps or waits for I/O here
      while (this.timers.length > 0 && this.timers[0][0] <= this.now) {
        this.ready.push(this.timers.shift()[2]);
      }
      this.turns++;
      const count = this.ready.length; // callbacks added now wait for the next turn
      for (let i = 0; i < count; i++) this.ready.shift()();
    }
  }
}

const loop = new EventLoop();
const log = [];

function say(text) {
  return () => log.push(`time ${loop.now}: ${text}`);
}

function first() {
  say("first callback")();
  loop.callSoon(say("callback that the first callback added"));
}

loop.callLater(20, say("timer of 20"));
loop.callLater(10, say("timer of 10"));
loop.callSoon(first);
loop.callSoon(say("second callback"));
loop.run();

console.log("order of the callbacks:");
for (const line of log) console.log("  " + line);
console.log("turns of the loop: " + loop.turns);
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

- Verdict: idiom. JavaScript already runs on an event loop. The example builds a small one with an array of callbacks and a sorted array of timers to show the parts.
