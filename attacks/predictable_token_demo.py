import time
from numpy.random import MT19937


def generate_token(timestamp):
    generator = MT19937(int(timestamp))

    token = bytes(
        int(generator.random_raw()) & 0xff
        for _ in range(16)
    )

    return token


def main():
    timestamp = int(time.time())

    token = generate_token(timestamp)

    print("PREDICTABLE TOKEN DEMONSTRATION")

    print("Unix Timestamp:", timestamp)
    print("Generated Token:", token.hex())

    print("\nTOKEN VERIFICATION")

    regenerated_token = generate_token(timestamp)

    print("Regenerated Token:", regenerated_token.hex())
    print("Verification:", regenerated_token == token)

    print("\nSEARCH INTERVAL INFORMATION")

    start_timestamp = timestamp - 300
    end_timestamp = timestamp + 300

    candidates = end_timestamp - start_timestamp + 1

    print("Start Timestamp:", start_timestamp)
    print("End Timestamp:", end_timestamp)
    print("Total Candidate Timestamps:", candidates)

    print("\nSECURITY ANALYSIS")
    print("Timestamp-based seeding is predictable.")
    print("MT19937 is not cryptographically secure.")


if __name__ == "__main__":
    main()
