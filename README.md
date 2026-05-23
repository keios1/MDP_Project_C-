# MDP (Markov Decision Process) Implementation in C++ 🎲

이 프로젝트는 C++를 활용하여 마르코프 결정 과정(Markov Decision Process, MDP)의 핵심 로직을 구현한 프로젝트입니다.

## 📌 프로젝트 개요
* **언어:** C++
* **주요 파일:** `MDP.cpp`
* **목적:** [예: 강화학습의 기초가 되는 MDP 모델을 코드로 직접 구현해보고, 상태(State) 전이와 보상(Reward) 시스템의 동작 방식을 이해하기 위함]

---

## 📖 MDP (마르코프 결정 과정) 란?
MDP는 불확실성을 가진 상황에서 의사결정을 모델링하는 수학적 틀입니다. 어떤 상태(State)에서 행동(Action)을 취했을 때, 다음 상태로 넘어갈 확률(Transition Probability)과 그에 따른 보상(Reward)을 정의합니다.

MDP는 주로 다음의 4가지 요소로 구성됩니다:
* $S$: 상태들의 집합 (Set of States)
* $A$: 행동들의 집합 (Set of Actions)
* $P(s' | s, a)$: 상태 $s$에서 행동 $a$를 했을 때 $s'$로 갈 전이 확률 (Transition Probability)
* $R(s, a, s')$: 그 결과로 얻는 보상 (Reward Function)

목표는 누적 보상을 최대화하는 **최적의 정책(Optimal Policy)**을 찾는 것입니다. 이를 위해 보통 아래와 같은 벨만 방정식(Bellman Equation)을 활용합니다.

$$V(s) = \max_a \left( R(s,a) + \gamma \sum_{s'} P(s'|s,a) V(s') \right)$$

---

## 💻 코드 설명 (`MDP.cpp`)
`MDP.cpp` 파일은 위에서 설명한 수학적 모델을 C++ 객체지향 및 절차적 로직으로 풀어냈습니다.

### 핵심 클래스 및 구조체
* **`State` / `Action`**: [상태와 행동을 정의하는 구조체나 열거형(Enum)에 대한 설명]
* **`Transition`**: [전이 확률 테이블을 2차원 배열이나 맵(Map)으로 구현한 방식 설명]
* **주요 함수**:
  * `[함수명 1]()`: [예: 가치 반복(Value Iteration)을 수행하여 각 상태의 가치를 업데이트합니다.]
  * `[함수명 2]()`: [예: 계산된 가치를 바탕으로 최적의 정책을 도출합니다.]

### 메모리 및 최적화 포인트
* [예: 동적 할당을 최소화하고 스마트 포인터를 사용하여 메모리 누수를 방지했습니다.]
* [예: 2차원 배열 복사를 줄이기 위해 참조(Reference)와 이동 의미론(Move Semantics)을 활용했습니다.]

---

## 🚀 실행 방법
이 코드는 표준 C++14 이상 환경에서 컴파일 및 실행할 수 있습니다.

```bash
# 컴파일
g++ -o mdp_exec MDP.cpp

# 실행
./mdp_exec
