# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-symbolic-link/.

import os


def make_symlink(target: str, link: str | os.PathLike[str]) -> str:
    """Make a symbolic link at link that holds target, and return the target
    that the link holds, read back with os.readlink.

    target is stored as it is: it need not exist, and a relative target is
    read from the directory of link. The order of the arguments is the order
    of os.symlink and of ln -s: first the target, then the new name.
    os.symlink raises FileExistsError when link exists, also as a dangling
    link, and FileNotFoundError when the directory of link does not exist.
    """
    os.symlink(target, link)
    return os.readlink(link)
