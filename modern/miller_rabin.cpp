#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

#include <boost/multiprecision/cpp_int.hpp>
#include <openssl/rand.h>
#include <openssl/bn.h>

using namespace std;
using namespace boost::multiprecision;

using BigInt = cpp_int;

const int BIT_LENGTH = 512;
const int MR_ROUNDS = 40;

const int SMALL_PRIMES[] = {
    3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
};

void secureRandomBytes(unsigned char* buffer, int length) {
    if (RAND_bytes(buffer, length) != 1) {
        throw runtime_error("Random generation failed");
    }
}

BigInt bytesToInt(const unsigned char* bytes, int length) {
    BigInt value = 0;

    for (int i = 0; i < length; i++) {
        value <<= 8;
        value |= bytes[i];
    }

    return value;
}

BigInt randomBelow(const BigInt& limit) {
    if (limit <= 0)
        throw runtime_error("Invalid random range");

    int bits = 0;
    BigInt temp = limit - 1;

    while (temp > 0) {
        temp >>= 1;
        bits++;
    }

    if (bits == 0)
        return 0;

    int byteCount = (bits + 7) / 8;
    int excessBits = byteCount * 8 - bits;

    vector<unsigned char> bytes(byteCount);

    while (true) {
        secureRandomBytes(bytes.data(), byteCount);

        if (excessBits > 0) {
            bytes[0] &= static_cast<unsigned char>(
                0xFF >> excessBits
            );
        }

        BigInt value = bytesToInt(bytes.data(), byteCount);

        if (value < limit)
            return value;
    }
}

BigInt generateCandidate() {
    unsigned char bytes[64];

    secureRandomBytes(bytes, 64);

    bytes[0] |= 0x80;
    bytes[63] |= 0x01;

    return bytesToInt(bytes, 64);
}

int bitLength(BigInt n) {
    int bits = 0;

    while (n > 0) {
        n >>= 1;
        bits++;
    }

    return bits;
}

BigInt modPow(BigInt base, BigInt exp, const BigInt& mod) {
    BigInt result = 1;
    base %= mod;

    while (exp > 0) {
        if ((exp & 1) != 0)
            result = (result * base) % mod;

        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}

bool millerRabin(
    const BigInt& n,
    int rounds,
    long long& roundsPerformed,
    vector<BigInt>* usedBases = nullptr
) {
    if (n < 2)
        return false;

    if (n == 2 || n == 3)
        return true;

    if ((n & 1) == 0)
        return false;

    for (int p : SMALL_PRIMES) {
        if (n == p)
            return true;

        if (n % p == 0)
            return false;
    }

    BigInt d = n - 1;
    int s = 0;

    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    for (int i = 0; i < rounds; i++) {
        BigInt a = randomBelow(n - 3) + 2;

        roundsPerformed++;

        if (usedBases != nullptr)
            usedBases->push_back(a);

        BigInt x = modPow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool passed = false;

        for (int r = 1; r < s; r++) {
            x = (x * x) % n;

            if (x == n - 1) {
                passed = true;
                break;
            }

            if (x == 1)
                break;
        }

        if (!passed)
            return false;
    }

    return true;
}

bool opensslPrimeCheck(const BigInt& n) {
    string decimal = n.str();

    BIGNUM* number = nullptr;
    BN_CTX* context = BN_CTX_new();

    if (context == nullptr) {
        throw runtime_error("OpenSSL context allocation failed");
    }

    if (BN_dec2bn(&number, decimal.c_str()) == 0) {
        BN_CTX_free(context);
        throw runtime_error("Integer conversion failed");
    }

    int result = BN_is_prime_ex(number, 64, context, nullptr);

    BN_free(number);
    BN_CTX_free(context);

    if (result < 0) {
        throw runtime_error("OpenSSL primality check failed");
    }

    return result == 1;
}

int main() {
    try {
        BigInt prime = 0;

        long long candidatesTested = 0;
        long long searchRounds = 0;

        cout << "Generating a 512-bit probable prime...\n";

        auto start = chrono::steady_clock::now();

        while (true) {
            BigInt candidate = generateCandidate();
            candidatesTested++;

            bool divisible = false;

            for (int p : SMALL_PRIMES) {
                if (candidate % p == 0) {
                    divisible = true;
                    break;
                }
            }

            if (divisible)
                continue;

            if (millerRabin(
                    candidate,
                    MR_ROUNDS,
                    searchRounds
                )) {
                prime = candidate;
                break;
            }
        }

        auto end = chrono::steady_clock::now();

        double elapsed = chrono::duration<double>(
            end - start
        ).count();

        int actualBits = bitLength(prime);

        long long verificationRounds = 0;
        vector<BigInt> bases;

        bool mrPassed = millerRabin(
            prime,
            MR_ROUNDS,
            verificationRounds,
            &bases
        );

        bool opensslPassed = opensslPrimeCheck(prime);

        bool correctBits = (actualBits == BIT_LENGTH);
        bool odd = ((prime & 1) != 0);

        ofstream primeFile("generated_prime.txt");
        primeFile << prime << '\n';
        primeFile.close();

        ofstream baseFile("verification_bases.txt");

        for (const BigInt& base : bases)
            baseFile << base << '\n';

        baseFile.close();

        ofstream csv("experimental_results.csv");

        csv << "Parameter,Value\n";
        csv << "Assigned bit length,512\n";
        csv << "Generated prime candidate,\""
            << prime << "\"\n";
        csv << "Actual bit length," << actualBits << '\n';
        csv << "Candidates tested," << candidatesTested << '\n';
        csv << "Miller-Rabin rounds per full test,40\n";
        csv << "Search Miller-Rabin rounds,"
            << searchRounds << '\n';
        csv << "Final verification rounds,"
            << verificationRounds << '\n';
        csv << "Execution time (seconds),"
            << fixed << setprecision(6) << elapsed << '\n';
        csv << "Miller-Rabin verification,"
            << (mrPassed ? "PASS" : "FAIL") << '\n';
        csv << "OpenSSL primality verification,"
            << (opensslPassed ? "PASS" : "FAIL") << '\n';

        csv.close();

        cout << "\n========== EXPERIMENTAL RESULTS ==========\n";
        cout << "Assigned bit length       : " << BIT_LENGTH << '\n';
        cout << "Generated prime candidate:\n" << prime << '\n';
        cout << "Actual bit length         : " << actualBits << '\n';
        cout << "Candidates tested         : " << candidatesTested << '\n';
        cout << "Miller-Rabin rounds       : " << MR_ROUNDS << '\n';
        cout << "Search MR rounds          : " << searchRounds << '\n';
        cout << "Verification rounds       : " << verificationRounds << '\n';
        cout << "Execution time (seconds)  : "
             << fixed << setprecision(6) << elapsed << '\n';

        cout << "\n========== VERIFICATION ==========\n";
        cout << "Correct bit length        : "
             << (correctBits ? "PASS" : "FAIL") << '\n';
        cout << "Candidate is odd          : "
             << (odd ? "PASS" : "FAIL") << '\n';
        cout << "Miller-Rabin verification : "
             << (mrPassed ? "PASS" : "FAIL") << '\n';
        cout << "OpenSSL verification      : "
             << (opensslPassed ? "PASS" : "FAIL") << '\n';

        cout << "\nFiles created:\n";
        cout << "generated_prime.txt\n";
        cout << "verification_bases.txt\n";
        cout << "experimental_results.csv\n";

        if (!correctBits || !odd || !mrPassed ||
            verificationRounds != MR_ROUNDS ||
            !opensslPassed) {
            cerr << "Verification failed.\n";
            return 1;
        }

        cout << "\nPrime generation completed successfully.\n";

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
