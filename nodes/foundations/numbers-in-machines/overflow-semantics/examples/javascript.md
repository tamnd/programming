```javascript run id=limits
const yesNo = (b) => (b ? "yes" : "no");
const max32 = 2147483647;
console.log(`max32 + 1: ${max32 + 1}`);
console.log(`(max32 + 1) | 0: ${(max32 + 1) | 0}`);

const safe = Number.MAX_SAFE_INTEGER;
console.log(`safe + 1 === safe + 2: ${yesNo(safe + 1 === safe + 2)}`);
console.log(`BigInt(safe) + 2n: ${BigInt(safe) + 2n}`);
console.log(`BigInt.asIntN(32, 2147483648n): ${BigInt.asIntN(32, 2147483648n)}`);

const addInt32 = (a, b) => {
  const sum = a + b;
  if (sum > max32 || sum < -max32 - 1) throw new RangeError("int32 overflow");
  return sum;
};
try {
  addInt32(max32, 1);
} catch (e) {
  console.log(`addInt32(max32, 1): ${e.name}`);
}
```

```output for=limits
max32 + 1: 2147483648
(max32 + 1) | 0: -2147483648
safe + 1 === safe + 2: yes
BigInt(safe) + 2n: 9007199254740993
BigInt.asIntN(32, 2147483648n): -2147483648
addInt32(max32, 1): RangeError
```

- Verdict: idiom. A JavaScript number is a float, so a sum is rounded, not wrapped. The bitwise operators such as `| 0` wrap a value to 32 bits.
- Above `Number.MAX_SAFE_INTEGER`, two different sums can give the same number. `BigInt` grows, and `BigInt.asIntN` wraps on request.
