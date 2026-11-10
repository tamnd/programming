# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/read-an-environment-variable/.

import os


def get_env(name: str, default: str) -> str:
    """Return the value of the environment variable name, or default when it is not set.

    An empty value is a value, so it does not give the default.
    """
    return os.environ.get(name, default)
