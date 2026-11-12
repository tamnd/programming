This area answers the first questions about a program: what it is, how a machine runs it, and how the machine stores numbers and text. The pages here are short on language detail. They give the model that the other areas use.

## What it holds

- **What a program is.** What a program is, how source code becomes a running program, and what the first program of a language looks like.
- **Computation in practice.** What computation is, seen from a working program: algorithms, state machines, side effects, the limits of what programs can decide, and why some programs never stop.
- **Bits and representation.** How values become bits: bits, bytes and words, number bases, byte order, and the bitwise operators that read and change single bits.
- **Numbers in machines.** How machines store integers and real numbers, the limits of each form, and the jobs that every program does with numbers.
- **Characters and text.** How a machine stores a character as a number, and why text and bytes are different things. Unicode in depth and the encodings go to data-and-persistence/text-encodings. The string type goes to data-and-values/strings.
- **The machine model.** The simple model of a computer that a programmer needs: a processor that runs instructions, registers, and memory with addresses.
- **The stack and the heap.** Where a running program keeps its data: the call stack with its frames, the heap, and the static area. Allocation methods and lifetimes go to memory-and-resources.
- **Compilation and interpretation.** How source code becomes something that runs, as the user of a language sees it. How to build a compiler or an interpreter goes to language-implementation.
- **Logic for programs.** The logic that conditions use: truth values, truth tables, the laws that simplify conditions, and the rules that languages use to turn values into true or false. Boolean algebra as mathematics links to math2.

## What it does not hold

- How to build a compiler or an interpreter. That is in [Language implementation](/language-implementation/). This area gives only the overview.
- Unicode in depth, such as normalization and the UTF encodings as formats. That is in [Data and persistence](/data-and-persistence/), under text encodings.
- The theory of computation as mathematics, such as proofs about Turing machines. This site links out for it.
- The analysis of algorithms. That is on [the algorithm site](https://algorithm.tamnd.com/).
