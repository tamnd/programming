```sql run id=scopes
CREATE TABLE customers (id INTEGER, name TEXT);
CREATE TABLE orders (id INTEGER, customer_id INTEGER, amount INTEGER);
INSERT INTO customers VALUES (1, 'ann'), (2, 'bob');
INSERT INTO orders VALUES (10, 1, 30), (11, 1, 20), (12, 2, 5);

SELECT c.name,
       (SELECT SUM(o.amount) FROM orders o WHERE o.customer_id = c.id) AS spent
FROM customers c
ORDER BY c.name;
```

```output for=scopes
ann|50
bob|5
```

- Verdict: native. A subquery can refer to the columns of the outer query. The name `c` is in scope in the correlated subquery because the subquery is inside the query that declares it.
- An unqualified column name in a subquery can bind to a table of the outer query when no table in the subquery has a column with that name. The second example shows the effect in SQLite.

```sql wrong id=outer-binding
CREATE TABLE orders (id INTEGER, customer_id INTEGER, amount INTEGER);
CREATE TABLE vip (cust_id INTEGER);
INSERT INTO orders VALUES (10, 1, 30), (11, 2, 20), (12, 3, 5);
INSERT INTO vip VALUES (1);

SELECT id FROM orders
WHERE customer_id IN (SELECT customer_id FROM vip)
ORDER BY id;
```

```output for=outer-binding
10
11
12
```

The table `vip` has no column `customer_id`, so the name in the subquery binds to `orders.customer_id`. The condition is true for each order that `vip` has at least one row for, and the query returns all three orders. Qualify the column, `SELECT v.customer_id FROM vip v`, and the database reports the error.
