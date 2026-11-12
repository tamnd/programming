```python run id=function-to-command
def score(candidate: dict, medical_exam: dict, scoring_guide) -> tuple[int, str]:
    result = 0
    health_level = 0
    if medical_exam["is_smoker"]:
        health_level += 10
    certification_grade = "regular"
    if scoring_guide(candidate["origin_state"]):
        certification_grade = "low"
        result -= 5
    result -= max(health_level - 5, 0)
    return result, certification_grade


class Scorer:
    def __init__(self, candidate: dict, medical_exam: dict, scoring_guide) -> None:
        self._candidate = candidate
        self._medical_exam = medical_exam
        self._scoring_guide = scoring_guide

    def execute(self) -> tuple[int, str]:
        self._result = 0
        self._health_level = 0
        self._score_smoking()
        self._score_certification()
        self._result -= max(self._health_level - 5, 0)
        return self._result, self._certification_grade

    def _score_smoking(self) -> None:
        if self._medical_exam["is_smoker"]:
            self._health_level += 10

    def _score_certification(self) -> None:
        self._certification_grade = "regular"
        if self._scoring_guide(self._candidate["origin_state"]):
            self._certification_grade = "low"
            self._result -= 5


# The lighter way: nested functions that share the locals with nonlocal.
def score_nested(candidate: dict, medical_exam: dict, scoring_guide) -> tuple[int, str]:
    result = 0
    health_level = 0

    def score_smoking() -> None:
        nonlocal health_level
        if medical_exam["is_smoker"]:
            health_level += 10

    score_smoking()
    grade = "low" if scoring_guide(candidate["origin_state"]) else "regular"
    result -= 5 if grade == "low" else 0
    return result - max(health_level - 5, 0), grade


args = ({"origin_state": "CA"}, {"is_smoker": True}, lambda s: s == "CA")
assert Scorer(*args).execute() == score(*args) == score_nested(*args)
print(Scorer(*args).execute())
```

```output
(-10, 'low')
```
