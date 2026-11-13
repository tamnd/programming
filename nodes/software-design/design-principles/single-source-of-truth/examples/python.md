```python run id=status-list
# Two copies of the same knowledge.
menu_statuses = ("open", "closed", "archived")


def is_valid_copy(s):
    return s in ("open", "closed")  # not updated


# One source of truth.
STATUSES = ("open", "closed", "archived")
menu = STATUSES


def is_valid(s):
    return s in STATUSES


print(f"copies: menu {len(menu_statuses)}, archived valid {str(is_valid_copy('archived')).lower()}")
print(f"single source: menu {len(menu)}, archived valid {str(is_valid('archived')).lower()}")
```

```output
copies: menu 3, archived valid false
single source: menu 3, archived valid true
```
