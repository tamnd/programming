```javascript run id=link-table
// A stand-in for the database: two entity tables and a link table of [employeeId, skillId] pairs.
const employeeTable = new Map();
const skillTable = new Map([[1, "Python"], [2, "SQL"], [3, "Go"]]);
const employeeSkill = new Set(); // Each key is "employeeId-skillId", like a two-column primary key.
let lastEmployee = 0;

function insertLink(employeeId, skillId) {
  const key = `${employeeId}-${skillId}`;
  if (employeeSkill.has(key)) {
    throw new Error("UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id");
  }
  employeeSkill.add(key);
}

function linkRows() {
  return [...employeeSkill]
    .map((k) => k.split("-").map(Number))
    .sort((a, b) => a[0] - b[0] || a[1] - b[1]);
}

class Employee {
  constructor(name, skills, id = null) {
    this.id = id;
    this.name = name;
    this.skills = skills; // A list of skill keys.
  }
}

function insert(e) {
  lastEmployee += 1;
  employeeTable.set(lastEmployee, e.name);
  e.id = lastEmployee;
  for (const s of e.skills) insertLink(e.id, s);
}

function updateSkills(e) {
  // Compare the collection with the link rows: insert the new pairs, delete the old ones.
  const inDb = linkRows().filter(([emp]) => emp === e.id).map(([, s]) => s);
  const added = e.skills.filter((s) => !inDb.includes(s));
  const removed = inDb.filter((s) => !e.skills.includes(s));
  for (const s of added) insertLink(e.id, s);
  for (const s of removed) employeeSkill.delete(`${e.id}-${s}`);
  return [removed.length, added.length];
}

function skillsOf(employeeId) {
  return linkRows()
    .filter(([emp]) => emp === employeeId)
    .map(([, s]) => skillTable.get(s))
    .join(", ");
}

function employeesWith(skillId) {
  return linkRows()
    .filter(([, s]) => s === skillId)
    .map(([emp]) => employeeTable.get(emp))
    .join(", ");
}

function links() {
  return linkRows()
    .map(([emp, s]) => `${emp}-${s}`)
    .join(" ");
}

const ann = new Employee("Ann", [1, 2]);
insert(ann);
insert(new Employee("Bo", [1]));
console.log(`links: ${links()}`);
console.log(`Ann: ${skillsOf(1)}`);
console.log(`Python: ${employeesWith(1)}`);

ann.skills = [2, 3];
const [deleted, inserted] = updateSkills(ann);
console.log(`update Ann: deleted ${deleted}, inserted ${inserted}`);
console.log(`links: ${links()}`);
console.log(`Ann: ${skillsOf(1)}`);

try {
  insertLink(2, 1);
} catch (e) {
  console.log(`link 2-1 again: ${e.message}`);
}
```

```output
links: 1-1 1-2 2-1
Ann: Python, SQL
Python: Ann, Bo
update Ann: deleted 1, inserted 1
links: 1-2 1-3 2-1
Ann: SQL, Go
link 2-1 again: UNIQUE constraint failed: employee_skill.employee_id, employee_skill.skill_id
```

- Verdict: idiom. The link table is a `Set` of keys in the form `"employeeId-skillId"`, so the pair works like a two-column primary key, and `insertLink` throws for a pair that is already there.
- `skillsOf` and `employeesWith` read the same set from the two sides, as the two joins do in SQL.
