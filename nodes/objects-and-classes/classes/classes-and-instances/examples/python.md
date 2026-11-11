```python run id=class-and-instance-attributes
class Account:
    rate = 0.02  # class attributes: one value, shared by all instances
    opened = 0

    def __init__(self, owner, balance=0):
        self.owner = owner  # instance attributes: one set per object
        self.balance = balance
        Account.opened += 1

    def deposit(self, amount):  # a method: a function stored in the class
        self.balance += amount
        return self

    def interest(self):
        return round(self.balance * self.rate, 2)

    def __repr__(self):
        return f"Account({self.owner!r}, {self.balance})"


a = Account("ana", 100)
b = Account("bo")
a.deposit(50)
print(a, b, Account.opened)
print(a.interest())
print(Account.deposit(b, 10))  # a.deposit(10) means Account.deposit(a, 10)
print(type(a) is Account, type(Account))  # a class is an object too
Savings = type("Savings", (Account,), {"rate": 0.05})  # a class made at run time
print(Savings("cy", 100).interest(), Account.opened)
```

```output for=class-and-instance-attributes
Account('ana', 150) Account('bo', 0) 2
3.0
Account('bo', 10)
True <class 'type'>
5.0 3
```

```python wrong id=shared-class-attribute
class Cart:
    items = []  # a class attribute: one list, stored in the class

    def add(self, item):
        self.items.append(item)  # finds the class's list and changes it


a = Cart()
b = Cart()
a.add("tea")
print(b.items, a.items is b.items)
```

```output for=shared-class-attribute
['tea'] True
```

- Verdict: native. A `class` statement creates a class object that holds the methods and the class attributes. Each instance has its own dictionary of attributes, which `__init__` usually fills. A lookup such as `a.rate` searches the instance first and then the class, so instances share class attributes until one assigns its own. `a.deposit(10)` is a short form of `Account.deposit(a, 10)`, which is why each method takes `self` as its first parameter. A class is itself an object, of type `type`, and a program can create a new class at run time by calling `type`.
- The wrong block puts a list in a class attribute. Every instance finds the same list through the class, so an item added through `a` appears in `b`. Create mutable state in `__init__`, so that each instance gets its own.
