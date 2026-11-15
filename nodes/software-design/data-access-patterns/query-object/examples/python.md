```python run id=query-object-to-sql
import sqlite3


class Player:
    def __init__(self, id, name, country, goals):
        self.id, self.name, self.country, self.goals = id, name, country, goals


# The metadata: each field of Player and its column in the player table.
PLAYER_MAP = {"table": "player",
              "columns": {"id": "id", "name": "name", "country": "nation", "goals": "goals_scored"}}


class Criterion:
    # One condition on a field. The query object is a tree of these.
    def __init__(self, op, field, value):
        self.op, self.field, self.value = op, field, value

    def sql(self, cols, params):
        col = cols[self.field]
        if self.op == "starts":
            params.append(self.value + "%")
            return f"{col} like ?"
        params.append(self.value)
        return f"{col} {self.op} ?"

    def matches(self, obj):
        v = getattr(obj, self.field)
        if self.op == "=":
            return v == self.value
        if self.op == ">":
            return v > self.value
        return v.startswith(self.value)


class AnyOf:
    # A group of conditions joined with "or".
    def __init__(self, *parts):
        self.parts = parts

    def sql(self, cols, params):
        return "(" + " or ".join(p.sql(cols, params) for p in self.parts) + ")"

    def matches(self, obj):
        return any(p.matches(obj) for p in self.parts)


def eq(field, value):
    return Criterion("=", field, value)


def gt(field, value):
    return Criterion(">", field, value)


def starts(field, value):
    return Criterion("starts", field, value)


class Query:
    # A query on fields of a class. It knows nothing of tables and columns
    # until to_sql reads the data map.
    def __init__(self):
        self.criteria, self.order = [], None

    def where(self, c):
        self.criteria.append(c)
        return self

    def order_by(self, field):
        self.order = field
        return self

    def to_sql(self, data_map):
        cols, params = data_map["columns"], []
        sql = f"select {', '.join(cols.values())} from {data_map['table']}"
        if self.criteria:
            sql += " where " + " and ".join(c.sql(cols, params) for c in self.criteria)
        if self.order:
            sql += f" order by {cols[self.order]}"
        return sql, params

    def matches(self, obj):
        return all(c.matches(obj) for c in self.criteria)


PLAYERS = [Player(1, "Ann", "NZ", 14), Player(2, "Bo", "NZ", 3), Player(3, "Cy", "NZ", 21),
           Player(4, "Dee", "AU", 9), Player(5, "Eli", "GB", 12)]


def make_db(data_map):
    cols = data_map["columns"]
    db = sqlite3.connect(":memory:")
    db.execute(f"create table player (id integer primary key, name text, {cols['country']} text, goals_scored integer)")
    db.executemany("insert into player values (?, ?, ?, ?)",
                   [(p.id, p.name, p.country, p.goals) for p in PLAYERS])
    return db


def run(title, query, data_map):
    sql, params = query.to_sql(data_map)
    print("query:", title)
    print("sql:", sql)
    print("params:", params)
    rows = make_db(data_map).execute(sql, params).fetchall()
    print("rows:", " | ".join(f"{r[1]} ({r[2]}, {r[3]})" for r in rows))
    found = sorted((p for p in PLAYERS if query.matches(p)), key=lambda p: getattr(p, query.order))
    print("in memory:", " | ".join(p.name for p in found))


q1 = Query().where(eq("country", "NZ")).where(gt("goals", 10)).order_by("name")
q2 = Query().where(AnyOf(starts("name", "B"), eq("country", "AU"))).order_by("name")
run("from NZ and more than 10 goals", q1, PLAYER_MAP)
run("name starts with B or from AU", q2, PLAYER_MAP)

# The column nation is renamed. Only the data map changes; the query does not.
NEW_MAP = {"table": "player", "columns": dict(PLAYER_MAP["columns"], country="country_code")}
run("from NZ and more than 10 goals, after the rename", q1, NEW_MAP)
```

```output
query: from NZ and more than 10 goals
sql: select id, name, nation, goals_scored from player where nation = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
query: name starts with B or from AU
sql: select id, name, nation, goals_scored from player where (name like ? or nation = ?) order by name
params: ['B%', 'AU']
rows: Bo (NZ, 3) | Dee (AU, 9)
in memory: Bo | Dee
query: from NZ and more than 10 goals, after the rename
sql: select id, name, country_code, goals_scored from player where country_code = ? and goals_scored > ? order by name
params: ['NZ', 10]
rows: Ann (NZ, 14) | Cy (NZ, 21)
in memory: Ann | Cy
```

- Verdict: idiom. The criteria are small classes with a `sql` method and a `matches` method, so one tree has two interpreters. `to_sql` reads the data map, so the query names only the fields of `Player`.
- The values go into the parameter list and never into the SQL text, so `sqlite3` binds them.
- In real code, SQLAlchemy `select()` and the Django `QuerySet` are query objects of this kind.
