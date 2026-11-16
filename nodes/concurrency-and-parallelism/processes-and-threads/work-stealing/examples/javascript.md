```javascript run id=work-stealing-model
// A step-by-step model of a work-stealing scheduler with two workers.
// Each worker has a deque (an array). Index 0 is the top (oldest task); the end is the bottom (newest task).
// A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
const WORKERS = 2;
const deques = Array.from({ length: WORKERS }, () => []);
const leaves = new Array(WORKERS).fill(0);
let total = 0;

deques[0].push([0, 8]);
let round = 0;
while (deques.some((d) => d.length > 0)) {
  round += 1;
  for (let w = 0; w < WORKERS; w++) {
    let task;
    let how;
    let source = "";
    if (deques[w].length > 0) {
      task = deques[w].pop(); // the owner takes the newest task (LIFO)
      how = "pops";
    } else {
      const victim = deques.findIndex((d, v) => v !== w && d.length > 0);
      if (victim === -1) {
        console.log(`round ${round}: w${w} is idle`);
        continue;
      }
      task = deques[victim].shift(); // a thief takes the oldest task (FIFO)
      how = "steals";
      source = ` from w${victim}`;
    }
    const [lo, hi] = task;
    if (hi - lo > 2) {
      const mid = Math.floor((lo + hi) / 2);
      deques[w].push([lo, mid], [mid, hi]);
      console.log(`round ${round}: w${w} ${how} [${lo}, ${hi})${source} and splits it`);
    } else {
      for (let i = lo; i < hi; i++) total += i;
      leaves[w] += 1;
      console.log(`round ${round}: w${w} ${how} [${lo}, ${hi})${source} and runs it`);
    }
  }
}

console.log(`leaves run: ${leaves.map((n, w) => `w${w} ${n}`).join(", ")}`);
console.log(`sum: ${total}`);
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: absent. JavaScript runs each event loop on one thread, and it has no work-stealing scheduler. This file is a model of the rules, with two arrays in place of two threads.
- The owner takes its newest task with `pop`, from the end of its array. A thief takes the oldest task of another worker with `shift`, from the start.
- In round 1, worker w1 steals the large task `[0, 4)` and splits it further. One steal gives w1 half of all the work.
