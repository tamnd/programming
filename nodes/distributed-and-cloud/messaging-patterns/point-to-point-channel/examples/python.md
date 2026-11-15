```python run id=competing-workers
import queue
import threading

jobs = queue.Queue()
done = queue.Queue()
STOP = None


def worker(name):
    while (job := jobs.get()) is not STOP:
        done.put((job, name))


workers = [threading.Thread(target=worker, args=(f"w{i}",)) for i in range(1, 4)]
for w in workers:
    w.start()
for n in range(1, 10):
    jobs.put(f"job-{n}")
for _ in workers:
    jobs.put(STOP)  # one stop message for each worker
for w in workers:
    w.join()

results = []
while not done.empty():
    results.append(done.get())
names = sorted(job for job, _ in results)
print("handled:", " ".join(names))
print("each job once:", len(names) == len(set(names)) == 9)
```

```output
handled: job-1 job-2 job-3 job-4 job-5 job-6 job-7 job-8 job-9
each job once: True
```

- Verdict: stdlib. `queue.Queue` gives each item to one `get` call, so three threads that read one queue share the jobs.
- `STOP` is a stop message. Each worker ends when it takes one, so the program sends one for each worker. With a broker, a worker usually runs until its process stops, and a stop message is not needed.
- `get` takes the item out of the queue at once. A broker client with manual acks, such as Pika with `auto_ack=False`, keeps the message until the worker acks it.
