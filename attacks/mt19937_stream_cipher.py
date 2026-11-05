import secrets
import string
from numpy.random import MT19937


def generate_keystream(seed, length):
    generator = MT19937(seed)
    return bytes(int(generator.random_raw()) & 0xff for _ in range(length))


def stream_cipher(data, seed):
    keystream = generate_keystream(seed, len(data))
    return bytes(a ^ b for a, b in zip(data, keystream))


def encrypt(plaintext, seed):
    return stream_cipher(plaintext, seed)


def decrypt(ciphertext, seed):
    return stream_cipher(ciphertext, seed)


def generate_plaintext():
    characters = string.ascii_letters + string.digits + string.punctuation
    prefix_length = secrets.randbelow(16) + 5
    prefix = ''.join(secrets.choice(characters) for _ in range(prefix_length))
    return (prefix + "A" * 14).encode()


def recover_seed(ciphertext):
    known_pattern = b"A" * 14

    for seed in range(65536):
        plaintext = decrypt(ciphertext, seed)

        if known_pattern in plaintext:
            return seed, plaintext

    return None, None


def main():
    test_message = b"HELLO WORLD"
    test_seed = 12345

    encrypted = encrypt(test_message, test_seed)
    decrypted = decrypt(encrypted, test_seed)

    print("TEST ENCRYPTION")
    print("Original:", test_message)
    print("Ciphertext:", encrypted.hex())
    print("Decrypted:", decrypted)
    print("Verification:", decrypted == test_message)

    print("\nKNOWN PLAINTEXT ATTACK")

    plaintext = generate_plaintext()
    seed = secrets.randbelow(65536)

    ciphertext = encrypt(plaintext, seed)

    print("Original Plaintext:", plaintext)
    print("Original Seed:", seed)
    print("Ciphertext:", ciphertext.hex())

    recovered_seed, recovered_plaintext = recover_seed(ciphertext)

    print("\nRECOVERY RESULTS")
    print("Recovered Seed:", recovered_seed)
    print("Recovered Plaintext:", recovered_plaintext)

    print("Seed Verification:", recovered_seed == seed)
    print("Plaintext Verification:", recovered_plaintext == plaintext)
    print("Known Pattern Verification:", b"A" * 14 in recovered_plaintext)


if __name__ == "__main__":
    main()

