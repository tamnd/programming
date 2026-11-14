```python run id=link-table
import sqlite3

db = sqlite3.connect(":memory:")
db.executescript("""
create table employee (id integer primary key, name text);
create table skill (id integer primary key, name text);
create table employee_skill (
    employee_id integer references employee(id),
    skill_id integer references skill(id),
    primary key (employee_id, skill_id));
insert into skill (name) values ('Python'), ('SQL'), ('Go');
""")


class Employee:
    def __init__(self, name, skills, id=None):
        self.id, self.name, self.skills = id, name, skills  # skills: a list of skill keys


def insert(e):
    e.id = db.execute("insert into employee (name) values (?)", (e.name,)).lastrowid
    for s in e.skills:
        db.execute("insert into employee_skill values (?, ?)", (e.id, s))


def update_skills(e):
    # Compare the collection with the link rows: insert the new pairs, delete the old ones.
    sql = "select skill_id from employee_skill where employee_id = ?"
    in_db = {s for (s,) in db.execute(sql, (e.id,))}
    added = [s for s in e.skills if s not in in_db]
    removed = sorted(in_db - set(e.skills))
    for s in added:
        db.execute("insert into employee_skill values (?, ?)", (e.id, s))
    for s in removed:
        db.execute("delete from employee_skill where employee_id = ? and skill_id = ?", (e.id, s))
    return len(removed), len(added)


def skills_of(employee_id):
    sql = """select s.name from skill s join employee_skill es on es.skill_id = s.id
             where es.employee_id = ? order by s.id"""
    return ", ".join(n for (n,) in db.execute(sql, (employee_id,)))


def employees_with(skill_id):
    sql = """select e.name from employee e join employee_skill es on es.employee_id = e.id
             where es.skill_id = ? order by e.id"""
    return ", ".join(n for (n,) in db.execute(sql, (skill_id,)))


def links():
    rows = db.execute("select employee_id, skill_id from employee_skill order by 1, 2")
    return " ".join(f"{e}-{s}" for e, s in rows)


ann = Employee("Ann", [1, 2])
insert(ann)
insert(Employee("Bo", [1]))
print("links:", links())
print("Ann:", skills_of(1))
print("Python:", employees_with(1))

ann.skills = [2, 3]
deleted, inserted = update_skills(ann)
print(f"update Ann: deleted {deleted}, inserted {inserted}")
print("links:", links())
print("Ann:", skills_of(1))

try:
    db.execute("insert into employee_skill values (2, 1)")
except sqlite3.IntegrityError as e:
    print("link 2-1 again:", e)
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

- Verdict: idiom. The link table `employee_skill` has two foreign keys and a primary key on the pair, so SQLite rejects a second row for the same pair with an `IntegrityError`.
- `update_skills` reads the keys of the link rows, inserts the pairs that are new and deletes the pairs that are gone. The other rows stay as they are.
