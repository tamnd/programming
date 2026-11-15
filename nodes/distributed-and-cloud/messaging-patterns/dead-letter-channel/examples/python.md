```python run id=max-deliveries
from collections import deque

MAX_DELIVERIES = 3


def handle(body):
    name, amount = body.split()
    if not amount.isdigit():
        raise ValueError(f"bad amount {amount}")
    print(f"ok {name} amount {amount}")


queue = deque({"body": b, "deliveries": 0} for b in ["pay-1 10", "pay-2 x", "pay-3 5"])
dead_letters = []
processed = 0
while queue:
    msg = queue.popleft()
    msg["deliveries"] += 1
    try:
        handle(msg["body"])
        processed += 1
    except ValueError as e:
        print(f"fail {msg['body'].split()[0]} delivery {msg['deliveries']}: {e}")
        if msg["deliveries"] < MAX_DELIVERIES:
            queue.append(msg)
        else:
            msg["reason"] = str(e)
            msg["source"] = "payments"
            dead_letters.append(msg)

for msg in dead_letters:
    print(f"dead letter {msg['body'].split()[0]}: deliveries={msg['deliveries']} "
          f"reason={msg['reason']} source={msg['source']}")
print(f"processed {processed}, dead letters {len(dead_letters)}")
```

```output
ok pay-1 amount 10
fail pay-2 delivery 1: bad amount x
ok pay-3 amount 5
fail pay-2 delivery 2: bad amount x
fail pay-2 delivery 3: bad amount x
dead letter pay-2: deliveries=3 reason=bad amount x source=payments
processed 2, dead letters 1
```

- Verdict: idiom. Python has no dead letter channel. A `collections.deque` is the queue, a count in each message is the delivery count, and a list is the dead letter channel.
- A message that fails goes back to the end of the queue, so `pay-3` is processed before the second delivery of `pay-2`. A broker that puts the message back at the head would make the next messages wait.
- With a broker, the receiver does not count. A Pika consumer calls `basic_nack` with `requeue=False`, and RabbitMQ moves the message to the dead-letter exchange of the queue.
