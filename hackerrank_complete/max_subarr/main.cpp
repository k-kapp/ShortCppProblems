#include <vector>
#include <iostream>
#include <list>
#include <climits>

using namespace std;

using info_list = list<pair<long, pair<int, int> > >;

void print_info_list(info_list lst)
{
	for (auto &el : lst)
	{
		cout << el.first << " (" << el.second.first << ", " << el.second.second << "), ";
	}
	cout << endl;
}

bool check_dupl(info_list arr)
{
	int count = 0;
	for (auto iter = arr.begin(); iter != arr.end(); advance(iter, 1))
	{
		if (next(iter, 1) != arr.end())
		{
			if ((long long)iter->first * (long long)next(iter, 1)->first >= 0)
			{
				return false;
			}
		}
	}
	return true;
}

info_list reduce(const list<long> &arr)
{
	info_list reduced;
	pair<int, int> curr_range;

	long curr_sum = 0;

	int idx = 0;

	for (auto &val : arr)
	{
		if ((long long)val * (long long)curr_sum >= 0)
		{
			if (!((val < 0) && (curr_sum == 0)))
				curr_sum += val;
		}
		else
		{
			curr_range.second = idx - 1;
			reduced.push_back({curr_sum, curr_range});
			curr_sum = val;
			curr_range.first = idx;
		}
		idx++;
	}
	reduced.push_back({curr_sum, {curr_range.first, idx - 1}});


	return reduced;
}

void reduce2(info_list &arr)
{
	bool no_erase;


	for (auto iter = arr.begin(); iter != arr.end();)
	{
		no_erase = true;
		int between_sum = 0;
		info_list::iterator orig_pos;
		orig_pos = iter;
		if ((next(iter, 1) != arr.end()) && (next(iter, 2) != arr.end()))
		{
			if ((abs(next(iter, 2)->first) >= abs(next(iter, 1)->first)) 
				&& (abs(iter->first) >= abs(next(iter, 1)->first)))
			{
				iter->first = next(iter, 2)->first + next(iter, 1)->first + iter->first;
				iter->second.second = next(iter, 2)->second.second;
				arr.erase(next(iter, 1), next(iter, 3));
				no_erase = false;
			}
		}
		if (no_erase)
		{
			advance(iter, 1);
		}
	}
}


int sum_range(int begin, int end, const list<int> arr)
{
	int sum = 0;
	for (auto iter = next(arr.begin(), begin); iter != next(arr.begin(), end); advance(iter, 1))
	{
		sum += *iter;
	}
	return sum;
}

pair<info_list, info_list> reduce_all(const list<long> arr)
{
	info_list reduced = reduce(arr);
	auto reduced_init = reduced;
	unsigned int prev_size;
	do
	{
		prev_size = reduced.size();
		reduce2(reduced);
	} while (prev_size != reduced.size());
	return {reduced, reduced_init};
}

pair<int, pair<int, int> > find_max(const info_list &arr)
{
	pair<long, pair<int, int> > max_info;
	max_info.first = -1;

	info_list::const_iterator iter;
	if (arr.begin()->first > 0)
		iter = arr.begin();
	else
		iter = next(arr.begin(), 1);

	for (; iter != arr.end(); advance(iter, 1))
	{
		if (iter->first > max_info.first)
			max_info = *iter;
		if (next(iter, 1) != arr.end())
		{
			advance(iter, 1);
		}
	}

	return max_info;
}

int all_positive(const info_list &arr)
{
	int sum = 0;


	info_list::const_iterator iter;
	if (arr.begin()->first > 0)
		iter = arr.begin();
	else
		iter = next(arr.begin(), 1);

	for (; iter != arr.end(); advance(iter, 1))
	{
		sum += iter->first;
		if (next(iter, 1) != arr.end())
		{
			advance(iter, 1);
		}
	}
	return sum;
}

int max(const list<long> &orig)
{
	int max_val = -INT_MAX + 1;

	for (auto &val : orig)
	{
		if (val > max_val)
		{
			max_val = val;
		}
	}

	return max_val;
}


int main(int argc, char * argv [])
{
	int num_runs;

	cin >> num_runs;

	vector<pair<int, int> > outs;

	bool debug = false;

	for (int run = 0; run < num_runs; run++)
	{
		list<long> inputs;
		int num_inputs;
		int temp;

		cin >> num_inputs;

		for (int i = 0; i < num_inputs; i++)
		{
			cin >> temp;
			inputs.push_back(temp);
		}

		auto reduced_pair = reduce_all(inputs);
		if ((reduced_pair.second.size() == 1) && (reduced_pair.second.front().first <= 0))
		{
			int max_val = max(inputs);
			outs.push_back({max_val, max_val});
		}
		else
		{
			auto max_info = find_max(reduced_pair.first);
			int overall_max = all_positive(reduced_pair.second);
			outs.push_back({max_info.first, overall_max});
		}
	}
	for (auto &pr : outs)
	{
		cout << pr.first << " " << pr.second << endl;
	}

	return 0;
}
