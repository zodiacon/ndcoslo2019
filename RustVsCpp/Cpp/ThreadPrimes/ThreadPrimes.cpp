// ThreadPrimes.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

using namespace std;

bool isPrime(int n) {
	int limit = (int)::sqrt(n);
	for (int i = 2; i <= limit; i++)
		if (n % i == 0)
			return false;
	return true;
}

size_t calcPrimes(int n, int first, int last) {
	vector<thread> threads;
	threads.reserve(n);

	int chunk = (last - first + 1) / n;

	vector<int> result;

	for (int i = 0; i < n; i++) {
		int start = first + i * chunk;
		int end = i == n - 1 ? last : start + chunk - 1;

		thread t([&result](int from, int to) {
			for (int i = from; i <= to; i++)
				if (isPrime(i))
					result.push_back(i);
			}, start, end);

		threads.push_back(move(t));
	}

	for (auto& t : threads)
		t.join();

	return result.size();
}

size_t calcPrimesFixed(int n, int first, int last) {
	vector<thread> threads;
	threads.reserve(n);

	int chunk = (last - first + 1) / n;

	vector<int> result;
	mutex lock;

	for (int i = 0; i < n; i++) {
		int start = first + i * chunk;
		int end = i == n - 1 ? last : start + chunk - 1;

		thread t([&result, &lock](int from, int to) {
			for (int i = from; i <= to; i++)
				if (isPrime(i)) {
					lock_guard locker(lock);
					result.push_back(i);
				}
			}, start, end);

		threads.push_back(move(t));
	}

	for (auto& t : threads)
		t.join();

	return result.size();
}

int main(int argc, const char* argv[]) {
	int n = argc > 1 ? atoi(argv[1]) : 8;
	cout << "Calculating primes with " << n << " threads" << endl;

	int first = 3, last = 20000000;
	auto timeStart = chrono::steady_clock::now();
	auto count = calcPrimesFixed(n, first, last);

	auto timeEnd = chrono::steady_clock::now();
	auto elapsed = chrono::duration_cast<chrono::milliseconds>(timeEnd - timeStart);

	cout << "Running time: " << elapsed.count() << " msec" << endl;
	cout << "Total primes: " << count << endl;
}
