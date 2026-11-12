```typescript run id=act
type Token = number;
type Request = [Token, string];

function service(requests: Request[]): Array<[Token, string]> {
  // replies in the reverse order; the token comes back unchanged
  return [...requests].reverse().map(([token, name]): [Token, string] => [token, `${name} loaded`]);
}

const pending = new Map<Token, string>();
const requests: Request[] = [];
["image-7", "image-9"].forEach((name, i) => {
  const token: Token = i + 1;
  pending.set(token, name);
  requests.push([token, name]);
  console.log(`send ${token}: load ${name}`);
});

for (const [token, result] of service(requests)) {
  pending.delete(token);
  console.log(`done ${token}: ${result}`);
}
```

```output
send 1: load image-7
send 2: load image-9
done 2: image-9 loaded
done 1: image-7 loaded
```

- Verdict: idiom. The alias `Token` names the role of the number, and the map type fixes what the client stores for each token.
