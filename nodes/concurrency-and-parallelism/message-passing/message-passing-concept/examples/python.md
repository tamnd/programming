```python run id=process-queues-and-pipes
from multiprocessing import Pipe, Process, Queue


def square_all(inbox, outbox):
    while (n := inbox.get()) is not None:
        outbox.put(n * n)
    outbox.put(None)


def change(conn):
    data = conn.recv()
    data.append("changed in child")
    conn.send(data)
    conn.close()


def make_account(balance):
    # Message passing in the sense of SICP: the object is a procedure
    # that takes the name of an operation as a message.
    def dispatch(message, amount=0):
        nonlocal balance
        if message == "deposit":
            balance += amount
        elif message == "withdraw":
            balance -= amount
        elif message == "balance":
            return balance
        else:
            raise ValueError("unknown message: " + message)

    return dispatch


if __name__ == "__main__":
    # A process takes messages from one queue and sends results on another.
    inbox, outbox = Queue(), Queue()
    p = Process(target=square_all, args=(inbox, outbox))
    p.start()
    for n in [1, 2, 3, 4]:
        inbox.put(n)
    inbox.put(None)
    results = []
    while (r := outbox.get()) is not None:
        results.append(r)
    p.join()
    print(results)

    # A message is a copy: the child changes its copy, not the list of the parent.
    parent_end, child_end = Pipe()
    data = ["original"]
    p = Process(target=change, args=(child_end,))
    p.start()
    parent_end.send(data)
    print(parent_end.recv(), data)
    p.join()

    account = make_account(100)
    account("deposit", 50)
    account("withdraw", 30)
    print(account("balance"))
    try:
        account("close")
    except ValueError as e:
        print(e)
```

```output for=process-queues-and-pipes
[1, 4, 9, 16]
['original', 'changed in child'] ['original']
120
unknown message: close
```

- Verdict: stdlib. A `multiprocessing.Queue` carries numbers to another process, and a second queue carries the squares back, with `None` as the end of the stream. A list sent through a `Pipe` is pickled, so the child changes a copy and the list of the parent stays `['original']`. The account is message passing in the sense of SICP: a procedure that takes the name of an operation.
