```go run id=function-to-command
package main

import "fmt"

type Candidate struct{ OriginState string }
type MedicalExam struct{ IsSmoker bool }
type ScoringGuide struct{ LowCertification map[string]bool }

func score(c Candidate, m MedicalExam, g ScoringGuide) (int, string) {
	result, healthLevel := 0, 0
	if m.IsSmoker {
		healthLevel += 10
	}
	grade := "regular"
	if g.LowCertification[c.OriginState] {
		grade = "low"
		result -= 5
	}
	result -= max(healthLevel-5, 0)
	return result, grade
}

type scorer struct {
	candidate   Candidate
	medicalExam MedicalExam
	guide       ScoringGuide
	result      int
	healthLevel int
	grade       string
}

func (s *scorer) execute() (int, string) {
	s.result, s.healthLevel = 0, 0
	s.scoreSmoking()
	s.scoreCertification()
	s.result -= max(s.healthLevel-5, 0)
	return s.result, s.grade
}

func (s *scorer) scoreSmoking() {
	if s.medicalExam.IsSmoker {
		s.healthLevel += 10
	}
}

func (s *scorer) scoreCertification() {
	s.grade = "regular"
	if s.guide.LowCertification[s.candidate.OriginState] {
		s.grade = "low"
		s.result -= 5
	}
}

func main() {
	c, m, g := Candidate{"CA"}, MedicalExam{true}, ScoringGuide{map[string]bool{"CA": true}}
	r1, g1 := score(c, m, g)
	s := &scorer{candidate: c, medicalExam: m, guide: g}
	r2, g2 := s.execute()
	if r1 != r2 || g1 != g2 {
		panic("mismatch")
	}
	fmt.Println(r2, g2)
}
```

```output
-10 low
```
