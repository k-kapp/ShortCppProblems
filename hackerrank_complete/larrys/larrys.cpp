#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

void remove_from_list(list<int> &linked, int search_val)
{
	for (auto iter = linked.begin(); iter != linked.end(); advance(iter, 1))
	{
		if (*iter == search_val)
		{
			if (distance(linked.begin(), iter) % 2 != 0)
			{
				if (next(iter, 1) == linked.end())
				{
					swap(*next(iter, -1), *next(iter, -2));
				}
				else
				{
					swap(*next(iter, 1), *next(iter, -1));
				}
			}
			linked.erase(iter);
			return;
		}
	}
}

bool can_sort(vector<int> &nums)
{
	list<int> linked = list<int>(nums.begin(), nums.end());

	sort(nums.begin(), nums.end());

	for (int i = 0; i < nums.size() - 2; i++)
	{
		int curr_num = nums[i];
		remove_from_list(linked, curr_num);
	}

	return linked.front() <= linked.back();
}

int main(int argc, char * argv [])
{
	int T;

	cin >> T;

	vector<bool> answers;
	answers.reserve(T);

	for (int i = 0; i < T; i++)
	{
		int N;
		cin >> N;

		vector<int> data;
		data.reserve(N);

		for (int j = 0; j < N; j++)
		{
			int val;
			cin >> val;
			data.push_back(val);
		}
		
		answers.push_back(can_sort(data));
	}

	for (auto ans : answers)
	{
		if (ans)
		{
			cout << "YES" << endl;
		}
		else
		{
			cout << "NO" << endl;
		}
	}

	return 0;
}
