#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

const int States = 5;
const double GAMMA = 0.9;
const double THETA = 1e-4; // 🟡 가치 수렴 판단 기준점

const char* StateName[States] = { "대기", "상점", "치유소", "전투", "보스" };

// 보상 테이블: R(s, a)
vector<vector<double>> reward = {
    {-100, 1, -1, 2, -1},
    {-1, -100, 2, -1, 5},
    {-1, -1, -100, 3, 6},
    {-1, -1, -1, -100, 10},
    {-1, -1, -1, -1, -100}
};

// 전이 확률: P(s' | s, a)
// 현재 상태 s에서 행동 a(목표 지역)를 취했을 때의 이동 성공 확률
vector<vector<double>> prob = {
    {0, 0.8, 0.7, 0.6, 0},
    {0, 0, 0.9, 0.8, 0.5},
    {0, 0, 0, 0.8, 0.6},
    {0, 0, 0, 0, 0.9},
    {0, 0, 0, 0, 1.0}
};

vector<int> policy(States, 0);
vector<double> V(States, 0.0);

// 단일 행동(정책)에 대한 평가
void policyEvaluation() {
    while (true) {
        double delta = 0.0;
        vector<double> newV = V;

        for (int s = 0; s < States; ++s) {
            int a = policy[s]; // 현재 정책이 지정한 단 하나의 행동
            if (reward[s][a] < 0) continue;

            // 벨만 기대 방정식: 성공 시 a로 이동, 실패 시 s
            double expectedValue = reward[s][a]
                + prob[s][a] * (GAMMA * V[a])
                + (1.0 - prob[s][a]) * (GAMMA * V[s]);

            newV[s] = expectedValue;
            delta = max(delta, abs(newV[s] - V[s]));
        }
        V = newV;
        if (delta < THETA) break; // 가치 수렴할 때까지 반복
    }
}

// 상태/행동 분리 및 기댓값 갱신
bool policyImprovement() {
    bool policyStable = true;
    for (int s = 0; s < States; ++s) {
        int oldAction = policy[s];
        double bestValue = -1e9;
        int bestAction = oldAction;

        for (int a = 0; a < States; ++a) {
            if (reward[s][a] < 0) continue; // 불가능한 행동 스킵

            // 새로운 행동 a를 취했을 때의 가치 평가
            double expectedValue = reward[s][a]
                + prob[s][a] * (GAMMA * V[a])
                + (1.0 - prob[s][a]) * (GAMMA * V[s]);

            if (expectedValue > bestValue) {
                bestValue = expectedValue;
                bestAction = a;
            }
        }
        policy[s] = bestAction;

        if (oldAction != bestAction) { // 정책이 하나라도 변경되었다면 아직 수렴하지 않은 것
            policyStable = false;
        }
    }
    return policyStable;
}

// 무한루프 방지용 최대 스텝 제한 추가
void simulatePathFrom(int start) {
    cout << "\n[경로 시뮬레이션]" << endl;
    int current = start;
    int steps = 0;
    const int MAX_STEPS = 20;

    while (current != 4 && steps < MAX_STEPS) {
        cout << StateName[current] << " → ";
        current = policy[current];
        steps++;
    }

    if (current == 4) cout << StateName[4] << " (도착)" << endl;
    else cout << "... (도달 실패: 무한 루프 감지)" << endl;
}

int main() {
    // 초기 정책 세팅
    for (int s = 0; s < States; ++s){ // 모든 상태를 체크
        for (int a = 0; a < States; ++a){ // 현재 상태에서 고를 수 있는 행동 a를 체크
            if (reward[s][a] >= 0){ // 보상이 0이 넘는지 체크
                policy[s] = a; // 이동가능한 길임으로 길로 저장 후 
                break;  // 하나 찾았으니 탈출
            }
        }
    }

    // 올바른 정책 반복 수렴 로직
    bool isStable = false;
    int iterationCount = 0;
    while (!isStable) { 
        iterationCount++; // 출력용 시행횟수 카운트
        policyEvaluation(); // 가치점수 매기는 함수
        isStable = policyImprovement(); // 점수를 비교하고 더 나은 정책으로 변경
    }

    cout << "[가치 함수 V(s) - " << iterationCount << "회 만에 수렴]" << endl;
    for (int s = 0; s < States; ++s) cout << StateName[s] << " = " << V[s] << endl;

    cout << "\n[최적 정책]" << endl;
    for (int s = 0; s < States; ++s) cout << StateName[s] << " → " << StateName[policy[s]] << endl;

    simulatePathFrom(0);
    return 0;
}