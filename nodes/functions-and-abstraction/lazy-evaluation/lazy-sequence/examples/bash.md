```bash run id=pipeline
seq 1 1000000000 | awk '{ print $1 * $1 }' | awk '$1 % 2 == 1' | head -n 2
```

```output for=pipeline
1
9
```

- Verdict: idiom. The processes of a pipeline run at the same time, and each one reads lines only as it needs them. When `head` has two lines, it exits. The next write of the second `awk` then fails with `SIGPIPE`, and the stop moves back to `seq`, so the billion numbers are never all produced.
- The pipe has a buffer, so a producer can run some lines ahead of its consumer. The stages are lazy at the level of buffers, not of single elements.
