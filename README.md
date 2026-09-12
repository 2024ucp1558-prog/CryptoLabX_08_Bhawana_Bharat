# CryptoLabX_08_Bhawana_Bharat

# Cryptography Laboratory (22CPP307)

## About the Project

CryptoLabX is a semester-long cryptography and cryptanalysis toolkit developed as part of the Cryptography Laboratory course. The project is designed to provide a common platform where different cryptographic algorithms, cryptanalysis techniques, security analysis experiments, and secure application concepts can be implemented and studied.

Instead of creating a separate repository for every laboratory experiment, all experiments are continuously developed and maintained in a single GitHub repository. The repository grows throughout the semester as new cryptographic algorithms, attacks, analysis techniques, and security modules are added.

The main focus of CryptoLabX is not only to implement cryptographic algorithms but also to understand how these algorithms work, how they can be attacked, and how secure software can be developed and analyzed.

---

# Team Information

| Field | Details |
|------|---------|
| Course | Cryptography Laboratory (22CPP307) |
| Group Number | 08 |
| Student 1 | Bhawana |
| Roll Number | 2024ucp1558 |
| Student 2 | Bharat |
| Roll Number | 2025ucp1725 |


---

# Project Objectives

The major objectives of CryptoLabX are:

- To understand fundamental concepts of cryptography.
- To implement classical cryptographic algorithms.
- To study cryptanalysis techniques.
- To perform frequency and statistical analysis.
- To understand how cryptographic systems can be attacked.
- To develop a small security-focused application.
- To identify vulnerabilities using Static Application Security Testing (SAST).
- To use Git and GitHub for version control.
- To maintain meaningful commit history.
- To develop modular and reusable cryptographic software.
- To document experiments, observations, and results systematically.

---

# Repository Structure

```text
CryptoLabX/
│
├── classical/
│   ├── monoalphabetic_substitution/
│   └── ...
│
├── modern/
│
├── hashing/
│
├── attacks/
│   └── shift_cipher_attack/
│
├── analysis/
│
├── datasets/
│
├── outputs/
│
├── docs/
│
├── secure_application/
│   ├── src/
│   ├── reports/
│   ├── screenshots/
│   ├── sast/
│   ├── crypto/
│   ├── outputs/
│   ├── testcases/
│   └── README.md
│
├── main.py
├── requirements.txt
├── .gitignore
└── README.md

Assignment 1 – Build Your CryptoLabX Toolkit

The first assignment establishes the foundation of the CryptoLabX toolkit. The main purpose is to create a modular project structure and introduce Git-based version control.

The project includes a command-line interface with options for encryption, decryption, attack, analysis, and exit. Basic file analysis is also performed by reading text files from the dataset folder.

The file analysis calculates the number of characters, words, lines, unique characters, and letter frequencies. A log file is maintained to record the date, time, and selected menu option.

At least five text files are maintained in the datasets folder for use in future assignments.

Assignment 2 – Static Application Security Testing

Static Application Security Testing (SAST) analyzes source code without executing the program. SAST tools use security rules to identify potentially vulnerable coding practices.

For this assignment, the assigned SAST tool was installed and configured. A small program containing intentionally insecure coding practices was created and analyzed.

The reported vulnerabilities were studied using their file names, line numbers, severity, confidence, rule IDs, explanations, and suggested remediation.

Assignment 3 – Secure E-Commerce Application

For Group 08, the assigned application is an E-Commerce Website.

The application demonstrates the core functionality of an online shopping system. The application also contains selected vulnerabilities for educational security analysis and SAST testing.

Core Functionalities
Product browsing
Shopping cart
Checkout
Order history
Vulnerabilities

Three vulnerabilities are demonstrated:

1. SQL Injection

SQL Injection occurs when user input is directly used in an SQL query without proper validation or parameterization.

2. Cross-Site Scripting (XSS)

XSS occurs when untrusted input is displayed without proper sanitization or output encoding.

3. Insecure Direct Object Reference (IDOR)

IDOR occurs when an application allows users to access objects using user-controlled identifiers without properly checking authorization.

The vulnerabilities are intentionally included for laboratory purposes so that their detection and prevention can be studied.

Assignment 4 – Cryptanalysis of Shift Cipher

The Shift Cipher is a classical substitution cipher in which every letter is shifted by a fixed number of positions in the alphabet.

Since the Shift Cipher has only a small number of possible keys, it can be attacked using brute force. Language-based and statistical techniques can further improve the identification of the correct key.

For this assignment, Shift Cipher cryptanalysis is performed using Brute Force, Dictionary Scoring, and Chi-Square Analysis.
Assignment 5 – Monoalphabetic Substitution Cipher

A Monoalphabetic Substitution Cipher replaces each plaintext letter with another letter according to a fixed substitution key.

Although the actual letters are changed, the frequency characteristics of the language are approximately preserved. Therefore, frequency analysis and word patterns can be used to recover the plaintext.

For this assignment, the cipher is implemented and cryptanalysis is performed using letter frequency, word frequency, repeated words, and repeated letter patterns.
Assignment 6 – Cryptanalysis of Vigenère Cipher

The Vigenère Cipher is a polyalphabetic substitution cipher that uses a repeating key. Different shifts are applied to different letters according to the key.

The repeating structure of the key can be analyzed using Kasiski Examination and frequency-based techniques.

Kasiski Examination identifies repeated patterns in the ciphertext and uses the distances between them to estimate possible key lengths. After estimating the key length, the ciphertext is divided into groups and frequency analysis is performed on each group.
