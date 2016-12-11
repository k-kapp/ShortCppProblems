#include <iostream>
#include <vector>

using namespace std;

inline int abs(int num)
{
	return num < 0 ? -num : num;
}

int get_other_idx(int num, int idx)
{
	idx += 1;
	int req_diff = abs(idx - num);

	if (idx > num)
	{
		return num - req_diff - 1;
	}
	else
	{
		return num + req_diff - 1;
	}
}

int get_other_number(int num, int idx)
{
	idx += 1;
	int req_diff = abs(idx - num);

	if (num > idx)
	{
		return idx - req_diff;
	}
	else
	{
		return idx + req_diff;
	}
}

vector<int> build_vector(int req_diff, int max_num)
{
	vector<int> values(max_num, 0);

	vector<int> insertees(max_num);

	for (int i = 0; i < max_num; i++)
	{
		insertees[i] = i + 1;
	}

	for (int i = 0; i < req_diff; i++)
	{
		values[i] = req_diff + 1 + i;
		insertees[req_diff + i] = 0;

		int other_num = values[i];
		int other_idx = i;
		int count = 0;
		while (count < max_num - 1)
		{
			other_idx = get_other_idx(other_num, other_idx);
			if (other_idx > max_num - 1)
				break;
			other_num = get_other_number(other_num, other_idx);
			if (other_num > max_num)
				return {-1};
			values[other_idx] = other_num;
			insertees[other_num - 1] = 0;
			count++;
		}
	}

	
	for (int i = req_diff; i < max_num; i++)
	{
		if (values[i] == 0)
		{
			values[i] = i + 1 - req_diff;
			if (insertees[i - req_diff] == 0)
				return {-1};
			else
				insertees[i - req_diff] = 0;
		}
	}

	return values;
}

int main(int argc, char * argv [])
{
	int T;
	cin >> T;

	vector< vector<int> > all_values;

	for (int i = 0; i < T; i++)
	{
		int N;
		cin >> N;

		int K;
		cin >> K;

		if (K <= N/2)
		{
			all_values.push_back(build_vector(K, N));
		}
		else
			all_values.push_back({-1});
	}

	for (auto &val_vec : all_values)
	{
		for (auto &val : val_vec)
		{
			cout << val << " ";
		}
		cout << endl;
	}

	return 0;
}
