#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <sstream>
using namespace std;

struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        return hash<T1>{}(p.first) ^ hash<T2>{}(p.second);
    }
};

class DFA {
private:
    unordered_set<string> states;
    unordered_set<string> alphabet;
    string start_state;
    unordered_set<string> accept_states;
    unordered_map<pair<string, string>, string, hash_pair> transitions;
    string current_state;
    vector<string> path;
    unordered_map<string, int> prices = {{"A", 3000}, {"B", 4000}, {"C", 6000}};
    int balance = 0;

public:
    DFA(
        unordered_set<string> states,
        unordered_set<string> alphabet,
        string start_state,
        unordered_set<string> accept_states,
        unordered_map<pair<string, string>, string, hash_pair> transitions
    ) : states(states), alphabet(alphabet), start_state(start_state),
        accept_states(accept_states), transitions(transitions), current_state(start_state) {
        path.push_back(start_state);
    }

    void reset() {
        current_state = start_state;
        balance = 0;
        path.clear();
        path.push_back(start_state);
    }

    bool is_valid_input(const string& input) {
        return alphabet.count(input) > 0;
    }

    bool process_input(string input) {
        if (!is_valid_input(input)) {
            return false;
        }

        auto key = make_pair(current_state, input);
        if (transitions.find(key) != transitions.end()) {
            current_state = transitions[key];
            path.push_back(current_state);
            
            if (isdigit(input[0])) {
                balance += stoi(input);
                // Ensure balance doesn't exceed 10000
                if (balance > 10000) {
                    current_state = "Reject";
                    path.push_back(current_state);
                }
            }
            return true;
        }
        return false;
    }

    bool is_accepted() {
        return accept_states.count(current_state);
    }

    bool is_reject() {
        return current_state == "Reject";
    }

    void show_available_drinks() {
        bool available = false;
        cout << "ON: ";
        for (auto& item : prices) {
            if (balance >= item.second) {
                if (available) cout << ", ";
                cout << "Minuman " << item.first;
                available = true;
            }
        }
        if (!available) cout << "Tidak ada minuman yang tersedia";
        cout << endl;
    }

    void print_path() {
        cout << "Lintasan DFA: ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i != 0) cout << " → ";
            cout << path[i];
        }
        cout << endl;
    }

    void process_drink(string drink) {
        if (prices.count(drink)) {
            if (balance >= prices[drink]) {
                print_path();
                cout << "Minuman " << drink << " dapat dibeli. Status: ACCEPTED." << endl;
                if (balance > prices[drink]) {
                    cout << "Kembalian: " << (balance - prices[drink]) << endl;
                }
                reset();
            } else {
                print_path();
                cout << "Uang tidak cukup. Status: REJECTED." << endl;
                reset();
            }
        }
    }

    int get_balance() {
        return balance;
    }
};

DFA load_dfa_from_file(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open DFA file" << endl;
        exit(1);
    }

    unordered_set<string> states;
    unordered_set<string> alphabet;
    string start_state;
    unordered_set<string> accept_states;
    unordered_map<pair<string, string>, string, hash_pair> transitions;

    string line;
    string section;
    while (getline(file, line)) {
        // Remove comments
        line = line.substr(0, line.find('#'));
        // Trim whitespace
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if (line.empty()) continue;

        if (line.find("States:") == 0) {
            stringstream ss(line.substr(7));
            string state;
            while (getline(ss, state, ',')) {
                if (!state.empty()) states.insert(state);
            }
        }
        else if (line.find("Alphabet:") == 0) {
            stringstream ss(line.substr(9));
            string symbol;
            while (getline(ss, symbol, ',')) {
                if (!symbol.empty()) alphabet.insert(symbol);
            }
        }
        else if (line.find("Start:") == 0) {
            start_state = line.substr(6);
        }
        else if (line.find("Accept:") == 0) {
            stringstream ss(line.substr(7));
            string state;
            while (getline(ss, state, ',')) {
                if (!state.empty()) accept_states.insert(state);
            }
        }
        else if (line.find("Transitions:") == 0) {
            continue;
        }
        else {
            // Parse transition line (format: from,input,to)
            stringstream ss(line);
            string from, input, to;
            getline(ss, from, ',');
            getline(ss, input, ',');
            getline(ss, to, ',');
            
            if (!from.empty() && !input.empty() && !to.empty()) {
                transitions[{from, input}] = to;
            }
        }
    }

    return DFA(states, alphabet, start_state, accept_states, transitions);
}

int main() {
    DFA vending_machine = load_dfa_from_file("vending_dfa.txt");

    string input;
    while (true) {
        cout << "Masukkan uang atau beli minuman (1000, 2000, 5000, 10000, A, B, C): ";
        cin >> input;

        if (input == "exit") break;

        if (!vending_machine.is_valid_input(input)) {
            cout << "Input tidak valid! Harap masukkan 1000, 2000, 5000, 10000, A, B, atau C" << endl;
            continue;
        }

        if (vending_machine.process_input(input)) {
            if (vending_machine.is_accepted()) {
                string drink = input;
                vending_machine.process_drink(drink);
            }
            else if (vending_machine.is_reject()) {
                vending_machine.print_path();
                cout << "Transaksi ditolak! Mesin akan direset." << endl;
                vending_machine.reset();  // Manual reset after rejection
            }
            else {
                if (isdigit(input[0])) {
                    cout << "Saldo saat ini: " << vending_machine.get_balance() << endl;
                    vending_machine.show_available_drinks();
                }
            }
        }
        else {
            cout << "Transisi tidak valid untuk state saat ini" << endl;
        }
    }

    return 0;
}