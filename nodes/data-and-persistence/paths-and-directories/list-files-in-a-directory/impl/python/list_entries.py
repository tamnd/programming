# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/list-files-in-a-directory/.

import os


def list_entries(root: str | os.PathLike[str], recursive: bool) -> list[str]:
    """Return the relative paths of the entries under root, sorted by code point.

    A directory has a "/" at the end. A symbolic link is listed, but it is
    never followed.
    """
    out: list[str] = []

    def walk(directory: str | os.PathLike[str], prefix: str) -> None:
        with os.scandir(directory) as entries:
            for entry in entries:
                if entry.is_dir(follow_symlinks=False):
                    out.append(prefix + entry.name + "/")
                    if recursive:
                        walk(entry.path, prefix + entry.name + "/")
                else:
                    out.append(prefix + entry.name)

    walk(root, "")
    out.sort()
    return out
