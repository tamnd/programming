```csharp run id=orders
var repo = new InMemoryOrderRepository();
repo.Add(new Order(1, "ana", 30));
repo.Add(new Order(2, "bo", 12));
repo.Add(new Order(3, "ana", 5));
Console.WriteLine("ana: " + string.Join(", ", repo.ByCustomer("ana").Select(o => $"order {o.Id} ({o.Total})")));
Console.WriteLine(ShipOrder(repo, 1));
Console.WriteLine(ShipOrder(repo, 9));
var stored = repo.Get(1)!;
Console.WriteLine($"stored: order {stored.Id} is {stored.Status.ToString().ToLowerInvariant()}");

static string ShipOrder(IOrderRepository orders, int id)
{
    if (orders.Get(id) is not { } order) return $"order {id}: not found";
    var shipped = order.Ship();
    orders.Update(shipped);
    return $"order {id} is {shipped.Status.ToString().ToLowerInvariant()}";
}

enum Status { Open, Shipped }

record Order(int Id, string Customer, int Total, Status Status = Status.Open)
{
    public Order Ship() => Status == Status.Open
        ? this with { Status = Status.Shipped }
        : throw new InvalidOperationException($"order {Id} is {Status}");
}

interface IOrderRepository
{
    void Add(Order order);
    Order? Get(int id);
    IReadOnlyList<Order> ByCustomer(string customer);
    void Update(Order order);
}

sealed class InMemoryOrderRepository : IOrderRepository
{
    private readonly SortedDictionary<int, Order> _rows = new();

    public void Add(Order order)
    {
        if (!_rows.TryAdd(order.Id, order)) throw new ArgumentException($"order {order.Id} exists");
    }

    public Order? Get(int id) => _rows.GetValueOrDefault(id);

    // A list, not an IQueryable: the query stays inside the repository.
    public IReadOnlyList<Order> ByCustomer(string customer) =>
        _rows.Values.Where(o => o.Customer == customer).ToList();

    public void Update(Order order)
    {
        if (!_rows.ContainsKey(order.Id)) throw new KeyNotFoundException($"order {order.Id}");
        _rows[order.Id] = order;
    }
}
```

```output
ana: order 1 (30), order 3 (5)
order 1 is shipped
order 9: not found
stored: order 1 is shipped
```

- Verdict: idiom. An interface gives the contract, and an immutable record is the domain object. `with` makes the changed copy in `Ship`.
- `ByCustomer` returns a materialized list. A repository that returns `IQueryable<Order>` lets each caller add query operators, so the queries leave the repository, and a test double cannot replace them. The Entity Framework Core documentation on testing says to return `IEnumerable` for this reason.
- With Entity Framework Core, `DbContext` already combines a unit of work and repositories, and `DbSet<Order>` is a collection of orders. The node page discusses when a repository on top of it adds nothing.
