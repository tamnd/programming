```sql run id=guard
CREATE TABLE items (name TEXT, total INTEGER, count INTEGER);
INSERT INTO items VALUES ('a', 10, 2), ('b', 5, 0), ('c', 9, 3);

SELECT name,
       CASE WHEN count > 0 THEN total / count ELSE NULL END AS average
FROM items
ORDER BY name;
```

```output for=guard
a|5
b|
c|3
```

- Verdict: absent. SQL describes a result, not steps. The order in which a database evaluates the parts of an expression is not defined. PostgreSQL states that the inputs of an operator or function are not necessarily evaluated left to right, and that the planner can reorder `AND` and `OR` terms in `WHERE` and `HAVING`.
- So `WHERE count > 0 AND total / count > 2` does not protect the division. `CASE` evaluates its branches in order, and is the documented way to force an order.
- SQLite returns NULL for division by zero, so this example does not fail without the guard there. PostgreSQL raises an error.
