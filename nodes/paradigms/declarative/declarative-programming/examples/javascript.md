```javascript run id=view
function view(state) {
  if (state.status === "sending") return "[form disabled] Sending...";
  if (state.status === "sent") return "Thank you!";
  const button = state.answer.trim() === "" ? "[Submit disabled]" : "[Submit]";
  const error = state.error ? ` Error: ${state.error}` : "";
  return `[input: ${state.answer}] ${button}${error}`;
}

const states = [
  { status: "typing", answer: "" },
  { status: "typing", answer: "Lima" },
  { status: "sending", answer: "Lima" },
  { status: "typing", answer: "Lima", error: "network" },
  { status: "sent", answer: "Lima" },
];
for (const s of states) console.log(view(s));

const dates = "from 2024-01-31 to 2025-12-01".matchAll(/\b(\d{4})-(\d{2})-(\d{2})\b/g);
console.log([...dates].map((m) => m[1]));
```

```output for=view
[input: ] [Submit disabled]
[input: Lima] [Submit]
[form disabled] Sending...
[input: Lima] [Submit] Error: network
Thank you!
[ '2024', '2025' ]
```

- Verdict: native. The `view` function describes what the screen shows for each state, and it does not say how to change the screen from one state to the next. This is the model of React and other declarative UI libraries: the library compares the old and the new description and makes the changes.
- Regular expressions are built in, with literal syntax `/.../`.
