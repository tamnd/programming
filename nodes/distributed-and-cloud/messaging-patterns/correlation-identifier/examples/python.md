```python run id=match-replies
import json

PRICES = {"A1": 1999, "B2": 500}

reply_channel = [  # one reply channel for all the requests of this requestor
    json.dumps({"correlation_id": "q0", "body": "1999 cents"}),  # left from a request that timed out
]

pending = {}  # request id -> what the request asked for
requests = []
for rid, sku in [("q1", "A1"), ("q2", "B2"), ("q3", "Z9")]:
    pending[rid] = sku
    requests.append(json.dumps({"id": rid, "sku": sku}))

# The replier works on the requests in a different order, and it copies the id of each request.
for raw in reversed(requests):
    request = json.loads(raw)
    sku = request["sku"]
    body = f"{PRICES[sku]} cents" if sku in PRICES else f"error unknown sku {sku}"
    reply_channel.append(json.dumps({"correlation_id": request["id"], "body": body}))

for raw in reply_channel:
    reply = json.loads(raw)
    cid = reply["correlation_id"]
    sku = pending.pop(cid, None)
    if sku is None:
        print(f"no outstanding request for {cid}, dropped")
        continue
    print(f"reply for {cid} ({sku}): {reply['body']}")
print("still waiting:", len(pending))
```

```output
no outstanding request for q0, dropped
reply for q3 (Z9): error unknown sku Z9
reply for q2 (B2): 500 cents
reply for q1 (A1): 1999 cents
still waiting: 0
```

- Verdict: idiom. A dict from the request id to the request is the table of the requests that wait, and `json` gives the bytes of each message.
- `pending.pop(cid, None)` finds and removes the entry in one step, so a second reply with the same id matches nothing.
- With a broker, the id goes in a header, such as `correlation_id` in AMQP 0-9-1 or `JMSCorrelationID` in Jakarta Messaging, not in the body.
