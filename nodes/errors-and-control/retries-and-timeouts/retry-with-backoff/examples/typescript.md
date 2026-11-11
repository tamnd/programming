```typescript run id=policy
type Outcome =
  | { kind: "ok"; value: string }
  | { kind: "transient"; reason: string }
  | { kind: "throttled"; retryAfterMs: number }
  | { kind: "permanent"; reason: string };

type Decision = { retry: false } | { retry: true; delayMs: number };

function decide(outcome: Outcome, attempt: number, random: () => number): Decision {
  const backoff = Math.floor(random() * Math.min(1000, 100 * 2 ** attempt));
  switch (outcome.kind) {
    case "ok":
    case "permanent":
      return { retry: false };
    case "transient":
      return { retry: true, delayMs: backoff };
    case "throttled":
      return { retry: true, delayMs: Math.max(backoff, outcome.retryAfterMs) };
    default: {
      const unhandled: never = outcome;
      return unhandled;
    }
  }
}

function run(outcomes: Outcome[], random: () => number): void {
  let now = 0;
  for (let attempt = 0; attempt < outcomes.length; attempt++) {
    const outcome = outcomes[attempt];
    const d = decide(outcome, attempt, random);
    if (!d.retry) {
      console.log(`  t=${now} ms: ${outcome.kind}, stop`);
      return;
    }
    console.log(`  t=${now} ms: ${outcome.kind}, wait ${d.delayMs} ms`);
    now += d.delayMs;
  }
  console.log(`  t=${now} ms: out of attempts`);
}

let seed = 11;
const random = () => {
  seed = (Math.imul(seed, 1664525) + 1013904223) >>> 0;
  return seed / 2 ** 32;
};

console.log("flaky");
run([{ kind: "transient", reason: "503" }, { kind: "throttled", retryAfterMs: 1500 }, { kind: "ok", value: "done" }], random);
console.log("bad request");
run([{ kind: "permanent", reason: "400" }], random);
```

```output
flaky
  t=0 ms: transient, wait 24 ms
  t=24 ms: throttled, wait 1500 ms
  t=1524 ms: ok, stop
bad request
  t=0 ms: permanent, stop
```

- Verdict: stdlib. TypeScript adds no retry construct. A union type lists the outcomes of a call, and the checker makes the retry policy handle each one.
- The `never` in the `default` branch fails to compile if someone adds a new kind of outcome and forgets to decide whether it is worth a retry.
- The policy is a pure function. It takes the attempt number and a source of random numbers, so a test can call it without a clock or a network.
