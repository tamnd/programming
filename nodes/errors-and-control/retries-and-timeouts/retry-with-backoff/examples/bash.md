```bash run id=backoff
#!/usr/bin/env bash
# Retry a command with capped exponential backoff and full jitter.
# The script keeps time in a counter and does not sleep, so it runs at once.

seed=12345
next_random() {
  # A linear congruential generator with a fixed seed, so each run gives the same delays.
  seed=$(( (seed * 1103515245 + 12345) % 2147483648 ))
  rnd=$(( seed / 65536 ))
}

calls=0
flaky() {
  # Exit code 75 is EX_TEMPFAIL: a temporary failure that is worth a retry.
  calls=$(( calls + 1 ))
  if (( calls <= $1 )); then return 75; fi
  echo "  ok after $calls calls"
}

usage_error() {
  # Exit code 64 is EX_USAGE: the command was called wrongly, and a retry cannot help.
  calls=$(( calls + 1 ))
  return 64
}

retry() {
  local attempts=5 base=100 cap=1000 now=0 attempt status limit delay
  for (( attempt = 0; attempt < attempts; attempt++ )); do
    "$@" && return 0
    status=$?
    if (( status != 75 )); then
      echo "  exit $status: not a temporary failure, stop"
      return "$status"
    fi
    if (( attempt == attempts - 1 )); then break; fi
    limit=$(( base << attempt ))
    (( limit > cap )) && limit=$cap
    next_random
    delay=$(( rnd % (limit + 1) ))
    printf '  t=%4d ms: attempt %d failed (exit %d); wait %d ms\n' "$now" $(( attempt + 1 )) "$status" "$delay"
    now=$(( now + delay ))
  done
  echo "  gave up after $attempts attempts"
  return "$status"
}

echo "flaky"; calls=0; retry flaky 2
echo "usage error"; calls=0; retry usage_error
echo "down"; calls=0; retry flaky 100
echo "exit status of the last retry: $?"
```

```output
flaky
  t=   0 ms: attempt 1 failed (exit 75); wait 56 ms
  t=  56 ms: attempt 2 failed (exit 75); wait 139 ms
  ok after 3 calls
usage error
  exit 64: not a temporary failure, stop
down
  t=   0 ms: attempt 1 failed (exit 75); wait 99 ms
  t=  99 ms: attempt 2 failed (exit 75); wait 81 ms
  t= 180 ms: attempt 3 failed (exit 75); wait 85 ms
  t= 265 ms: attempt 4 failed (exit 75); wait 25 ms
  gave up after 5 attempts
exit status of the last retry: 75
```

- Verdict: idiom. Bash has no retry builtin. A loop around the command, with `$?` for the exit status, is the usual form.
- The exit status says whether a retry can help. The codes 64 and 75 come from `sysexits.h`: `EX_USAGE` is a wrong call, and `EX_TEMPFAIL` is a temporary failure where the user is invited to retry.
- A real script calls `sleep` with the delay. The loop here adds the delay to a counter, so the output does not depend on the clock.
- The script uses its own generator and not `$RANDOM`, because the sequence of `$RANDOM` for a given seed is not part of any standard.
