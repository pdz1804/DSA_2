import random
import string
import sympy
import os

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
        case = random.choices(["LAPSE", "HAND", "KOKUSEN", "KEITEIKEN", "CLEAVE", "LIMITLESS"], weights=[10, 3, 1, 1, 1, 1], k=1)[0]
        if case == "LAPSE":
            # Generate a random string of characters
            random_string = ''.join(random.choices(string.ascii_letters, k=random.randint(30,80)))
            test_case = f"{case} {random_string}"
        elif case == "KEITEIKEN":
            # Generate a random number in the range from 1 to max_size
            number = random.randint(1, max_size // 5)
            test_case = f"{case} {number}"
        elif case in ["CLEAVE", "LIMITLESS"]:
            # Generate a random number in the range from 1 to max_size
            number = random.randint(1, max_size)
            test_case = f"{case} {number}"
        else:
            test_case = f"{case}"
        test_cases.append(test_case)
    return test_cases

# Function to generate test cases and write them to separate files
def generate_and_write_test_cases(num_files, directory):
    # Create the directory if it doesn't exist
    if not os.path.exists(directory):
        os.makedirs(directory)

    for i in range(num_files):
        # Generate the test cases
        test_cases = generate_test_cases(random.choice([1234]))

        # Write the test cases to a file in the specified directory
        with open(os.path.join(directory, f'new_input_last_{i+1}.txt'), 'w') as f:
            for index, test_case in enumerate(test_cases):
                # If it's the last test case, don't include a newline character
                if index == len(test_cases) - 1:
                    f.write(f"{test_case}")
                else:
                    f.write(f"{test_case}\n")


# Generate 10 test cases in 10 files in the 'test_cases' directory
generate_and_write_test_cases(1, 'E:\\2_LEARNING_BKU\\2_File_2\\K22_HK3\\CO2003_DSA_Nguyen_Duc_Dung\\BTL\\BTL2\\Last_Check\\BTL_16_12\\input')