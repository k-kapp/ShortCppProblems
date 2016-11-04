#include <iostream>
#include <vector>
#include <utility>
#include <climits>

using namespace std;

pair<pair<unsigned long long, long>, unsigned long long> max_sum_subarr(unsigned long * data, long main_size, long arr_size, long mod, unsigned long long prev_sum = 0)
{
	unsigned long long sum = (prev_sum - data[arr_size]);
	unsigned long long init_sum = sum;
	long sub_idx = 0, add_idx = arr_size;
	long max_mod_sum = sum % mod;
	unsigned long long max_sum = sum;
	while (add_idx < main_size)
	{
		sum = (sum - data[sub_idx] + data[add_idx]);
		/*
		sum -= data.at(sub_idx);
		if (sum < 0)
		{
			sum = abs(sum) % mod;
			sum = mod - sum;
		}
		sum = (sum + data.at(add_idx)) % mod;
		*/
		
		if (sum % mod > max_mod_sum)
			max_mod_sum = sum % mod;
		if (sum > max_sum)
			max_sum = sum;
		add_idx++;
		sub_idx++;
	}
	return {{max_sum, max_mod_sum}, init_sum};
}

unsigned long long sum_arr(unsigned long * data, long size)
{
	unsigned long long sum = 0;
	for (long i = 0; i < size; i++)
	{
		if ((ULLONG_MAX - sum) < data[i])
			cout << "WARNING: overflow detected" << endl;
		sum += data[i];
	}
	return sum;
}

long max_sum_all(unsigned long * data, long main_size, long mod)
{
	unsigned long long whole_sum = sum_arr(data, main_size);
	//cout << "whole_sum = " << whole_sum << endl;
	unsigned long long max_sum, init_sum;
	long max_mod_sum;
	//auto pair_vals = max_sum_subarr(data, main_size, 1, mod);
	max_sum = whole_sum;
	max_mod_sum = max_sum % mod;
	init_sum = max_sum;

	pair<pair<unsigned long long, long>, unsigned long long> pair_vals;

	if (max_sum < mod)
		return max_sum;

	for (long arr_size = main_size - 1; arr_size > 0; arr_size--)
	{
		pair_vals = max_sum_subarr(data, main_size, arr_size, mod, init_sum);
		if (pair_vals.first.second > max_mod_sum)
			max_mod_sum = pair_vals.first.second;
		init_sum = pair_vals.second;
		max_sum = pair_vals.first.first;
		if (max_mod_sum == mod - 1)
		{
			//cout << "returning due to mod - 1" << endl;
			return max_mod_sum;
		}
		if (max_sum < mod)
		{
			//cout << "returning due to < mod" << endl;
			return max_mod_sum;
		}
		if (arr_size % 100 == 0)
		{
			cout << "size " << arr_size << " with mod: " << mod << endl;
			cout << "max sum: " << max_sum << endl;
		}
	}
	return max_mod_sum;
}

int main(int argc, char * argv [])
{
	long num_queries, arr_len, mod;

	cin >> num_queries;

	unsigned long * data;

	unsigned long temp_in;

	vector<long> answers;

	for (long i = 0; i < num_queries; i++)
	{
		cin >> arr_len;
		cin >> mod;

		data = new unsigned long [arr_len];

		for (long j = 0; j < arr_len; j++)
		{
			cin >> temp_in;
			temp_in = temp_in % mod;
			if (temp_in != 0)
				data[j] = temp_in;
			else
			{
				arr_len--;
				cout << "decrementing arr len" << endl;
			}
		}
		answers.push_back(max_sum_all(data, arr_len, mod));
	}
	for (auto &ans : answers)
		cout << ans << endl;
	return 0;
}

