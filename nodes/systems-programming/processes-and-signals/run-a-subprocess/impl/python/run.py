# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/run-a-subprocess/.

import subprocess


def run(argv: list[str]) -> tuple[int, str, str]:
    """Run argv with no shell and return (status, stdout, stderr).

    A missing program raises FileNotFoundError. A status that is not 0
    is a result, so check is False.
    """
    result = subprocess.run(
        argv,
        stdin=subprocess.DEVNULL,
        capture_output=True,
        encoding="utf-8",
        check=False,
    )
    if result.returncode < 0:
        raise ChildProcessError(f"{argv[0]} was stopped by signal {-result.returncode}")
    return result.returncode, result.stdout, result.stderr
