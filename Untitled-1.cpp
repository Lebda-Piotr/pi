#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <vector>
#include <chrono>

const int num_threads = 4;  // Domyślna liczba wątków

double f(double x) {
    return 4.0 / (1.0 + x * x);
}

void calculate_partial_integral(double start, double end, double &partial_result, int num_intervals) {
    partial_result = 0.0;
    double step = (end - start) / num_intervals;  // Dokładność całkowania równa num_intervals

    for (double x = start; x < end; x += step) {
        partial_result += f(x) * step;
    }
}

int main() {
    int num_intervals, num_threads_user;

    std::cout << "Podaj ilość liczb z przedziału całki: ";
    std::cin >> num_intervals;

    std::cout << "Podaj ilość wątków: ";
    std::cin >> num_threads_user;

    if (num_threads_user <= 0) {
        std::cerr << "Błędna liczba wątków. Ustawiam domyślną wartość: " << num_threads << std::endl;
        num_threads_user = num_threads;
    }

    const double interval_width = 1.0 / num_intervals;

    std::vector<double> partial_results(num_threads_user, 0.0);
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < num_threads_user; ++i) {
        double start = i * interval_width * num_intervals / num_threads_user;
        double end = (i + 1) * interval_width * num_intervals / num_threads_user;

        threads.emplace_back(calculate_partial_integral, start, end, std::ref(partial_results[i]), num_intervals);
    }

    for (auto &thread : threads) {
        thread.join();
    }

    double total_result = 0.0;
    for (double result : partial_results) {
        total_result += result;
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << std::fixed << std::setprecision(10);  // Ustawienie precyzji na 10 miejsc po przecinku
    std::cout << "Wynik: " << total_result << std::endl;
    std::cout << "Czas obliczeń: " << duration.count() << " ms" << std::endl;

    return 0;
}

