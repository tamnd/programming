```python run id=child
import os
import subprocess

SCRIPT = 'echo "child sees GREETING=$GREETING"; GREETING=changed; echo "child set GREETING=$GREETING"; exit 3'

greeting = "hello"
env = dict(os.environ, GREETING=greeting)
child = subprocess.Popen(["sh", "-c", SCRIPT], env=env, stdout=subprocess.PIPE, text=True)
out, _ = child.communicate()
print(out, end="")
print("child exit status:", child.returncode)
print("child pid differs:", child.pid != os.getpid())
print("parent greeting:", greeting)
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: True
parent greeting: hello
```

- Verdict: stdlib. `subprocess` starts other programs, `multiprocessing` runs Python functions in child processes, and `os.fork` is the POSIX call.
- `Popen` starts the child and returns at once. `communicate()` reads the output of the child and waits for it to end, so `returncode` is set after it.
- The child gets a copy of the environment, with `GREETING` added. The assignment in the shell changes only that copy.
- For CPU-bound Python code, `multiprocessing` and `ProcessPoolExecutor` run work in parallel in separate processes, as the Idea section shows.
