from lab_setup import create_lab

from padding_oracle_attack import (
    recover_plaintext,
    remove_pkcs7_padding
)


def main():

    print("=" * 65)
    print("AES-CBC PADDING ORACLE ATTACK")
    print("=" * 65)

    
    original_plaintext, ciphertext, iv, oracle = create_lab()

    print("\n[+] IV:")
    print(iv.hex())

    print("\n[+] Ciphertext:")
    print(ciphertext.hex())

    print("\n[+] Ciphertext length:")
    print(len(ciphertext), "bytes")

    print("\n[+] Number of ciphertext blocks:")
    print(len(ciphertext) // 16)

    print("\n[+] Starting attack...")
    print()

   
    recovered_padded = recover_plaintext(
        ciphertext,
        iv,
        oracle
    )

    recovered_plaintext = remove_pkcs7_padding(
        recovered_padded
    )

  

    print("\n" + "=" * 65)
    print("RESULTS")
    print("=" * 65)

    print("\nOriginal plaintext:")
    print(original_plaintext.decode())

    print("\nRecovered plaintext:")
    print(recovered_plaintext.decode())

    print("\nTotal oracle queries:")
    print(oracle.query_count)

    print("\nRecovery successful:")
    print(recovered_plaintext == original_plaintext)

    print("\n" + "=" * 65)


if __name__ == "__main__":
    main()

