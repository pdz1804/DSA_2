#include "main.h"
#include "restaurant.cpp"
// #include "restaurant_output_check.cpp"

class Timer1
{
public:
    Timer1() : start_(std::chrono::high_resolution_clock::now()) {}
    ~Timer1()
    {
        const auto finish = std::chrono::high_resolution_clock::now();
        const auto duration = finish - start_;
        const double ms = (double)duration.count();
        cout << "\n| Elapsed time: " << ms << " s |\n";
        // solution << "\n| Elapsed time: " << ms << " s |\n";
    }

private:
    const std::chrono::high_resolution_clock::time_point start_;
};

int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();

    string fileName = "input/new_input_1.txt";
    // string fileName = "test.txt";
    simulate(fileName);

    end = clock();
    double cputime = double(end - start) / CLOCKS_PER_SEC;
    // cout << "CPU time: " << cputime << endl;

    return 0;
}