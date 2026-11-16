```csharp run id=packed
using System;

static byte[] ToPacked(long n)
{
    string s = Math.Abs(n).ToString();
    int size = (s.Length + 2) / 2;
    var digits = new int[size * 2];
    int start = size * 2 - 1 - s.Length;
    for (int i = 0; i < s.Length; i++)
    {
        digits[start + i] = s[i] - '0';
    }
    digits[size * 2 - 1] = n < 0 ? 0xD : 0xC;
    var output = new byte[size];
    for (int i = 0; i < size; i++)
    {
        output[i] = (byte)(digits[2 * i] << 4 | digits[2 * i + 1]);
    }
    return output;
}

static int BcdAddByte(int a, int b, out bool carry)
{
    int s = a + b;
    if ((s & 0x0F) > 9 || (a & 0x0F) + (b & 0x0F) > 0x0F)
    {
        s += 0x06;
    }
    if (s > 0x99)
    {
        s += 0x60;
    }
    carry = s > 0xFF;
    return s & 0xFF;
}

Console.WriteLine($"packed -12345: {Convert.ToHexString(ToPacked(-12345))}");
Console.WriteLine($"packed 127: {Convert.ToHexString(ToPacked(127))}");
int r = BcdAddByte(0x45, 0x38, out bool c);
Console.WriteLine($"45 + 38: {r:X2} carry {(c ? "yes" : "no")}");
r = BcdAddByte(0x99, 0x99, out c);
Console.WriteLine($"99 + 99: {r:X2} carry {(c ? "yes" : "no")}");
```

```output for=packed
packed -12345: 12345D
packed 127: 127C
45 + 38: 83 carry no
99 + 99: 98 carry yes
```

- Verdict: idiom. C# has no BCD type. `Convert.ToHexString` shows the packed bytes, and each hexadecimal digit is one BCD digit or the sign.
- `BcdAddByte` adds two packed bytes with the add-6 correction and returns the decimal carry, so 99 + 99 gives 98 and a carry for 198.
