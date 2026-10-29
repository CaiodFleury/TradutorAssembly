#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>
#include <utility>

using namespace std;

const vector<pair<string, int>> opcodes = {
    {"add", 2}, {"sub", 2}, {"mult", 2}, 
    {"div", 2}, {"jmp", 2}, {"jmpn", 2}, 
    {"jmpp", 2}, {"jmpz", 2}, {"copy", 3}, 
    {"load", 2}, {"store", 2}, {"input", 2},
    {"output", 2}, {"stop", 1}, {"const", 1}, 
    {"space", 1}
};

bool getInstructionInfo(const string& op, int& size, int& opcodeNum) {
    for (size_t i = 0; i < opcodes.size(); i++) {
        if (opcodes[i].first == op) {
            size = opcodes[i].second;
            opcodeNum = i + 1;
            return true;
        }
    }
    return false;
}

bool validateAndSplit(string input, vector<string>& parts) {
    istringstream stream(input);
    string part;
    while (stream >> part) {
        parts.push_back(part);
    }
    return !parts.empty();
}

bool IsLabel(string word) {
    return !word.empty() && word.back() == ':';
}

int main(int argc, char* argv[]) {

    if (argc < 2) return 1;

    ifstream file(argv[1]);
    if (!file.is_open()) return 1;

    vector<string> linhas;
    string line;
    while (getline(file, line)) {
        transform(line.begin(), line.end(), line.begin(), ::tolower);
        linhas.push_back(line);
    }
    file.close();

    map<string, int> simbols;
    vector<vector<string>> matrizLinhas; 
    int currentPos = 0;

    for (size_t i = 0; i < linhas.size(); i++) {
        vector<string> parts;
        if (validateAndSplit(linhas[i], parts)) {
            int idx = 0;
            if (parts.empty() || parts[idx] == "section" || parts[idx] == "secao") continue;

            if (IsLabel(parts[idx])) {
                string labelName = parts[idx];
                labelName.pop_back(); 
                simbols[labelName] = currentPos;
                idx++;
                if (idx >= parts.size()) continue;
            }

            string opcodesim = parts[idx];
            if (opcodesim[0] == ';') continue;

            int size = 0, opcodeNum = 0;
            if (getInstructionInfo(opcodesim, size, opcodeNum)) {
                currentPos += size;
                matrizLinhas.push_back(parts);
            } else {
                return 1;
            }
        }
    }

    for (size_t i = 0; i < matrizLinhas.size(); i++) {
        int idxOpcode = 0;
        if (IsLabel(matrizLinhas[i][0])) {
            idxOpcode = 1;
        }

        string op = matrizLinhas[i][idxOpcode];

        if (op == "space") {
            cout << 0 << " ";
            continue;
        } 
        else if (op == "const") {
            if (idxOpcode + 1 < matrizLinhas[i].size()) {
                cout << matrizLinhas[i][idxOpcode + 1] << " ";
            }
            continue;
        }

        int size = 0, opcodeNum = 0;
        if (getInstructionInfo(op, size, opcodeNum)) {
            cout << opcodeNum;
        } else {
            cout << 0;
        }

        for (size_t j = idxOpcode + 1; j < matrizLinhas[i].size(); j++) {
            if (matrizLinhas[i][j][0] == ';') break;

            string token = matrizLinhas[i][j];
            if (simbols.find(token) != simbols.end()) {
                cout << " " << simbols[token];
            } else {
                cout << " " << token;
            }
        }
        cout << " ";
    }
    cout << "\n";

    return 0;
}