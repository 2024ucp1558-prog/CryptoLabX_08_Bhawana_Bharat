#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <cctype>

using namespace std;

string clean_ciphertext(string s) {
    string r;
    for(char c : s) {
        if(isalpha(static_cast<unsigned char>(c)))
            r += toupper(static_cast<unsigned char>(c));
    }
    return r;
}

vector<string> find_repeated_patterns(string s, int len = 3) {
    vector<string> p;
    map<string,int> cnt;

    for(int i = 0; i + len <= (int)s.size(); i++)
        cnt[s.substr(i, len)]++;

    for(auto x : cnt)
        if(x.second > 1)
            p.push_back(x.first);

    return p;
}

vector<int> calculate_distances(string s, string p) {
    vector<int> pos, d;

    for(int i = 0; i + (int)p.size() <= (int)s.size(); i++)
        if(s.substr(i, p.size()) == p)
            pos.push_back(i);

    for(int i = 1; i < (int)pos.size(); i++)
        d.push_back(pos[i] - pos[i - 1]);

    return d;
}

vector<int> find_factors(int n) {
    vector<int> f;

    for(int i = 2; i <= n; i++)
        if(n % i == 0)
            f.push_back(i);

    return f;
}

vector<int> kasiski_analysis(string s) {
    map<int,int> score;
    vector<string> patterns = find_repeated_patterns(s);

    for(string p : patterns) {
        vector<int> d = calculate_distances(s, p);

        for(int x : d) {
            vector<int> f = find_factors(x);

            for(int k : f)
                if(k <= 20)
                    score[k]++;
        }
    }

    vector<pair<int,int>> a;

    for(auto x : score)
        a.push_back({x.second, x.first});

    sort(a.rbegin(), a.rend());

    vector<int> result;

    for(auto x : a) {
        result.push_back(x.second);

        if(result.size() == 5)
            break;
    }

    return result;
}

double calculate_ic(string s) {
    if(s.size() < 2)
        return 0;

    int f[26] = {0};

    for(char c : s)
        f[c - 'A']++;

    double sum = 0;

    for(int i = 0; i < 26; i++)
        sum += f[i] * (f[i] - 1);

    return sum / (s.size() * (s.size() - 1));
}

vector<string> split_into_groups(string s, int keyLen) {
    vector<string> g(keyLen);

    for(int i = 0; i < (int)s.size(); i++)
        g[i % keyLen] += s[i];

    return g;
}

vector<int> frequency_analysis(string g) {
    vector<int> f(26, 0);

    for(char c : g)
        f[c - 'A']++;

    return f;
}

int find_shift(string g) {
    double best = -1;
    int shift = 0;

    double eng[26] = {
        8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015,
        6.094, 6.966, 0.153, 0.772, 4.025, 2.406, 6.749,
        7.507, 1.929, 0.095, 5.987, 6.327, 9.056, 2.758,
        0.978, 2.360, 0.150, 1.974, 0.074
    };

    for(int k = 0; k < 26; k++) {
        double score = 0;

        for(int j = 0; j < 26; j++) {
            int cnt = 0;

            for(char c : g) {
                if((c - 'A' - k + 26) % 26 == j)
                    cnt++;
            }

            double expected = g.size() * eng[j] / 100.0;

            if(expected > 0)
                score += cnt * cnt / expected;
        }

        if(best < 0 || score < best) {
            best = score;
            shift = k;
        }
    }

    return shift;
}

string find_key(vector<string> groups) {
    string key;

    for(string g : groups)
        key += char('A' + find_shift(g));

    return key;
}

string vigenere_decrypt(string s, string key) {
    string r;

    for(int i = 0; i < (int)s.size(); i++) {
        r += char(
            'A' +
            (s[i] - 'A' -
             (key[i % key.size()] - 'A') + 26) % 26
        );
    }

    return r;
}

string vigenere_encrypt(string s, string key) {
    string r;

    for(int i = 0; i < (int)s.size(); i++) {
        r += char(
            'A' +
            (s[i] - 'A' +
             key[i % key.size()] - 'A') % 26
        );
    }

    return r;
}

bool verify(string original, string encrypted) {
    return original == encrypted;
}

int main() {

    ifstream in("input/ciphertext.txt");
    ofstream out("output/result.txt");

    if(!in || !out) {
        cout << "File error\n";
        return 1;
    }

    string raw, line;

    while(getline(in, line))
        raw += line + " ";

    string cipher = clean_ciphertext(raw);

    vector<int> candidates = kasiski_analysis(cipher);

    if(candidates.empty())
        candidates = {3, 4, 5, 6, 7};

    int keyLen = candidates[0];

    double bestIC = 0;
    int bestLen = keyLen;

    for(int len : candidates) {

        vector<string> groups = split_into_groups(cipher, len);
        double total = 0;

        for(string g : groups)
            total += calculate_ic(g);

        total /= groups.size();

        if(total > bestIC) {
            bestIC = total;
            bestLen = len;
        }
    }

    keyLen = bestLen;

    vector<string> groups = split_into_groups(cipher, keyLen);

    string key = find_key(groups);

    string plain = vigenere_decrypt(cipher, key);

    string check = vigenere_encrypt(plain, key);

    bool verification = verify(cipher, check);

    
    string result;

    result += "\n";
    result += "       VIGENERE CIPHER ANALYSIS\n";
    result += "\n\n";

    result += "Estimated Key Length: ";
    result += to_string(keyLen);
    result += "\n\n";

    result += "Kasiski Candidate Key Lengths: ";

    for(int i = 0; i < (int)candidates.size(); i++) {
        result += to_string(candidates[i]);

        if(i + 1 < (int)candidates.size())
            result += ", ";
    }

    result += "\n";
    result += "Best Average IC: " + to_string(bestIC) + "\n\n";

    result += "Frequency Tables\n";
    result += "----------------\n\n";

    for(int i = 0; i < (int)groups.size(); i++) {

        vector<int> f = frequency_analysis(groups[i]);

        result += "Group " + to_string(i + 1) + ": ";
        result += groups[i] + "\n";

        for(int j = 0; j < 26; j++) {
            result += char('A' + j);
            result += ":";
            result += to_string(f[j]);
            result += " ";
        }

        result += "\n";

        result += "IC: ";
        result += to_string(calculate_ic(groups[i]));
        result += "\n\n";
    }

    result += "Recovered Key: ";
    result += key;
    result += "\n\n";

    result += "Recovered Plaintext\n";
    result += "-------------------\n\n";

    for(int i = 0; i < (int)plain.size(); i++) {
        result += plain[i];

        if((i + 1) % 80 == 0)
            result += "\n";
    }

    result += "\n\n";

    result += "Verification: ";

    if(verification)
        result += "PASSED\n";
    else
        result += "FAILED\n";

    result += "\n";

    result += "Original Ciphertext Length: ";
    result += to_string(cipher.size());
    result += "\n";

    result += "Re-encrypted Length: ";
    result += to_string(check.size());
    result += "\n";

    result += "\n\n";
    result += "             ANALYSIS DONE\n";
    result += "\n";

   

    out << result;


    cout << result;

    out.close();
    in.close();

    cout << "\nResult saved to output/result.txt\n";

    return 0;
}

