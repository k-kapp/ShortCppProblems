#include <vector>
#include <iostream>
#include <climits>

using namespace std;

struct lowest_info
{
	vector<int> idxes;
	int value;
};

long find_max(const vector<int> &heights, int start_idx, int end_idx)
{
	if (start_idx == end_idx)
		return -LONG_MAX + 1;
	//cout << "entering find_max";
	//cout << " with start_idx, end_idx: " << start_idx << ", " << end_idx << endl;

	lowest_info low = {vector<int>(), INT_MAX};

	for (int i = start_idx; i < end_idx; i++)
	{
		if (heights[i] < low.value)
		{
			low = {{i}, heights[i]};
		}
		else if (heights[i] == low.value)
		{
			low.idxes.push_back(i);
		}
	}

	long max = low.value*(end_idx - start_idx);

	long candidate = find_max(heights, start_idx, low.idxes[0]);
	if (candidate > max)
		max = candidate;
	candidate = find_max(heights, low.idxes.back() + 1, end_idx);
	if (candidate > max)
		max = candidate;

	for (int i = 0; i < low.idxes.size() - 1; i++)
	{
		candidate = find_max(heights, low.idxes[i] + 1, low.idxes[i + 1]);
		if (candidate > max)
			max = candidate;
	}

	//cout << "exiting find_max" << endl;

	return max;
}


int main(int argc, char * argv [])
{
	int N;

	cin >> N;

	vector<int> heights(N);

	for (int i = 0; i < N; i++)
	{
		cin >> heights[i];
	}

	long maximum = find_max(heights, 0, heights.size());

	cout << maximum << endl;

	return 0;
}
