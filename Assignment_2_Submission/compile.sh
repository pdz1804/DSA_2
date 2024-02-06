g++ -g -o main main.cpp -I . -std=c++11
valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./main > output_submission.txt