---
tag: A05RNF
kind: concept
slug: aspect-oriented-programming
name: Aspect-oriented programming
aliases: [AOP, aspects, cross-cutting concerns, pointcuts and advice, weaving, interceptors]
area: paradigms/aspect-oriented
level: core
summary: >-
  Aspect-oriented programming puts a concern that cuts across many modules, such as logging or transactions, into one aspect, and a weaver applies its advice at the join points that a pointcut selects.
applies_to: core-languages
viz: optional
formal: none
history:
  year: 1997
  people: [Gregor Kiczales, Cristina Lopes, John Lamping]
  note: A group at Xerox PARC named aspect-oriented programming at ECOOP 1997 and then built AspectJ for Java.
see_also: [decorator-pattern, annotations, higher-order-functions, dependency-injection, runtime-reflection, template-method-pattern, object-oriented-programming, defer-and-finally]
same_as:
  wikidata: Q30267
  wikipedia: Aspect-oriented_programming
sources:
- url: https://www.cs.ubc.ca/~gregor/papers/kiczales-ECOOP1997-AOP.pdf
  title: Aspect-Oriented Programming, Gregor Kiczales and others, ECOOP 1997, LNCS 1241
  locator: abstract; section 5
- url: https://docs.spring.io/spring-framework/reference/core/aop/introduction-defn.html
  title: AOP Concepts, Spring Framework reference
  locator: AOP Concepts
- url: https://eclipse.dev/aspectj/doc/released/progguide/starting-aspectj.html
  title: Getting Started with AspectJ, The AspectJ Programming Guide
  locator: Introduction
---

## Idea

Aspect-oriented programming (AOP) puts a concern that cuts across many parts of a program into one module, called an aspect, and applies it to those parts by rule. Logging, security checks, transactions, caching and metrics are typical cross-cutting concerns: each must run in many methods, in many classes, and none belongs to any one of them. Gregor Kiczales and his colleagues at Xerox PARC named the problem in 1997. Some design decisions are "scattered throughout the code, resulting in 'tangled' code that is excessively difficult to develop and maintain", and the reason is "that they cross-cut the system's basic functionality."[^1] The example below keeps the audit concern out of the `Account` class, and applies it from outside to the methods that match a pattern:

```python run id=idea
import fnmatch
import functools


class Account:
    def __init__(self, balance):
        self.balance = balance

    def deposit(self, amount):
        self.balance += amount

    def withdraw(self, amount):
        if amount > self.balance:
            raise ValueError("insufficient funds")
        self.balance -= amount

    def report(self):
        return f"balance {self.balance}"


# An aspect has two parts: a pointcut that selects methods, and advice that runs around them.
def weave(cls, pointcut, advice):
    for name, member in list(vars(cls).items()):
        if callable(member) and fnmatch.fnmatch(name, pointcut):
            setattr(cls, name, advice(member))


def audit(method):
    @functools.wraps(method)
    def around(self, *args):
        print(f"before {method.__name__}{args}")
        try:
            result = method(self, *args)
        except Exception as e:
            print(f"after {method.__name__}: failed: {e}")
            raise
        print(f"after {method.__name__}: balance {self.balance}")
        return result

    return around


weave(Account, "[dw]*", audit)  # deposit and withdraw, but not report or __init__

acct = Account(100)
acct.deposit(50)
try:
    acct.withdraw(500)
except ValueError:
    pass
print(acct.report())
```

```output for=idea
before deposit(50,)
after deposit: balance 150
before withdraw(500,)
after withdraw: failed: insufficient funds
balance 150
```

The class has no line about auditing, and the audit code has no line about accounts. The call to `weave` connects them: it states where the aspect applies (the methods whose names start with `d` or `w`) and what it does there (print before and after the call). A new method named `deposit_all` would get the audit with no change to the aspect.

## How it works

**Join points, pointcuts and advice.** AOP names three parts. The Spring documentation defines them. A join point is "a point during the execution of a program, such as the execution of a method or the handling of an exception." Advice is the "action taken by an aspect at a particular join point." A pointcut is "a predicate that matches join points."[^2] An aspect groups pointcuts and advice for one concern. In AspectJ, a pointcut such as `execution(* Account.with*(..))` selects the execution of all methods of `Account` whose names start with `with`, and advice marked `before`, `after` or `around` runs at those points.

**Kinds of advice.** Before advice runs before the join point. After advice runs after it, and AspectJ splits it into after a normal return, after an exception, and after either one. Around advice runs instead of the join point and decides whether, when and how many times to proceed to it. Around advice can change the arguments and the result, and it can catch exceptions, so it can do all that the other kinds do. The function `around` in the example is around advice.

**Weaving.** A weaver joins the aspects with the base code. AspectJ weaves at compile time, by changing the bytecode of the classes it compiles, or at load time, by changing classes as the JVM loads them. Spring AOP and the proxy libraries of .NET weave at run time: they put a proxy object in front of the real object, and the proxy runs the advice before it passes each call on. Python decorators and JavaScript proxies are run-time weaving by hand. Kiczales's paper describes the weaver as the part that "handles these details, instead of the programmer having to do the tangling manually."[^1]

**Inter-type declarations.** AspectJ also lets an aspect add fields and methods to other classes, or declare that a class implements an interface. The AspectJ guide says that "pointcuts and advice dynamically affect program flow, inter-type declarations statically affects a program's class hierarchy".[^3] This makes it possible to put all the code of one concern, data as well as behavior, in one file.

**Obliviousness.** In full AOP, the base code does not know that aspects apply to it. This is the source of both the power and the main criticism of AOP: a reader of a method cannot see from its text what runs when it is called. Many later systems give up some of this power for clarity. They require a visible marker at each join point, such as an annotation or a decorator, so the reader sees that something more happens.

## In languages

| Language | Verdict | Mechanism |
|---|---|---|
| Python | idiom | decorators, `setattr` on classes |
| JavaScript | stdlib | `Proxy` and `Reflect` |
| TypeScript | native | decorators; `Proxy` |
| Java | library | AspectJ, Spring AOP; `java.lang.reflect.Proxy` in the JDK |
| Kotlin | library | as Java |
| C# | stdlib | `DispatchProxy`; weavers in libraries |
| Go | idiom | middleware that wraps a handler |
| Rust | idiom | wrapper functions; attribute macros in crates |
| C | absent | wrapper functions; instrumentation hooks of the compiler and the linker |
| C++ | idiom | generic wrappers; AspectC++ |
| Bash | idiom | `trap` with `DEBUG` and `RETURN` |
| PowerShell | idiom | proxy functions |
| SQL | idiom | `BEFORE` and `AFTER` triggers |
| Regex | n/a | no functions |
| HTML | n/a | no functions |
| CSS | n/a | no functions |

Java is the home of AOP. AspectJ came from the group at Xerox PARC that named the idea, and the AspectJ guide states its aim: "Aspect-oriented programming is a way of modularizing crosscutting concerns much like object-oriented programming is a way of modularizing common concerns."[^3] Most Java programmers meet AOP through Spring, where annotations such as `@Transactional` and `@Cacheable` mark methods, and proxies apply the advice. The Java file of this page shows the main limit of proxies: a call from inside the object to another of its methods does not pass through the proxy, so it gets no advice.

The other core languages have no weaver, and they use the parts of AOP that their features allow. Python decorators, TypeScript decorators and C# attributes put a visible marker on each join point, and a function or a framework adds the behavior. JavaScript `Proxy` and C# `DispatchProxy` intercept calls at run time. Go middleware, and wrapper functions in Rust, C and C++, are around advice that the programmer applies by hand. These forms keep the concern in one place, which is the main benefit of AOP, but they give up pointcuts that select join points by pattern across the whole program.

## Variations and related ideas

- **Decorators.** A function that wraps a function, or a class that wraps an object with the same interface. See the [decorator pattern](/decorator-pattern/) and [higher-order functions](/higher-order-functions/).
- **Annotations and attributes.** Metadata on declarations that a framework reads to apply behavior. See [annotations](/annotations/).
- **Middleware and interceptors.** Chains of handlers in web frameworks, RPC systems and message buses, each of which can act before and after the next one.
- **Dependency injection containers.** Containers often apply aspects when they build objects, by giving out proxies. See [dependency injection](/dependency-injection/).
- **Metaobject protocols.** Kiczales's earlier work let programs change how the language runs method calls and object creation. AOP grew out of it. See [runtime reflection](/runtime-reflection/).
- **Template method.** A base class fixes the steps around a hook that subclasses fill in. It is advice by inheritance, applied to one class at a time. See the [template method pattern](/template-method-pattern/).
- **Context managers and defer.** `with`, `using`, `defer` and `finally` put code before and after a block at one place in the source. See [defer and finally](/defer-and-finally/).

## Pitfalls

- **Invisible behavior.** With pointcuts that match by pattern, a reader of a method cannot see which advice runs on it. Use tools that show the advice on each method, or require a marker such as an annotation at each join point.
- **Fragile pointcuts.** A pointcut that matches names by pattern breaks silently when a method is renamed, and it can start to match a new method by accident. Match on annotations or types, not on names.
- **Self-invocation in proxies.** A call from inside an object to its own method goes to `this`, not to the proxy, so the advice does not run. The Java file shows this. Spring's `@Transactional` on a method called from the same class is a well known case.
- **Lost results and exceptions.** Around advice must return the result of the join point and must let exceptions through unless it handles them on purpose. The wrong block in the Python file returns `None` for every call.
- **Order of aspects.** When two aspects apply to one join point, the order matters: a retry aspect outside a transaction aspect retries the whole transaction, but inside it retries only part of it. State the order explicitly.
- **Debugging.** Stack traces contain proxy and advice frames, and a debugger can step into woven code that is not in any source file.

## Cost

Compile-time and load-time weaving put the advice into the bytecode, so the cost at run time is the cost of the advice itself plus a method call or two. Load-time weaving adds time at startup, because each class is changed as it loads. Proxies cost more on each call: a reflective call through `Proxy` or `DispatchProxy` boxes the arguments into an array and calls the target by reflection, which is many times slower than a direct call. Libraries that generate bytecode for proxies reduce this cost. Wrapper functions in Rust and C++ are usually inlined and cost nothing. The larger cost of AOP is in understanding: each aspect is one more thing that a reader must know about to predict what a call does.

## History

AOP came from research on reflection and metaobject protocols at Xerox PARC. Gregor Kiczales, who had worked on the metaobject protocol of the Common Lisp Object System, led a group that presented "Aspect-Oriented Programming" at ECOOP in June 1997.[^1] The paper built several domain-specific aspect languages and a weaver for each. In one of its examples, a weaver of 1959 lines produced code with performance "comparable to a 35213 line manually tangled version".[^1] The group then built AspectJ, a general aspect language for Java, and released it from about 1998. AspectJ moved to the Eclipse Foundation in 2002. The Spring Framework made proxy-based AOP part of Java enterprise programming from 2004, and annotations in Java 5 gave a way to mark join points in the source. After the 2000s, interest in full AOP fell, and its ideas lived on in decorators, middleware, annotations and interceptors in many languages.

## References
