```csharp run id=read-commands-and-collatz-from-one
var reader = new StringReader("add\nlist\nquit\nignored\n");
string? command;
do
{
    command = reader.ReadLine();
    Console.WriteLine($"command: {command}");
} while (command != "quit");

// The Collatz rule from 1: the do loop makes one round, the while loop none.
int n = 1, steps = 0;
do
{
    n = n % 2 == 0 ? n / 2 : 3 * n + 1;
    steps++;
} while (n != 1);
Console.WriteLine($"do: {steps} steps");

n = 1;
steps = 0;
while (n != 1)
{
    n = n % 2 == 0 ? n / 2 : 3 * n + 1;
    steps++;
}
Console.WriteLine($"while: {steps} steps");
```

```output for=read-commands-and-collatz-from-one
command: add
command: list
command: quit
do: 3 steps
while: 0 steps
```

- Verdict: native. The first loop must read a command before it can test it, so `command` is declared before the loop and the `do` form fits. The two Collatz loops have the same condition, but from 1 the `do` loop goes 1, 4, 2, 1 in 3 steps, and the `while` loop does nothing.
