```python run id=active
import queue
import threading
from concurrent.futures import Future


class AccountServant:
    """The implementation. Only the scheduler thread calls it, so it has no lock."""

    def __init__(self):
        self.total = 0

    def deposit(self, amount):
        self.total += amount
        return self.total

    def balance(self):
        return self.total


class Scheduler(threading.Thread):
    def __init__(self, servant):
        super().__init__(daemon=True)
        self.servant = servant
        self.activation_queue = queue.Queue()
        self.gate = threading.Event()  # lets the demo hold the scheduler

    def run(self):
        self.gate.wait()
        while True:
            name, args, future = self.activation_queue.get()
            if name is None:
                return
            future.set_result(getattr(self.servant, name)(*args))


class AccountProxy:
    """The interface. A call becomes a method request and returns a future."""

    def __init__(self):
        self.scheduler = Scheduler(AccountServant())
        self.scheduler.start()

    def _request(self, name, *args):
        future = Future()
        self.scheduler.activation_queue.put((name, args, future))
        return future

    def deposit(self, amount):
        return self._request("deposit", amount)

    def balance(self):
        return self._request("balance")

    def stop(self):
        self.scheduler.activation_queue.put((None, (), None))
        self.scheduler.join()


account = AccountProxy()
per_client = [[] for _ in range(4)]


def client(mine):
    for _ in range(250):
        mine.append(account.deposit(1))  # each call returns at once


clients = [threading.Thread(target=client, args=(mine,)) for mine in per_client]
for t in clients:
    t.start()
for t in clients:
    t.join()
futures = [f for mine in per_client for f in mine]
print("deposits queued:", len(futures))
print("deposits done before the scheduler starts:", sum(f.done() for f in futures))
account.scheduler.gate.set()
print("balance after 1000 deposits:", account.balance().result())
account.stop()
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: library. `queue.Queue` is the activation queue and `concurrent.futures.Future` is the future. The proxy, scheduler and servant are plain classes.
