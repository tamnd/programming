```python run id=frames
import sys


def count(n):
    here = n * 10                    # a local: one copy in each frame
    print(f"enter n={n} here={here}")
    if n > 1:
        count(n - 1)
    else:
        frames = []
        f = sys._getframe()          # the frame of this call
        while f.f_code is count.__code__:
            frames.append(f"n={f.f_locals['n']}")
            f = f.f_back             # the frame of the caller
        print("frames:", " <- ".join(frames))
    print(f"leave n={n} here={here}")


count(3)
```

```output for=frames
enter n=3 here=30
enter n=2 here=20
enter n=1 here=10
frames: n=1 <- n=2 <- n=3
leave n=1 here=10
leave n=2 here=20
leave n=3 here=30
```

- Verdict: native. Each call has a frame object. `f_locals` holds the locals of the call, and `f_back` is the frame of the caller.
