```csharp run id=fall-through-csharp
using System;

static string Rights(int level)
{
    string out_ = "";
    switch (level)
    {
        case 3:
            out_ += " delete";
            goto case 2;
        case 2:
            out_ += " write";
            goto case 1;
        case 1:
            out_ += " read";
            break;
        default:
            out_ += " none";
            break;
    }
    return out_;
}

Console.WriteLine("3:" + Rights(3));
Console.WriteLine("2:" + Rights(2));
Console.WriteLine("0:" + Rights(0));
```

```output for=fall-through-csharp
3: delete write read
2: write read
0: none
```

- Verdict: idiom. C# does not allow fall-through from a section with statements: each section ends with `break`, `goto`, `return` or `throw`. `goto case 2;` jumps to the section for 2, and so gives the effect of fall-through on request.
