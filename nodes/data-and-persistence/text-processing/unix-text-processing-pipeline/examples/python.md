```python run id=python-popen-pipeline
import os
import subprocess
import tempfile

text = 'The cat sat on the mat. The dog sat on the log.\nA cat and a dog: the cat ran.\n'
env = {'LC_ALL': 'C', 'PATH': os.environ['PATH']}

# tr -cs 'A-Za-z' '\n' < words.txt | tr 'A-Z' 'a-z' | sort | uniq -c, with no shell
commands = [['tr', '-cs', 'A-Za-z', '\n'], ['tr', 'A-Z', 'a-z'], ['sort'], ['uniq', '-c']]

with tempfile.TemporaryDirectory() as d:
    path = os.path.join(d, 'words.txt')
    with open(path, 'w') as f:
        f.write(text)
    with open(path) as source:
        processes = []
        stdin = source
        for command in commands:
            p = subprocess.Popen(command, stdin=stdin, stdout=subprocess.PIPE, env=env)
            if processes:
                processes[-1].stdout.close()  # only p reads this pipe now
            processes.append(p)
            stdin = p.stdout
        output = processes[-1].communicate()[0]
    statuses = [p.wait() for p in processes]

print(output.decode(), end='')
print('statuses:', statuses)
```

```output
      2 a
      1 and
      3 cat
      2 dog
      1 log
      1 mat
      2 on
      1 ran
      2 sat
      5 the
statuses: [0, 0, 0, 0]
```

The program starts one process for each stage with `subprocess.Popen`, and gives the standard output of each process to the next one as its standard input. After it starts a stage, it closes its own copy of the read end of the pipe before it, so that only the new stage reads that pipe. `communicate` reads the output of the last stage, and `wait` gives the status of each stage, as `PIPESTATUS` does in Bash. The program passes each argument as an item of a list, so no shell splits or changes the arguments.
