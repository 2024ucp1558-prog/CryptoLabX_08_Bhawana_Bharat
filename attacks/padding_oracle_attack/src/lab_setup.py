from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes

from oracle import PaddingOracle


BLOCK_SIZE = 16


def pkcs7_pad(data):
    padding_length = BLOCK_SIZE - (len(data) % BLOCK_SIZE)

    return (
        data
        + bytes([padding_length]) * padding_length
    )


def create_lab():
    """
    Create a complete laboratory scenario.

    Returns:
        plaintext
        ciphertext
        iv
        oracle

    The AES key is deliberately NOT returned.
    """

    plaintext = (
        b"Padding oracle attacks demonstrate why "
        b"authenticated encryption matters."
    )

    # The key exists only inside the lab setup/oracle.
    key = get_random_bytes(16)

    iv = get_random_bytes(16)

    cipher = AES.new(
        key,
        AES.MODE_CBC,
        iv=iv
    )

    ciphertext = cipher.encrypt(
        pkcs7_pad(plaintext)
    )

    oracle = PaddingOracle(key)

    # Notice: key is NOT returned.
    return plaintext, ciphertext, iv, oracle

