import random
import string
import sympy

def generate_test_cases(num_cases):
    test_cases = []
    # Generate a random prime number less than 100 for the first line
    max_size = random.choice([i for i in range(10, 25) if sympy.isprime(i)])
    first_line = f"MAXSIZE {max_size}"
    test_cases.append(first_line)
    for _ in range(num_cases-1):
        # Generate a random prime number less than or equal to max_size
        # start = random.choice([i for i in range(2, max_size+1) if sympy.isprime(i)])
        # Randomly choose one of the cases, with "LAPSE" having a probability of more than 50%
        case = random.choices(["LAPSE", "HAND", "KOKUSEN", "KEITEIKEN", "CLEAVE", "LIMITLESS"], weights=[10, 2, 0.7, 2, 1, 1], k=1)[0]
        if case == "LAPSE":
            # Generate a random string of characters
            random_string = ''.join(random.choices(string.ascii_letters, k=random.randint(30,80)))
            test_case = f"{case} {random_string}"
        elif case in ["KEITEIKEN", "CLEAVE", "LIMITLESS"]:
            # Generate a random number in the range from 1 to max_size
            number = random.randint(1, max_size // 2)
            test_case = f"{case} {number}"
        else:
            test_case = f"{case}"
        test_cases.append(test_case)
    return test_cases

# Generate 50 test cases with max_size 100
test_cases = generate_test_cases(400)

# Write the test cases to a text file
with open('test_cases.txt', 'w') as f:
    for i, test_case in enumerate(test_cases):
        f.write(f"{test_case}\n")
