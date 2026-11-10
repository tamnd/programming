```sql run id=shop
CREATE TABLE items (name TEXT PRIMARY KEY, stock INTEGER NOT NULL);
CREATE TABLE reservations (item TEXT NOT NULL REFERENCES items (name), paid INTEGER NOT NULL);
CREATE TABLE parcels (id INTEGER PRIMARY KEY, item TEXT NOT NULL);

INSERT INTO items VALUES ('book', 2), ('lamp', 0);
INSERT INTO reservations VALUES ('book', 1);
INSERT INTO parcels (item) VALUES ('book');

-- The facade for reads: one view over three tables.
CREATE VIEW available AS
SELECT
    i.name,
    i.stock - (SELECT count(*) FROM reservations AS r WHERE r.item = i.name) AS free,
    (SELECT count(*) FROM parcels AS p WHERE p.item = i.name) AS shipped
FROM items AS i;

SELECT printf('%s: %d free, %d shipped', name, free, shipped) FROM available ORDER BY name;
```

```output
book: 1 free, 1 shipped
lamp: 0 free, 0 shipped
```

- Verdict: idiom. A view gives one query interface over several tables. A client reads `available` and does not need to know how stock, reservations and parcels are stored. The example runs in SQLite.
- The view is a facade only for reads. In a database with stored procedures, such as PostgreSQL, a procedure that reserves, charges and ships inside one transaction is the facade for writes.
- A grant on the view and not on the tables makes the facade the only way in, as an unexported name does in a programming language.
