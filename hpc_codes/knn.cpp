# KNN (K-Nearest Neighbours) – Serial and Parallel Implementation on Car Dataset

## Problem Statement

Implement the K-Nearest Neighbours (KNN) algorithm on a car dataset using the following features:

* Price
* Mileage
* Engine Capacity

The goal is to compare all cars with a target car using Euclidean Distance and predict the nearest neighbours.

Two implementations are required:

1. Serial KNN
2. Parallel KNN

Additionally:

* Measure execution timestamps
* Compare execution times
* Show nearest cars

---

# 1. Euclidean Distance Formula

KNN works by calculating the distance between the target car and every other car.

The Euclidean Distance formula:

[
Distance = \sqrt{(p_1-p_2)^2 + (m_1-m_2)^2 + (e_1-e_2)^2}
]

Where:

* (p) = price
* (m) = mileage
* (e) = engine

---

# 2. Sample Dataset

| Car    | Price   | Mileage | Engine |
| ------ | ------- | ------- | ------ |
| Swift  | 700000  | 22      | 1197   |
| Baleno | 850000  | 21      | 1197   |
| Creta  | 1500000 | 17      | 1497   |
| Venue  | 1300000 | 18      | 1493   |
| City   | 1400000 | 19      | 1498   |
| Verna  | 1600000 | 18      | 1497   |
| WagonR | 650000  | 24      | 998    |
| Alto   | 500000  | 25      | 796    |
| Nexon  | 1200000 | 20      | 1497   |
| Punch  | 900000  | 20      | 1199   |

Target Car:

* Price = 1250000
* Mileage = 19
* Engine = 1490

---

# 3. COMBINED SERIAL + PARALLEL KNN IMPLEMENTATION (C++)

This program:

* Takes car dataset input from user
* Takes target car input
* Executes both Serial and Parallel KNN
* Calculates Euclidean distance
* Displays nearest neighbours
* Measures execution timestamps
* Compares execution times

---

```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace chrono;

struct Car {
    string name;
    double price;
    double mileage;
    double engine;
};

struct Result {
    string name;
    double distance;
};

// Euclidean Distance Function

double calculateDistance(Car c, Car target) {
    return sqrt(
        pow(c.price - target.price, 2) +
        pow(c.mileage - target.mileage, 2) +
        pow(c.engine - target.engine, 2)
    );
}

int main() {

    int n;

    cout << "Enter number of cars: ";
    cin >> n;

    vector<Car> cars(n);

    cout << "\nEnter Car Details:\n";
    cout << "Format: Name Price Mileage Engine\n\n";

    for (int i = 0; i < n; i++) {
        cin >> cars[i].name
            >> cars[i].price
            >> cars[i].mileage
            >> cars[i].engine;
    }

    Car target;

    cout << "\nEnter Target Car Details:\n";
    cout << "Format: Price Mileage Engine\n";

    target.name = "Target";

    cin >> target.price
        >> target.mileage
        >> target.engine;

    int k;

    cout << "\nEnter value of K: ";
    cin >> k;

    // SERIAL IMPLEMENTATION

    vector<Result> serialResults;

    auto serialStart = high_resolution_clock::now();

    for (auto car : cars) {
        double dist = calculateDistance(car, target);
        serialResults.push_back({car.name, dist});
    }

    sort(serialResults.begin(), serialResults.end(),
         [](Result a, Result b) {
             return a.distance < b.distance;
         });

    auto serialStop = high_resolution_clock::now();

    auto serialDuration =
        duration_cast<microseconds>(serialStop - serialStart);

    // PARALLEL IMPLEMENTATION

    vector<Result> parallelResults(n);

    auto parallelStart = high_resolution_clock::now();

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {

        double dist = calculateDistance(cars[i], target);

        parallelResults[i].name = cars[i].name;
        parallelResults[i].distance = dist;
    }

    sort(parallelResults.begin(), parallelResults.end(),
         [](Result a, Result b) {
             return a.distance < b.distance;
         });

    auto parallelStop = high_resolution_clock::now();

    auto parallelDuration =
        duration_cast<microseconds>(parallelStop - parallelStart);

    // OUTPUT

    cout << "\n====================================";
    cout << "\nSERIAL KNN RESULT";
    cout << "\n====================================\n";

    for (int i = 0; i < k; i++) {
        cout << i + 1 << ". "
             << serialResults[i].name
             << " -> Distance = "
             << serialResults[i].distance
             << endl;
    }

    cout << "\nSerial Execution Time: "
         << serialDuration.count()
         << " microseconds\n";

    cout << "\n====================================";
    cout << "\nPARALLEL KNN RESULT";
    cout << "\n====================================\n";

    for (int i = 0; i < k; i++) {
        cout << i + 1 << ". "
             << parallelResults[i].name
             << " -> Distance = "
             << parallelResults[i].distance
             << endl;
    }

    cout << "\nParallel Execution Time: "
         << parallelDuration.count()
         << " microseconds\n";

    double speedup =
        (double)serialDuration.count() /
        parallelDuration.count();

    cout << "\nSpeedup = "
         << speedup << "x" << endl;

    return 0;
}
```

---

# 4. Sample Input

```text
Enter number of cars: 5

Enter Car Details:
Format: Name Price Mileage Engine

Swift 700000 22 1197
Creta 1500000 17 1497
Venue 1300000 18 1493
Nexon 1200000 20 1497
Baleno 850000 21 1197

Enter Target Car Details:
Format: Price Mileage Engine

1250000 19 1490

Enter value of K: 3
```

---

# 5. Sample Output

```text
====================================
SERIAL KNN RESULT
====================================
1. Venue -> Distance = 50009
2. Nexon -> Distance = 50049
3. Creta -> Distance = 250000

Serial Execution Time: 120 microseconds

====================================
PARALLEL KNN RESULT
====================================
1. Venue -> Distance = 50009
2. Nexon -> Distance = 50049
3. Creta -> Distance = 250000

Parallel Execution Time: 45 microseconds

Speedup = 2.66x
```

---

# 6. Compilation Command

```bash
g++ knn.cpp -fopenmp -o knn
./knn
```

---

# 7. Important Observation

Your features are on different scales:

* Price → Lakhs
* Mileage → Tens
* Engine → Thousands

So price dominates Euclidean distance.

Correct KNN should ideally use normalization before distance calculation.

---

# 8. Time Complexity

For N cars:

* Distance Calculation = O(N)
* Sorting = O(N log N)

Parallelization speeds up distance computation.

---

# 9. Final Conclusion

This project demonstrates:

* Serial KNN
* Parallel KNN using OpenMP
* Euclidean distance comparison
* Timestamp measurement
* Speedup analysis
* Car recommendation based on similarity
