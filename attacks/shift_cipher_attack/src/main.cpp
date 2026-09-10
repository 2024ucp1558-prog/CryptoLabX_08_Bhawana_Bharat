#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <utility>

using namespace std;


string encryptText(const string& text, int key);
string decryptText(const string& text, int key);


unordered_set<string> loadDictionary(const string& filename);

pair<int, string> dictionaryAttack(
    const string& ciphertext,
    const unordered_set<string>& dictionary);


pair<int, string> chiSquareAttack(
    const string& ciphertext);

int main() {

    string plaintext;
    int key;

    cout << "===== Shift Cipher Cryptanalysis =====\n\n";

    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    cout << "Enter key (0-25): ";
    cin >> key;

    string ciphertext = encryptText(plaintext, key);

    cout << "\nEncrypted Ciphertext:\n";
    cout << ciphertext << "\n";

    string dictionaryFile =
        "../dictionary/english_words.txt";

    unordered_set<string> dictionary =
        loadDictionary(dictionaryFile);

    auto dictionaryResult =
        dictionaryAttack(ciphertext, dictionary);

    auto chiSquareResult =
        chiSquareAttack(ciphertext);

    cout << "\n===== Dictionary Attack =====\n";
    cout << "Predicted Key: "
         << dictionaryResult.first << "\n";

    cout << "Predicted Plaintext:\n";
    cout << dictionaryResult.second << "\n";

    cout << "\n===== Chi-Square Attack =====\n";
    cout << "Predicted Key: "
         << chiSquareResult.first << "\n";

    cout << "Predicted Plaintext:\n";
    cout << chiSquareResult.second << "\n";

    cout << "\n===== Actual Values =====\n";
    cout << "Actual Key: " << key << "\n";
    cout << "Original Plaintext: "
         << plaintext << "\n";

    return 0;
}
