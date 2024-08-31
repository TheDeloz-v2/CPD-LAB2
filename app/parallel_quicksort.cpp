#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath> 
#include <fstream>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;

/**
 * Saves an array of numbers to a file.
 *
 * @param numbers   Pointer to the array of numbers.
 * @param n         The number of elements in the array.
 * @param filename  The name of the file to save the numbers to.
 */
void save_numbers(int* numbers, int n, const string filename) {
    ofstream outfile(filename);

    if (outfile.is_open()) {
        for (int i = 0; i < n; i++) {
            outfile << numbers[i];
            if (i < n - 1) {
                outfile << ',';
            }
        }

        outfile.close();
        cout << "-> File " << filename << " with numbers generated" << endl;
    } else {
        cerr << "-> Error creating file " << filename << endl;
    }
}

/**
 * Reads a sequence of numbers from a file and stores them in an array.
 * 
 * @param numbers       Pointer to the array where the numbers will be stored.
 * @param filename      The name of the file to read the numbers from.
 * @return              The number of elements read from the file, or -1 if an error occurred.
 */
int read_numbers(int* numbers, const string& filename) {
    ifstream infile(filename);

    if (infile.is_open()) {
        string line;
        int count = 0;

        while (getline(infile, line, ',')) {
            numbers[count++] = stoi(line);
        }

        infile.close();
        return count;
    } else {
        cerr << "-> Error reading file " << filename << endl;
        return -1;
    }
}

/**
 * Generates random numbers and assigns them to the given array.
 * 
 * @param numbers   Pointer to the array where the numbers will be stored.
 * @param n         The number of elements in the array.
 */
void generate_numbers(int* numbers, int n) {
    #pragma omp parallel for // Parallelize the generation of numbers
    for (int i = 0; i < n; i++) {
        numbers[i] = rand() % 100000000;
    }
}

/**
 * Sorts an array of integers using the QuickSort algorithm.
 *
 * @param data      The array of integers to be sorted.
 * @param lo        The lower index of the range to be sorted.
 * @param hi        The higher index of the range to be sorted.
 */
void quickSort(int *data, int lo, int hi) {
    if(lo > hi) return;
    int l = lo;
    int h = hi;
    int p = data[(hi + lo)/2];

    while(l <= h){
        while((data[l] - p) < 0) l++;
        while((data[h] - p) > 0) h--;
        if(l<=h){
        //swap
        int tmp = data[l];
        data[l] = data[h];
        data[h] = tmp;
        l++; h--;
        }
    }
    //recursive call
    quickSort(data, lo, h);
    quickSort(data, l, hi);
}

/**
 * @brief Main function for sorting random numbers using quicksort algorithm.
 * 
 * This function generates a set of random numbers, saves them to a file, reads the numbers from the file,
 * sorts them using the quicksort algorithm, and saves the sorted numbers to another file. It also measures
 * the execution time of the sorting process.
 * 
 * @return 0 indicating successful execution.
 */
int main() {
    // Set the number of elements to generate
    int n = 10000000;

    // Seed the random number generator
    srand(time(0));

    auto start = high_resolution_clock::now();

    // Generate random numbers
    int* numbers = new int[n];
    generate_numbers(numbers, n);
    cout << "-> " << n << " random numbers generated" << endl;
    
    // Save the generated numbers to a file
    save_numbers(numbers, n, "numbers.csv");

    // Read the numbers from the file
    int count = read_numbers(numbers, "numbers.csv");
    
    // Sort the numbers using the quicksort algorithm
    if (count > 0) {
        quickSort(numbers, 0, count - 1);
        save_numbers(numbers, count, "sorted_numbers.csv");
    }

    auto end = high_resolution_clock::now();

    // Calculate the execution time
    duration<double> execution_time = (end - start);
    cout << "-> Execution time: " << execution_time.count() << " s" << endl;

    delete[] numbers;
    
    return 0;
}
