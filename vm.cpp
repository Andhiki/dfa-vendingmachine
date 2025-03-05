#include <iostream>
#include <unordered_map>
#include <unordered_set>
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
    int balance;

public:
    DFA(unordered_set<string> states, unordered_set<string> alphabet, string start_state,
        unordered_set<string> accept_states, unordered_map<pair<string, string>, string, hash_pair> transitions)
        : states(states), alphabet(alphabet), start_state(start_state), accept_states(accept_states), transitions(transitions), current_state(start_state), balance(0) {}

    void reset() {
        current_state = start_state;
        balance = 0;
    }

    bool process_input(string user_input) {
        if (alphabet.count(user_input)) {
            if (isdigit(user_input[0])) {
                balance += stoi(user_input);
            }
            auto key = make_pair(current_state, user_input);
            if (transitions.find(key) != transitions.end()) {
                current_state = transitions[key];
            }
            return true;
        }
        return false;
    }

    bool is_accepted() {
        return accept_states.count(current_state);
    }

    int get_balance() {
        return balance;
    }

    int get_change(int price) {
        return balance - price;
    }

    void show_available_drinks(unordered_map<string, int>& prices) {
      cout << "Minuman yang bisa dibeli: ";
      bool found = false;
      for (auto it = prices.begin(); it != prices.end(); ++it) {
          if (balance >= it -> second) {
              if (found) cout << ", ";
              cout << it->first << " (" << it->second << ")";
              found = true;
          }
      }
      if (!found) {
          cout << "Tidak ada (saldo tidak cukup)";
      }
      cout << endl;
  }
};

int main() {
    unordered_set<string> states = {"S0", "S1000", "S2000", "S3000", "S4000", "S5000", "S6000", "S7000", "S8000", "S9000", "S10000"};
    unordered_set<string> alphabet = {"1000", "2000", "5000", "10000", "A", "B", "C"};
    string start_state = "S0";
    unordered_set<string> accept_states = {"S3000", "S4000", "S6000"};
    unordered_map<pair<string, string>, string, hash_pair> transitions = {
        {{"S0", "1000"}, "S1000"}, {{"S0", "2000"}, "S2000"}, {{"S0", "5000"}, "S5000"}, {{"S0", "10000"}, "S10000"},
        {{"S1000", "1000"}, "S2000"}, {{"S1000", "2000"}, "S3000"}, {{"S1000", "5000"}, "S6000"},
        {{"S2000", "1000"}, "S3000"}, {{"S2000", "2000"}, "S4000"}, {{"S2000", "5000"}, "S7000"},
        {{"S3000", "1000"}, "S4000"}, {{"S3000", "2000"}, "S5000"}, {{"S3000", "5000"}, "S8000"},
        {{"S4000", "1000"}, "S5000"}, {{"S4000", "2000"}, "S6000"}, {{"S4000", "5000"}, "S9000"}
    };

    DFA dfa(states, alphabet, start_state, accept_states, transitions);

    unordered_map<string, int> prices = { {"A", 3000}, {"B", 4000}, {"C", 6000} };
    string user_input;
    while (true) {
        cout << "Masukkan uang atau pilih minuman (1000, 2000, 5000, 10000, A, B, C): ";
        cin >> user_input;
        if (user_input == "exit") {
            break;
        }
        if (dfa.process_input(user_input)) {
            dfa.show_available_drinks(prices);
            if (prices.find(user_input) != prices.end()) {
                if (dfa.get_balance() >= prices[user_input]) {
                    int change = dfa.get_change(prices[user_input]);
                    cout << "Minuman " << user_input << " dapat dibeli dengan saldo " << dfa.get_balance() << ". Status: ACCEPTED." << endl;
                    if (change > 0) {
                        cout << "Kembalian: " << change << endl;
                    }
                    dfa.reset();
                } else {
                    cout << "Uang tidak cukup untuk membeli minuman " << user_input << ". Saldo saat ini: " << dfa.get_balance() << " - Harga: " << prices[user_input] << endl;
                }
            } else {
                cout << "Saldo saat ini: " << dfa.get_balance() << endl;
            }
        } else {
            cout << "Input tidak valid." << endl;
        }
    }
    return 0;
}