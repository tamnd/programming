# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.


class Text(str):
    """A str with one more method. A built-in type such as str cannot get new
    attributes, so a subclass adds the method, and a Text is still a str."""

    __slots__ = ()

    def word_count(self):
        """Returns the number of runs of characters that are not whitespace."""
        return len(self.split())


def word_count(text):
    return Text(text).word_count()
