#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <fstream>
using namespace std;

pair<double, vector<vector<char>>> maxValue(vector<vector<char>> state, double alpha, double beta, int depth);
double minValue(vector<vector<char>> state, double alpha, double beta, int depth);
vector<vector<char>> alphaBetaSearch();
bool findLiberty(vector<vector<char>> state, int x, int y);
vector<vector<char>> currentState(5, vector<char>(5));
vector<vector<char>> previousState(5, vector<char>(5));
char myColour, opponentColour;
int maxDepth = 4, moves = 0;

void writeMove() {
    ofstream movesFile;
    movesFile.open("moves.txt");
    movesFile << moves + 2;
}

int main() {
    string line;
    int ct = 0;
    ifstream inputFile("input.txt");
    while (getline(inputFile, line)) {
        if (ct == 0)
            myColour = line[0];
        else if (ct <= 5)
            for (int it = 0; it < 5; it++)
                previousState[ct - 1][it] = line[it];
        else
            for (int it = 0; it < 5; it++)
                currentState[ct - 6][it] = line[it];
        ct++;
    }
    inputFile.close();
    opponentColour = myColour == '1' ? '2' : '1';
    vector<vector<char>> nextState = alphaBetaSearch();
    int dx = -1, dy = -1;
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (currentState[i][j] == '0' && nextState[i][j] != '0') {
                dx = i;
                dy = j;
                break;
            }
    writeMove();
    ofstream outputFile;
    outputFile.open("output.txt");
    if (dx == -1 || dy == -1)
        outputFile << "PASS";
    else
        outputFile << dx << ',' << dy;
    return 0;
}

bool isCurrentEmpty() {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (currentState[i][j] != '0')
                return false;
    return true;
}

bool isPreviousEmpty() {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (previousState[i][j] != '0')
                return false;
    return true;
}

void updateMaxDepth() {
    if (moves == 0 || moves == 1)
        maxDepth = 4 - moves;
    else if (moves == 2 || moves == 3)
        maxDepth = 6 - moves;
    else if (moves == 4)
        maxDepth = 8 - moves;
    else if (moves == 5 || moves == 6 || moves == 7)
        maxDepth = 10 - moves;
    else if (moves == 8)
        maxDepth = 12 - moves;
    else if (moves == 9 || moves == 10)
        maxDepth = 14 - moves;
    else if (moves == 11 || moves == 12 || moves == 13)
        maxDepth = 16 - moves;
    else if (moves == 14)
        maxDepth = 18 - moves;
    else if (moves == 15 || moves == 16)
        maxDepth = 20 - moves;
    else if (moves == 17 or moves == 18)
        maxDepth = 22 - moves;
    else
        maxDepth = 24 - moves;
}

void findMoves() {
    if (isCurrentEmpty() && isPreviousEmpty())
        moves = 0;
    else if (isPreviousEmpty())
        moves = 1;
    else {
        string line;
        ifstream movesFile("moves.txt");
        getline(movesFile, line);
        moves = stoi(line);
    }
    updateMaxDepth();
}

bool isValidChild(vector<vector<char>> state) {
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (state[i][j] != previousState[i][j])
                return true;
    return false;
}

double eulerValue(vector<vector<char>> state, char colour) {
    double q1 = 0, q3 = 0, qd = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char p1 = state[i][j], p2 = state[i][j + 1], p3 = state[i + 1][j], p4 = state[i + 1][j + 1];
            if ((p1 == colour && p2 == colour && p3 == colour && p4 == '0') || (p1 == colour && p2 == colour && p3 == '0' && p4 == colour)
                || (p1 == colour && p2 == '0' && p3 == colour && p4 == colour) || (p1 == '0' && p2 == colour && p3 == colour && p4 == colour))
                q3++;
            else if ((p1 == colour && p2 == '0' && p3 == '0' && p4 == colour) || (p1 == '0' && p2 == colour && p3 == colour && p4 == '0'))
                qd++;
            else if ((p1 == colour && p2 == '0' && p3 == '0' && p4 == '0') || (p1 == '0' && p2 == colour && p3 == '0' && p4 == '0')
                     || (p1 == '0' && p2 == '0' && p3 == colour && p4 == '0') || (p1 == '0' && p2 == '0' && p3 == '0' && p4 == colour))
                q1++;
        }
    }
    return ((double)(q1 - q3 + 2 * qd)) / 4;
}

pair<int, int> libertyDifference(vector<vector<char>> state) {
    int libertyCount = 0, stoneCount = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (state[i][j] == myColour) {
                stoneCount++;
                if (findLiberty(state, i, j))
                    libertyCount++;
            } else if (state[i][j] == opponentColour) {
                stoneCount--;
                if (findLiberty(state, i, j))
                    libertyCount--;
            }
        }
    }
    return make_pair(stoneCount, libertyCount);
}

double utilityValue(vector<vector<char>> state) {
    double edgeCount = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i == 0 || i == 4 || j == 0 || j == 4) {
                if (state[i][j] == myColour)
                    edgeCount++;
                else if (state[i][j] == opponentColour)
                    edgeCount--;
            }
        }
    }
    pair<int, int> libertyDiff = libertyDifference(state);
    double utility = double(min(max(libertyDiff.second, -4), 4)) - 4 * eulerValue(state, myColour) + 7 * libertyDiff.first - edgeCount;
    return myColour == '1' ? utility - 2.5 : utility;
}

vector<pair<int, int>> detectNeighbours(int x, int y) {
    vector<pair<int, int>> neighbors;
    if (x > 0)
        neighbors.emplace_back(x - 1, y);
    if (x < 4)
        neighbors.emplace_back(x + 1, y);
    if (y > 0)
        neighbors.emplace_back(x, y - 1);
    if (y < 4)
        neighbors.emplace_back(x, y + 1);
    return neighbors;
}

vector<pair<int, int>> detectNeighborAllies(vector<vector<char>> state, int x, int y) {
    vector<pair<int, int>> neighbors = detectNeighbours(x, y);
    vector<pair<int, int>> groupAllies;
    for (pair<int, int> piece : neighbors) {
        if (state[piece.first][piece.second] == state[x][y])
            groupAllies.push_back(piece);
    }
    return groupAllies;
}

bool findLiberty(vector<vector<char>> state, int x, int y) {
    stack<pair<int, int>> dfsStack;
    dfsStack.push(make_pair(x, y));
    set<pair<int, int>> allyMembers;
    pair<int, int> piece;
    while (!dfsStack.empty()) {
        piece = dfsStack.top();
        dfsStack.pop();
        if (allyMembers.find(piece) == allyMembers.end()) {
            allyMembers.insert(piece);
            vector<pair<int, int>> neighborAllies = detectNeighborAllies(state, piece.first, piece.second);
            for (pair<int, int> ally : neighborAllies)
                if (allyMembers.find(ally) == allyMembers.end())
                    dfsStack.push(ally);
        }
    }
    for (pair<int, int> allyMember : allyMembers) {
        vector<pair<int, int>> neighbors = detectNeighbours(allyMember.first, allyMember.second);
        for (pair<int, int> neighbor : neighbors) {
            if (state[neighbor.first][neighbor.second] == '0')
                return true;
        }
    }
    return false;
}

vector<pair<int, int>> findDiedPieces(vector<vector<char>> state, char colour) {
    vector<pair<int, int>> diedPieces;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (state[i][j] == colour && !findLiberty(state, i, j))
                diedPieces.emplace_back(i, j);
        }
    }
    return diedPieces;
}

vector<vector<char>> removeDiedPieces(vector<vector<char>> state, char colour) {
    vector<pair<int, int>> diedPieces = findDiedPieces(state, colour);
    for (pair<int, int> diedPiece : diedPieces)
        state[diedPiece.first][diedPiece.second] = '0';
    return state;
}

vector<vector<vector<char>>> getChildren(vector<vector<char>> state, char colour) {
    vector<vector<vector<char>>> children;
    char oppositeColour = colour == '1' ? '2' : '1';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (state[i][j] == '0') {
                state[i][j] = colour;
                vector<vector<char>> updatedState = removeDiedPieces(state, oppositeColour);
                if (isValidChild(updatedState) && findLiberty(updatedState, i, j))
                    children.push_back(updatedState);
                state[i][j] = '0';
            }
        }
    }
    return children;
}

double minValue(vector<vector<char>> state, double alpha, double beta, int depth) {
    if (depth <= maxDepth) {
        vector<vector<vector<char>>> childStates = getChildren(state, opponentColour);
        childStates.push_back(state);
        double v = 200;
        for (vector<vector<char>> childState : childStates) {
            pair<double, vector<vector<char>>> maxState = maxValue(childState, alpha, beta, depth + 1);
            if (maxState.first < v)
                v = maxState.first;
            if (v <= alpha) return v;
            beta = min(beta, v);
        }
        return v;
    } else
        return utilityValue(state);
}

pair<double, vector<vector<char>>> maxValue(vector<vector<char>> state, double alpha, double beta, int depth) {
    if (depth <= maxDepth) {
        vector<vector<vector<char>>> childStates = getChildren(state, myColour);
        childStates.push_back(state);
        double v = -200;
        vector<vector<char>> bestState;
        for (vector<vector<char>> childState : childStates) {
            double minState = minValue(childState, alpha, beta, depth + 1);
            if (minState > v) {
                v = minState;
                bestState = childState;
            }
            if (v >= beta) return make_pair(v, bestState);
            alpha = max(alpha, v);
        }
        return make_pair(v, bestState);
    } else
        return make_pair(utilityValue(state), state);
}

vector<vector<char>> alphaBetaSearch() {
    findMoves();
    pair<double, vector<vector<char>>> v = maxValue(currentState, -200, 200, 1);
    return v.second;
}
