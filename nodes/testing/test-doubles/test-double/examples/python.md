```python test id=register
import unittest
from unittest import mock


class Mailer:
    def send(self, to, body):
        raise NotImplementedError("talks to an SMTP server")


def register(email, users, mailer):
    if users.exists(email):
        return "taken"
    users.add(email)
    mailer.send(email, "Welcome!")
    return "created"


class FakeUsers:
    def __init__(self):
        self.emails = set()

    def exists(self, email):
        return email in self.emails

    def add(self, email):
        self.emails.add(email)


class RegisterTest(unittest.TestCase):
    def setUp(self):
        self.users = FakeUsers()
        self.mailer = mock.create_autospec(Mailer, instance=True)

    def test_unused_email_is_stored_and_welcomed(self):
        self.assertEqual(register("ada@example.com", self.users, self.mailer), "created")
        self.assertTrue(self.users.exists("ada@example.com"))
        self.mailer.send.assert_called_once_with("ada@example.com", "Welcome!")

    def test_taken_email_gets_no_mail(self):
        self.users.add("ada@example.com")
        self.assertEqual(register("ada@example.com", self.users, self.mailer), "taken")
        self.mailer.send.assert_not_called()


if __name__ == "__main__":
    unittest.main(verbosity=2)
```

```output stream=stderr
test_taken_email_gets_no_mail (test_main.RegisterTest.test_taken_email_gets_no_mail) ... ok
test_unused_email_is_stored_and_welcomed (test_main.RegisterTest.test_unused_email_is_stored_and_welcomed) ... ok

----------------------------------------------------------------------
Ran 2 tests in $TIME

OK
```

- Verdict: stdlib. `unittest.mock` is part of the standard library. It gives `Mock`, `MagicMock`, `create_autospec` and `patch`.
- The store is a fake written by hand, because a set states its behaviour in a few lines. The mailer is a mock with a spec, because the test cares only about the call.
- The assertion methods such as `assert_called_once_with` and `assert_not_called` belong to the mock, not to `TestCase`. A misspelt name such as `assret_called_with` would be a fresh attribute of the mock, so `Mock` raises `AttributeError` for names that start with `assert`, `assret` and a few other misspellings, unless the mock is made with `unsafe=True`.
- `patch` replaces a name in a module for the length of a test or a `with` block. It suits code that does not take its dependencies as parameters, but it ties the test to the name that the code looks up.
