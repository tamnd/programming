```javascript run id=shipping-gateway
// The foreign API of a shipping service. Its names and types do not fit our code:
// the weight is in grams, the flags are a string, and the result is an object
// with a return code and the amount as a string of cents.
function foreignQuote(weightGrams, dest, svcFlags) {
  console.log(`  connection: quote(weight_grams=${weightGrams}, dest=${dest}, svc_flags=${svcFlags})`);
  if (weightGrams > 30000) return { rc: 17, msg: "too heavy" };
  return { rc: 0, amt: String(500 + Math.floor(weightGrams / 100) * 10), cur: "EUR" };
}

class NoService extends Error {}

// Our interface to the shipping service, in the terms of our code.
class ShippingGateway {
  constructor(connection) {
    this.connection = connection; // the call to the foreign API; a test can replace it
  }

  standardCost(kilograms, country) {
    const reply = this.connection(Math.round(kilograms * 1000), country, "STD");
    if (reply.rc !== 0) throw new NoService(`code ${reply.rc}: ${reply.msg}`);
    return { cents: Number(reply.amt), currency: reply.cur };
  }
}

function show(gateway, kilograms, country) {
  const kg = kilograms.toFixed(1);
  try {
    const { cents, currency } = gateway.standardCost(kilograms, country);
    const amount = `${Math.floor(cents / 100)}.${String(cents % 100).padStart(2, "0")}`;
    console.log(`parcel of ${kg} kg to ${country} costs ${amount} ${currency}`);
  } catch (e) {
    if (!(e instanceof NoService)) throw e;
    console.log(`parcel of ${kg} kg to ${country}: no service (${e.message})`);
  }
}

const gateway = new ShippingGateway(foreignQuote);
show(gateway, 1.2, "DE");
show(gateway, 31.0, "DE");

// A test gives the gateway a connection that returns canned data.
const canned = new ShippingGateway(() => ({ rc: 0, amt: "999", cur: "EUR" }));
show(canned, 2.0, "FR");
```

```output
  connection: quote(weight_grams=1200, dest=DE, svc_flags=STD)
parcel of 1.2 kg to DE costs 6.20 EUR
  connection: quote(weight_grams=31000, dest=DE, svc_flags=STD)
parcel of 31.0 kg to DE: no service (code 17: too heavy)
parcel of 2.0 kg to FR costs 9.99 EUR
```

- Verdict: idiom. A class with one method is the gateway, and the connection is a function. `NoService` extends `Error`, so `show` can tell it apart from other errors.
- `toFixed(1)` prints the weight with one decimal, as the other examples do.
