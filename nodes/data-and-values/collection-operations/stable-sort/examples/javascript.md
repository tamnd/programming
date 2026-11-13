```javascript run id=stable-tosorted-passes
const staff = [
  { name: "ann", team: "ops", grade: 3 },
  { name: "bob", team: "dev", grade: 2 },
  { name: "cy", team: "ops", grade: 2 },
  { name: "di", team: "dev", grade: 3 },
  { name: "ed", team: "ops", grade: 2 },
];
const names = (a) => a.map((p) => p.name).join(" ");

// sort and toSorted are stable, so the name order stays inside each team.
console.log(names(staff.toSorted((a, b) => a.team.localeCompare(b.team))));

// Two stable passes: the second key first, then the first key.
const pass1 = staff.toSorted((a, b) => a.grade - b.grade);
console.log(names(pass1.toSorted((a, b) => a.team.localeCompare(b.team))));

// One pass with a chained compare function gives the same order.
console.log(names(staff.toSorted((a, b) => a.team.localeCompare(b.team) || a.grade - b.grade)));

// A reversed compare function keeps equal items in their old order.
console.log(names(staff.toSorted((a, b) => b.team.localeCompare(a.team))));
```

```output
bob di ann cy ed
bob di cy ed ann
bob di cy ed ann
ann cy ed bob di
```

- Verdict: native. `toSorted` is stable, so the team sort keeps the name order. Two passes and one chained compare function give the same order. A compare function with swapped arguments sorts the teams in descending order and keeps the equal items in their input order.
