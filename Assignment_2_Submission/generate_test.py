import random
import string

def generate_test_cases(prefix, num_cases):
    test_cases = []
    for _ in range(num_cases):
        #random_string = ''.join(random.choices(string.ascii_letters, k=length))
        random_string = ''.join(random.choices(string.ascii_letters, k=random.randint(20,50)))
        test_case = prefix + random_string
        test_cases.append(test_case)
    return test_cases

# Generate 50 test cases with the prefix "LAPSE " and a random string of length 20
test_cases = generate_test_cases("LAPSE ", 200)

for i, test_case in enumerate(test_cases):
    print(test_case)

# Write the test cases to a text file
with open('test_cases.txt', 'w') as f:
    for i, test_case in enumerate(test_cases):
        f.write(f"{test_case}\n")