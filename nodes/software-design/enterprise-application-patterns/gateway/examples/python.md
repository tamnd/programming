```python run id=shipping-gateway
# The foreign API of a shipping service. Its names and types do not fit our code:
# the weight is in grams, the flags are a string, and the result is a dict
# with a return code and the amount as a string of cents.
def foreign_quote(weight_grams, dest, svc_flags):
    print(f"  connection: quote(weight_grams={weight_grams}, dest={dest}, svc_flags={svc_flags})")
    if weight_grams > 30000:
        return {"rc": 17, "msg": "too heavy"}
    return {"rc": 0, "amt": str(500 + weight_grams // 100 * 10), "cur": "EUR"}


class NoService(Exception):
    pass


class ShippingGateway:
    """Our interface to the shipping service, in the terms of our code."""

    def __init__(self, connection):
        self.connection = connection  # the call to the foreign API; a test can replace it

    def standard_cost(self, kilograms, country):
        reply = self.connection(round(kilograms * 1000), country, "STD")
        if reply["rc"] != 0:
            raise NoService(f"code {reply['rc']}: {reply['msg']}")
        return int(reply["amt"]), reply["cur"]


def show(gateway, kilograms, country):
    try:
        cents, currency = gateway.standard_cost(kilograms, country)
        print(f"parcel of {kilograms:.1f} kg to {country} costs {cents // 100}.{cents % 100:02d} {currency}")
    except NoService as e:
        print(f"parcel of {kilograms:.1f} kg to {country}: no service ({e})")


gateway = ShippingGateway(foreign_quote)
show(gateway, 1.2, "DE")
show(gateway, 31.0, "DE")

# A test gives the gateway a connection that returns canned data.
canned = ShippingGateway(lambda g, d, f: {"rc": 0, "amt": "999", "cur": "EUR"})
show(canned, 2.0, "FR")
```

```output
  connection: quote(weight_grams=1200, dest=DE, svc_flags=STD)
parcel of 1.2 kg to DE costs 6.20 EUR
  connection: quote(weight_grams=31000, dest=DE, svc_flags=STD)
parcel of 31.0 kg to DE: no service (code 17: too heavy)
parcel of 2.0 kg to FR costs 9.99 EUR
```

- Verdict: idiom. A gateway is a normal class. `ShippingGateway` turns kilograms into grams, the return code into the `NoService` exception, and the string of cents into an integer.
- The connection is a function that the gateway takes in its constructor. The last call gives the gateway a lambda with canned data, as a test does.
