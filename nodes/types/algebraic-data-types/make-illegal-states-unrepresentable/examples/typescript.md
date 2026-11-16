```typescript run id=connection-state
type State = "connecting" | "connected" | "disconnected";

// Before: a state flag and five optional fields.
interface FlagSet {
  state: State;
  lastPingTime?: number;
  lastPingId?: number;
  sessionId?: string;
  whenInitiated?: number;
  whenDisconnected?: number;
}

const FIELDS = [
  "lastPingTime",
  "lastPingId",
  "sessionId",
  "whenInitiated",
  "whenDisconnected",
] as const;
const SAMPLE = {
  lastPingTime: 12,
  lastPingId: 7,
  sessionId: "s1",
  whenInitiated: 10,
  whenDisconnected: 20,
};

// The fields that each state must have, and no others.
const ALLOWED: Record<State, string[]> = {
  connecting: ["whenInitiated"],
  connected: ["sessionId", "lastPingTime,lastPingId,sessionId"],
  disconnected: ["whenDisconnected"],
};

function legal(c: FlagSet): boolean {
  const present = FIELDS.filter((f) => c[f] !== undefined).join(",");
  return ALLOWED[c.state].includes(present);
}

// After: each state carries its own fields.
type ConnectionState =
  | { kind: "connecting"; whenInitiated: number }
  | {
      kind: "connected";
      sessionId: string;
      lastPing?: readonly [time: number, id: number];
    }
  | { kind: "disconnected"; whenDisconnected: number };

function toFlags(s: ConnectionState): FlagSet {
  switch (s.kind) {
    case "connecting":
      return { state: s.kind, whenInitiated: s.whenInitiated };
    case "connected":
      return {
        state: s.kind,
        sessionId: s.sessionId,
        lastPingTime: s.lastPing?.[0],
        lastPingId: s.lastPing?.[1],
      };
    case "disconnected":
      return { state: s.kind, whenDisconnected: s.whenDisconnected };
    default: {
      const unreachable: never = s;
      return unreachable;
    }
  }
}

const flagSets: FlagSet[] = [];
for (const state of Object.keys(ALLOWED) as State[]) {
  for (let mask = 0; mask < 1 << FIELDS.length; mask++) {
    const c: FlagSet = { state };
    FIELDS.forEach((f, bit) => {
      if (mask & (1 << bit)) Object.assign(c, { [f]: SAMPLE[f] });
    });
    flagSets.push(c);
  }
}
let count = flagSets.filter(legal).length;
console.log(`flag set: ${flagSets.length} forms, ${count} legal`);

// One value of each form: the optional lastPing gives connected two forms.
const sums: ConnectionState[] = [
  { kind: "connecting", whenInitiated: 10 },
  { kind: "connected", sessionId: "s1" },
  { kind: "connected", sessionId: "s1", lastPing: [12, 7] },
  { kind: "disconnected", whenDisconnected: 20 },
];
count = sums.filter((s) => legal(toFlags(s))).length;
console.log(`sum type: ${sums.length} forms, ${count} legal`);

// No session id, but the loose type accepts it. The precise type rejects
// { kind: "connected" } without a sessionId at compile time.
const bad: FlagSet = { state: "connected" };
const ok = legal(bad) ? "yes" : "no";
console.log(`built: ${bad.state} without a session id, legal: ${ok}`);
```

```output
flag set: 96 forms, 4 legal
sum type: 4 forms, 4 legal
built: connected without a session id, legal: no
```
