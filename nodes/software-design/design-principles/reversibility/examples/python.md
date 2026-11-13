```python run id=reversible-migration
table = {"columns": ["id", "name"]}

add_email = {
    "up": lambda t: t["columns"].append("email"),
    "down": lambda t: t["columns"].remove("email"),
}

print(f"before: {', '.join(table['columns'])}")
add_email["up"](table)
print(f"up:     {', '.join(table['columns'])}")
add_email["down"](table)  # the decision is reversed
print(f"down:   {', '.join(table['columns'])}")
```

```output
before: id, name
up:     id, name, email
down:   id, name
```
