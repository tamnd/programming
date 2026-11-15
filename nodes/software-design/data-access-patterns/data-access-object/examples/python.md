```python run id=two-daos-one-interface
from abc import ABC, abstractmethod
from dataclasses import dataclass


@dataclass
class Player:
    # The transfer object: plain data that goes in and out of the DAO.
    id: int
    name: str
    country: str


class PlayerDao(ABC):
    # The interface that the business code knows. It names no storage.
    @abstractmethod
    def insert(self, p): ...

    @abstractmethod
    def find(self, id): ...

    @abstractmethod
    def find_by_country(self, country): ...

    @abstractmethod
    def find_all(self): ...

    @abstractmethod
    def update(self, p): ...

    @abstractmethod
    def delete(self, id): ...


class MemoryPlayerDao(PlayerDao):
    # The data source is a map from key to record.
    def __init__(self):
        self.rows = {}

    def insert(self, p):
        self.rows[p.id] = (p.name, p.country)

    def find(self, id):
        name, country = self.rows[id]
        return Player(id, name, country)

    def find_by_country(self, country):
        return [self.find(id) for id in sorted(self.rows) if self.rows[id][1] == country]

    def find_all(self):
        return [self.find(id) for id in sorted(self.rows)]

    def update(self, p):
        self.rows[p.id] = (p.name, p.country)

    def delete(self, id):
        del self.rows[id]


class TextPlayerDao(PlayerDao):
    # The data source is a flat file: one line "id;name;country" for each player.
    def __init__(self):
        self.lines = []

    def _parse(self, line):
        id, name, country = line.split(";")
        return Player(int(id), name, country)

    def _index(self, id):
        return next(i for i, line in enumerate(self.lines) if self._parse(line).id == id)

    def insert(self, p):
        self.lines.append(f"{p.id};{p.name};{p.country}")

    def find(self, id):
        return self._parse(self.lines[self._index(id)])

    def find_by_country(self, country):
        return [p for p in self.find_all() if p.country == country]

    def find_all(self):
        return sorted((self._parse(line) for line in self.lines), key=lambda p: p.id)

    def update(self, p):
        self.lines[self._index(p.id)] = f"{p.id};{p.name};{p.country}"

    def delete(self, id):
        del self.lines[self._index(id)]


def dao_factory(kind):
    # The one place that knows which storage the program uses.
    return {"memory": MemoryPlayerDao, "text": TextPlayerDao}[kind]()


class PlayerService:
    # The business object. It uses only the DAO interface.
    def __init__(self, dao):
        self.dao = dao

    def transfer(self, id, country):
        p = self.dao.find(id)
        p.country = country
        self.dao.update(p)


def names(players):
    return " | ".join(f"{p.name} {p.country}" for p in players)


daos = {}
for kind in ["memory", "text"]:
    dao = daos[kind] = dao_factory(kind)
    service = PlayerService(dao)
    print("dao:", type(dao).__name__)
    for p in [Player(1, "Ann", "NZ"), Player(2, "Bo", "NZ"), Player(3, "Cy", "GB")]:
        dao.insert(p)
    print("all:", names(dao.find_all()))
    print("find 1:", names([dao.find(1)]))
    service.transfer(1, "AU")
    print("from NZ:", names(dao.find_by_country("NZ")))
    print("from AU:", names(dao.find_by_country("AU")))
    dao.delete(3)
    print("all:", names(dao.find_all()))

# The business code never saw this format; only TextPlayerDao knows it.
print("lines in the text store:", " | ".join(daos["text"].lines))
```

```output
dao: MemoryPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
dao: TextPlayerDao
all: Ann NZ | Bo NZ | Cy GB
find 1: Ann NZ
from NZ: Bo NZ
from AU: Ann AU
all: Ann AU | Bo NZ
lines in the text store: 1;Ann;AU | 2;Bo;NZ
```

- Verdict: idiom. `PlayerDao` is an abstract base class, so a DAO that leaves out a method cannot be created. The transfer object is a dataclass.
- The two DAOs keep the data in different forms, a map of records and a list of text lines, and both give back `Player` objects. A DAO for SQLite, as in the Solution section, has the same methods.
- `dao_factory` is the only code that names a class of DAO.
