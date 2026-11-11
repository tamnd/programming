```python run id=order
import functools


def tag(label):
    def aspect(fn):
        @functools.wraps(fn)
        def around(*args):
            print(f"enter {label}")
            result = fn(*args)
            print(f"leave {label}")
            return result

        return around

    return aspect


@tag("outer")
@tag("inner")
def work(x):
    print("work", x)
    return x * 2


print(work(21), work.__name__)
```

```output for=order
enter outer
enter inner
work 21
leave inner
leave outer
42 work
```

```python wrong id=lost-result
def log(fn):
    def around(*args):
        print("calling", fn.__name__)
        fn(*args)

    return around


@log
def total(xs):
    return sum(xs)


print(total([1, 2, 3]), total.__name__)
```

```output for=lost-result
calling total
None around
```

- Verdict: idiom. Python has no pointcuts or weaver in the language, but decorators and `setattr` on classes give before, after and around advice. The function `weave` in the first block of this page picks methods by a name pattern, as a pointcut does.
- Stacked decorators apply from the bottom up, so the top one runs first and finishes last. The order of aspects is the order of the decorators.
- In the wrong block, the advice calls the function but does not return its result, so every call returns `None`. It also does not use `functools.wraps`, so the function loses its name. An around advice must return what the advised function returns.
