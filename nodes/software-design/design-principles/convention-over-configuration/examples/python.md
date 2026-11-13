```python run id=table-names
def table_for(model):
    # The convention: the table is the class name in lower case, plus "s".
    return getattr(model, "table", None) or f"{model.__name__.lower()}s"


class Sale:
    pass


class Order:
    pass


class ProductSale:
    table = "product_sales"  # the one deviation needs configuration


for model in (Sale, Order, ProductSale):
    print(f"{model.__name__} -> {table_for(model)}")
```

```output
Sale -> sales
Order -> orders
ProductSale -> product_sales
```
