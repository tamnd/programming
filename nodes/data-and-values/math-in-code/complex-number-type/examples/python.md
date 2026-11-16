```python run id=complex-and-cmath
import cmath

z = complex(3, 4)
print(z, z.real, z.imag, abs(z))

# The polar form: the length and the angle in radians.
r, phi = cmath.polar(1j)
print(r, phi)
print(cmath.rect(2, 0))

# complex() parses a string with no spaces.
print(complex("1+2j"), complex("-3.5j"))

# cmath accepts negative numbers; math does not.
print(cmath.sqrt(-4), cmath.exp(0j))
print(cmath.isclose(cmath.exp(1j * cmath.pi), -1))
```

```output for=complex-and-cmath
(3+4j) 3.0 4.0 5.0
1.0 1.5707963267948966
(2+0j)
(1+2j) -3.5j
2j (1+0j)
True
```

- Verdict: native. `complex` is a built-in type, and a number with the suffix `j` is an imaginary literal. `abs` gives the length, `cmath.polar` and `cmath.rect` convert to and from the polar form, and `complex()` parses a string such as `"1+2j"`. `cmath.exp(1j * cmath.pi)` is close to -1 but not equal to it, so the comparison uses `cmath.isclose`.
