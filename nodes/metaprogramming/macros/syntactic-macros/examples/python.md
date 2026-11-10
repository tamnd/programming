```python run id=ast-transform
import ast

source = "total = price * quantity"
tree = ast.parse(source)


class CheckedMultiply(ast.NodeTransformer):
    def visit_BinOp(self, node):
        self.generic_visit(node)
        if isinstance(node.op, ast.Mult):
            return ast.Call(
                func=ast.Name("checked_mul", ast.Load()),
                args=[node.left, node.right],
                keywords=[],
            )
        return node


tree = ast.fix_missing_locations(CheckedMultiply().visit(tree))
print(ast.unparse(tree))


def checked_mul(a, b):
    result = a * b
    if result > 1000:
        raise OverflowError(f"{a} * {b} is more than 1000")
    return result


namespace = {"checked_mul": checked_mul, "price": 30, "quantity": 4}
exec(compile(tree, "<generated>", "exec"), namespace)
print(namespace["total"])
```

```output for=ast-transform
total = checked_mul(price, quantity)
120
```

- Verdict: absent. Python has no macros. The compiler does not let a program change the syntax tree of the program that it is compiling.
- The `ast` module parses source text into a tree, `ast.NodeTransformer` rewrites it, and `compile` turns the result into code. This is the work of a syntactic macro, done at run time on code that the program holds as text.
- Decorators change a function after it is defined. They work on the function object, not on its syntax.
