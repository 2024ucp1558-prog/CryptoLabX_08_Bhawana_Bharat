BLOCK_SIZE = 16


def split_blocks(data):
    """
    Split ciphertext into 16-byte AES blocks.
    """

    if len(data) % BLOCK_SIZE != 0:
        raise ValueError(
            "Ciphertext length must be a multiple of 16 bytes."
        )

    return [
        data[i:i + BLOCK_SIZE]
        for i in range(0, len(data), BLOCK_SIZE)
    ]


def recover_block(oracle, previous_block, target_block):
    """
    Recover one plaintext block using a padding oracle.

    previous_block:
        C(i-1), or the IV when attacking the first block.

    target_block:
        C(i).

    The AES key is NEVER accessed here.
    """

    # I = Dk(Ci)
    intermediate = bytearray(BLOCK_SIZE)

    # P = I XOR C(i-1)
    recovered = bytearray(BLOCK_SIZE)

    for position in range(BLOCK_SIZE - 1, -1, -1):

        padding_value = BLOCK_SIZE - position


        modified_previous = bytearray(previous_block)

        
        for j in range(position + 1, BLOCK_SIZE):

            modified_previous[j] = (
                intermediate[j] ^ padding_value
            )

        found = False

        for guess in range(256):

            modified_previous[position] = guess

            valid = oracle.query(
                bytes(modified_previous),
                target_block
            )

            if valid:

                intermediate[position] = (
                    guess ^ padding_value
                )

                # CBC decryption:
                #
                # Pi = Dk(Ci) XOR C(i-1)

                recovered[position] = (
                    intermediate[position]
                    ^ previous_block[position]
                )

                found = True
                break

        if not found:
            raise RuntimeError(
                f"Could not recover byte {position} "
                f"of target block."
            )

    return bytes(recovered)


def recover_plaintext(ciphertext, iv, oracle):
    """
    Recover all padded plaintext blocks.

    The AES key is NOT passed here.
    """

    ciphertext_blocks = split_blocks(ciphertext)

    plaintext = bytearray()

    previous_block = iv

    for block_number, target_block in enumerate(
        ciphertext_blocks,
        start=1
    ):

        print(
            f"[*] Recovering plaintext block "
            f"{block_number}/{len(ciphertext_blocks)}..."
        )

        recovered_block = recover_block(
            oracle,
            previous_block,
            target_block
        )

        plaintext.extend(recovered_block)

        print(
            f"    Recovered: {recovered_block!r}"
        )

  
        previous_block = target_block

    return bytes(plaintext)


def remove_pkcs7_padding(data):
    """
    Validate and remove PKCS#7 padding.
    """

    if not data:
        raise ValueError(
            "Cannot remove padding from empty data."
        )

    padding_length = data[-1]

    if padding_length < 1 or padding_length > BLOCK_SIZE:
        raise ValueError(
            "Invalid PKCS#7 padding."
        )

    expected_padding = bytes(
        [padding_length]
    ) * padding_length

    if data[-padding_length:] != expected_padding:
        raise ValueError(
            "Invalid PKCS#7 padding."
        )

    return data[:-padding_length]

