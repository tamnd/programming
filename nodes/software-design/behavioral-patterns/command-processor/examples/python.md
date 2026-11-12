```python run id=processor
class Insert:
    def __init__(self, words, word):
        self.words, self.word = words, word

    def name(self):
        return f"insert {self.word}"

    def execute(self):
        self.words.append(self.word)

    def undo(self):
        self.words.pop()


class Processor:
    def __init__(self):
        self.done = []

    def do_it(self, command):
        print(f"do: {command.name()}")
        command.execute()
        self.done.append(command)

    def undo_it(self):
        command = self.done.pop()
        print(f"undo: {command.name()}")
        command.undo()


words, processor = [], Processor()
processor.do_it(Insert(words, "hello"))
processor.do_it(Insert(words, "world"))
print("text:", " ".join(words))
processor.undo_it()
print("text:", " ".join(words))
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. Any object with `name`, `execute` and `undo` methods is a command, and the processor keeps a list of the commands that it executed.
