#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;
int comparisions = 0;
vector<int> comps;
vector<int> array_gen(int n)
{
    vector<int> arr(n);
    iota(arr.begin(), arr.end(), 1);

    random_device rd;
    default_random_engine rng(rd());

    shuffle(arr.begin(), arr.end(), rng);

    return arr;
}

int randomized_partition(vector<int> &arr, int low, int high)
{
    random_device rd;
    default_random_engine rng(rd());
    uniform_int_distribution<int> dist(low, high);

    int random_pivot = dist(rng);
    swap(arr[random_pivot], arr[high]);

    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        if (arr[j] < pivot)
        {
            i++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
        comparisions++;
    }
    int temp = arr[high];
    arr[high] = arr[i + 1];
    arr[i + 1] = temp;
    return i + 1;
}

void randomized_quicksort(vector<int> &arr, int low, int high)
{
    if (low < high)
    {
        int pivot_index = randomized_partition(arr, low, high);
        randomized_quicksort(arr, low, pivot_index - 1);
        randomized_quicksort(arr, pivot_index + 1, high);
    }
}

int main()
{
    vector<int> sizes = {10000, 100000, 1000000, 10000000};
    vector<vector<int>> arrays;
    ofstream file("quick-sort-output.txt");

    for (int size : sizes)
    {
        for (int i = 0; i < 100; ++i)
        {
            cout << "generating array of size " << size << ": " << i << "\n";
            arrays.push_back(array_gen(size));
        }
    }
    for (int i = 0; i < arrays.size(); ++i)
    {
        // file << "initial array: ";
        // for (int j = 0; j < arrays[i].size(); ++j)
        // {
        //     file << arrays[i][j] << " ";
        // }

        // file << "\n";
        cout << "sorting array " << i + 1 << "\n";
        randomized_quicksort(arrays[i], 0, arrays[i].size() - 1);
        // file << "Sorted array: ";
        // for (int j = 0; j < arrays[i].size(); ++j)
        // {
        //     file << arrays[i][j] << " ";
        // }
        // file << "\n";
        comps.push_back(comparisions);
        comparisions = 0;
    }
    for (int i = 0; i < comps.size(); ++i)
    {
        cout << "comparisions: " << comps[i] << "\n";
        file << "comparisions: " << comps[i] << "\n";
    }

    return 0;
}
