
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

string key = "QWERTYUIOPASDFGHJKLZXCVBNM";

void apply_substitution(string &text, string k, bool decrypt = false) {
    for (char &c : text) {
        if (isalpha(c)) {
            bool upper = isupper(c);
            char x = toupper(c);

            if (!decrypt)
                c = k[x - 'A'];
            else {
                for (int i = 0; i < 26; i++)
                    if (k[i] == x)
                        c = 'A' + i;
            }

            if (!upper)
                c = tolower(c);
        }
    }
}

void frequency_analysis(string text) {
    map<char, int> f;
    int total = 0;

    for (char c : text) {
        if (isalpha(c)) {
            f[toupper(c)]++;
            total++;
        }
    }

    vector<pair<char, int>> v(f.begin(), f.end());

    sort(v.begin(), v.end(), [](auto a, auto b) {
        return a.second > b.second;
    });

    cout << "\nFrequency Analysis\n";

    for (auto x : v) {
        double p = x.second * 100.0 / total;
        cout << x.first << "  " << x.second << "  " << p << "%\n";
    }

    if (!v.empty())
        cout << "Most frequent: " << v[0].first << "\n";
}

void word_frequency_analysis(string text) {
    map<string, int> f;
    string word;

    for (char c : text) {
        if (isalpha(c))
            word += tolower(c);
        else if (!word.empty()) {
            f[word]++;
            word = "";
        }
    }

    if (!word.empty())
        f[word]++;

    cout << "\nWord Frequency Analysis\n";

    for (auto x : f) {
        if (x.first.size() <= 3 || x.second > 1)
            cout << x.first << " : " << x.second << "\n";
    }
}

string pattern(string word) {
    map<char, int> m;
    string p;
    int n = 0;

    for (char c : word) {
        if (!m.count(c))
            m[c] = n++;

        p += to_string(m[c]);
    }

    return p;
}

void pattern_analysis(string text) {
    map<string, vector<string>> p;
    string word;

    for (char c : text) {
        if (isalpha(c))
            word += tolower(c);
        else if (!word.empty()) {
            p[pattern(word)].push_back(word);
            word = "";
        }
    }

    if (!word.empty())
        p[pattern(word)].push_back(word);

    cout << "\nPattern Analysis\n";

    for (auto x : p) {
        if (x.second.size() > 1) {
            cout << x.first << " : ";
            for (string w : x.second)
                cout << w << " ";
            cout << "\n";
        }
    }
}

void display_partial_plaintext(string text, map<char, char> sub) {
    for (char c : text) {
        char x = toupper(c);

        if (isalpha(c) && sub.count(x))
            cout << sub[x];
        else
            cout << c;
    }

    cout << "\n";
}

bool verify_solution(string plain, string cipher, string k) {
    apply_substitution(plain, k);

    for (int i = 0; i < plain.size(); i++) {
        if (isalpha(plain[i]) && plain[i] != cipher[i])
            return false;
    }

    return true;
}

int main() {
    ifstream file("plaintext.txt");

    if (!file) {
        cout << "plaintext.txt not found\n";
        return 0;
    }

    string plaintext, line;

    while (getline(file, line))
        plaintext += line + "\n";

    string ciphertext = plaintext;

    cout << "Plaintext:\n";
    cout << plaintext;

    apply_substitution(ciphertext, key);

    cout << "\nCiphertext:\n";
    cout << ciphertext;

    frequency_analysis(ciphertext);
    word_frequency_analysis(ciphertext);
    pattern_analysis(ciphertext);

    map<char, char> sub;

    cout << "\nPartial Plaintext:\n";

    sub['Q'] = 'E';
    sub['W'] = 'T';
    sub['E'] = 'A';
    sub['R'] = 'O';

    display_partial_plaintext(ciphertext, sub);

    string recovered_key = key;

    string recovered = ciphertext;
    apply_substitution(recovered, recovered_key, true);

    cout << "\nRecovered Plaintext:\n";
    cout << recovered;

    string check = recovered;

    apply_substitution(check, recovered_key);

    cout << "\nVerification: ";

    if (check == ciphertext)
        cout << "Successful\n";
    else
        cout << "Failed\n";

    return 0;
}

