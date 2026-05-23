#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

const int States = 5;
const double GAMMA = 0.9;
const int MAX_ITER = 100;

// 상태 이름
const char* StateName[States] = { "대기", "상점", "치유소", "전투", "보스" };

// 보상 테이블
vector<vector<double>> reward = { // 불가능한 이동을 -１로 계산 보상을 크게 낮춰 a->a로 못가게함.
    {-100, 1, -1, 2, -1}, //무한 루프 방지
    {-1, -100, 2, -1, 5},
    {-1, -1, -100, 3, 6},
    {-1, -1, -1, -100, 10},
    {-1, -1, -1, -1, -100}
};

// 전이 확률
vector<vector<double>> prob = {
    {0, 0.4, 0.3, 0.3, 0},
    {0, 0, 0.2, 0.3, 0.5},
    {0, 0, 0, 0.5, 0.5},
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 1}
};

// 정책: 각 상태에서 어떤 행동(다음 상태)을 할 것인가
vector<int> policy(States, 0);

// 가치 함수
vector<double> V(States, 0.0);

// 정책 평가
void policyEvaluation() {
    for (int a = 0; a < MAX_ITER; ++a) { // 최대값 지정
        vector<double> newV(States, 0.0);// 값 저장을 위해 벡터 공간 준비
        for (int s = 0; s < States; ++s) { // 모든 상태를 확인
            int a = policy[s]; // 현재 정책이 선택한 행동
            for (int move = 0; move < States; ++move) { // 현재에서 갈 수 있는 다음 상태 전부를 확인
                if (reward[s][move] >= 0) { // 유효한 이동이면 계산
                    newV[s] += prob[s][move] * (reward[s][move] + GAMMA * V[move]); // 벨만 기대 방정식에 따라 가치 함수를 계산
                }
            }
        }
        V = newV; // 기대값 업데이트
    }
}

// 정책 개선
void policyImprovement() {
    for (int s = 0; s < States; ++s) {//모든 상태를 반복
        double bestValue = -1e9; // 가장 작은값부터 시작
        int bestAction = s; // 나은 행동을 저장
        for (int a = 0; a < States; ++a) { // s에서 동작할 수 있는 모든 상태 점검
            if (reward[s][a] < 0) continue; // 보상이 없으면 건너뛴다.
            double value = 0.0; // 기대 보상을 계산할 변수
            for (int move1 = 0; move1 < States; ++move1) {
                value += prob[a][move1] * (reward[a][move1] + GAMMA * V[move1]); //  기대값 계산
            }
            if (value > bestValue) { // 더 나은값으로 변경
                bestValue = value;
                bestAction = a;
            }
        }
        policy[s] = bestAction; // 더 나은 액션을 저장
    }
}

// 경로 시뮬레이션 함수
void simulatePathFrom(int start) {
    cout << endl;
    cout << "[경로 시뮬레이션]" << endl;
    int current = start;
    int steps = 0;
    while (current != 4) { // 보스 도달
        cout << StateName[current] << " → ";
        current = policy[current];
        steps++;
    }
    cout << StateName[4] << " (도착)";
    cout << endl;
}

int main() {
    // 가능한 첫 행동을 골라서 초기 정책을 만들어줌
    for (int s = 0; s < States; ++s) {
        for (int a = 0; a < States; ++a) {
            if (reward[s][a] >= 0) { // 이동이 가능할 시 
                policy[s] = a; //  유효한 행동 a정책을 저장
                break;
            }
        }
    }

    // 정책 반복 수행
    for (int i = 0; i < 10; ++i) {
        policyEvaluation();
        policyImprovement();
    }

    // 결과 출력
    cout << "[가치 함수 V(s)]" << endl;
    for (int s = 0; s < States; ++s)
        cout << StateName[s] << " = " << V[s] << endl;

    cout << endl;

    cout << "[최적 정책]" << endl;
    for (int s = 0; s < States; ++s)
        cout << StateName[s] << " → " << StateName[policy[s]] << endl;

    simulatePathFrom(0);

    return 0;
}
