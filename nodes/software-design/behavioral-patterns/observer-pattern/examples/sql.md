```sql run id=sensor
CREATE TABLE sensor (id INTEGER PRIMARY KEY, value REAL);
CREATE TABLE screen (line TEXT);
CREATE TABLE alarms (line TEXT);
INSERT INTO sensor VALUES (1, 0);

CREATE TRIGGER display AFTER UPDATE OF value ON sensor
BEGIN
    INSERT INTO screen VALUES (printf('display: %.1f', NEW.value));
END;

CREATE TRIGGER alarm AFTER UPDATE OF value ON sensor WHEN NEW.value > 30
BEGIN
    INSERT INTO alarms VALUES (printf('alarm: %.1f is above 30', NEW.value));
END;

UPDATE sensor SET value = 21.5 WHERE id = 1;
UPDATE sensor SET value = 30.5 WHERE id = 1;
DROP TRIGGER display;
UPDATE sensor SET value = 31.0 WHERE id = 1;

SELECT line FROM screen;
SELECT line FROM alarms;
```

```output for=sensor
display: 21.5
display: 30.5
alarm: 30.5 is above 30
alarm: 31.0 is above 30
```

- Verdict: idiom. A trigger runs its statements when a row of its table changes. The table is the subject, `CREATE TRIGGER` subscribes, and `DROP TRIGGER` unsubscribes.
- A trigger can only run SQL. It records the change in other tables, and a client reads them later. The example runs in SQLite.
- PostgreSQL has `LISTEN` and `NOTIFY`, which send a message to the connected clients that listen on a channel, when the transaction commits.
