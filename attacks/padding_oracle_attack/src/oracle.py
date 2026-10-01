from Crypto.Cipher import AES


BLOCK_SIZE = 16


class PaddingOracle:

    def __init__(self, key):
        self.__key = key
        self.query_count = 0

    def query(self, previous_block, target_block):

        if len(previous_block) != BLOCK_SIZE:
            raise ValueError("Previous block must be 16 bytes.")

        if len(target_block) != BLOCK_SIZE:
            raise ValueError("Target block must be 16 bytes.")

        self.query_count += 1

        cipher = AES.new(
            self.__key,
            AES.MODE_ECB
        )

        decrypted = cipher.decrypt(target_block)

        plaintext = bytes(
            a ^ b
            for a, b in zip(decrypted, previous_block)
        )

        padding_length = plaintext[-1]

        if padding_length < 1 or padding_length > BLOCK_SIZE:
            return False

        expected_padding = bytes(
            [padding_length]
        ) * padding_length

        return plaintext[-padding_length:] == expected_padding

