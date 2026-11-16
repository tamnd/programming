```javascript run id=executor-policies
// Three ways to run a task. The code that submits the tasks is the same for each.
const executors = {
  direct: (task) => task(),
  microtask: (task) => queueMicrotask(task),
  timer: (task) => setTimeout(task, 0),
};

const log = [];
for (const [name, execute] of Object.entries(executors)) {
  execute(() => log.push(name));
}
log.push("all tasks submitted");

// Print the log when the event loop has run the queued tasks.
setTimeout(() => console.log(log.join("\n")), 20);
```

```output for=executor-policies
direct
all tasks submitted
microtask
timer
```

- Verdict: absent. JavaScript has no executor interface, but the event loop is an executor with fixed rules. A program chooses when a task runs by the queue that it puts the task in.
- `direct` runs the task inside the call. `queueMicrotask` runs it when the current script has finished and the stack is empty. `setTimeout` puts it in the timer queue, which the event loop handles after the microtasks.
- All three run on the main thread. For work on other threads, Node.js has `worker_threads`.
