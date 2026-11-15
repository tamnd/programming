```python run id=price-requests
import queue
import threading

PRICES = {"A1": 1999, "B2": 500}

# Named channels, as on a broker: one request channel and one reply channel for each requestor.
channels = {"prices": queue.Queue(), "replies.web": queue.Queue(), "replies.mobile": queue.Queue()}


def replier():
    while (request := channels["prices"].get()) is not None:
        sku = request["body"]
        if sku in PRICES:
            reply = {"correlation_id": request["id"], "body": f"{PRICES[sku]} cents"}
        else:
            reply = {"correlation_id": request["id"], "body": f"error unknown sku {sku}"}
        channels[request["reply_to"]].put(reply)  # the return address of the request


def ask(requestor, rid, sku):
    reply_to = "replies." + requestor
    channels["prices"].put({"id": rid, "reply_to": reply_to, "body": sku})
    try:
        reply = channels[reply_to].get(timeout=2)  # block, but not for ever
    except queue.Empty:
        return f"{requestor} asked for {sku}: no reply in 2 s"
    return f"{requestor} asked for {sku}: {reply['body']}"


worker = threading.Thread(target=replier)
worker.start()
print(ask("web", "q1", "A1"))
print(ask("mobile", "q2", "B2"))
print(ask("web", "q3", "Z9"))
channels["prices"].put(None)  # stop the replier
worker.join()
```

```output
web asked for A1: 1999 cents
mobile asked for B2: 500 cents
web asked for Z9: error unknown sku Z9
```

- Verdict: idiom. Python has no request-reply channel, but `queue.Queue` and a thread give the two channels and the replier. With a broker, the names of the channels are queue names, and `reply_to` is a property of the message.
- `get(timeout=2)` blocks, but it raises `queue.Empty` after 2 seconds, so a requestor does not wait for ever.
- The replier reads the `reply_to` field of each request and does not know the requestors. `None` on the request channel stops it.
