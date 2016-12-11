#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct sorting
{
	sorting(int start_idx) 
		: start_idx(start_idx)
	{}

	bool ascending;
	int start_idx, end_idx;
};

sorting check_segment(const vector<int> &values, int start_idx)
{
	sorting sort_info(start_idx);
	bool determined = false;

	int curr_idx = start_idx;

	while (curr_idx < values.size() - 1)
	{
		if (values[curr_idx + 1] != values[curr_idx])
		{
			if (!determined)
			{
				if (values[curr_idx + 1] > values[curr_idx])
				{
					sort_info.ascending = true;
				}
				else
				{
					sort_info.ascending = false;
				}
				determined = true;
			}
			else if (sort_info.ascending)
			{
				if (values[curr_idx + 1] < values[curr_idx])
				{
					sort_info.end_idx = curr_idx;
					return sort_info;
				}
			}
			else
			{
				if (values[curr_idx + 1] > values[curr_idx])
				{
					sort_info.end_idx = curr_idx;
					return sort_info;
				}
			}
		}
		curr_idx++;
	}
	sort_info.end_idx = curr_idx;
	return sort_info;
}

vector<sorting> make_sortings(const vector<int> &values)
{
	sorting sort_info = check_segment(values, 0);
	vector<sorting> sortings;
	sortings.push_back(sort_info);

	while (sort_info.end_idx != values.size() - 1)
	{
		sort_info = check_segment(values, sort_info.end_idx);
		sortings.push_back(sort_info);
	}

	return sortings;
}


int main(int argc, char * argv [])
{
	int n;
	cin >> n;

	vector<int> values(n);

	for (int i = 0; i < n; i++)
	{
		cin >> values[i];
	}

	auto sortings = make_sortings(values);

	if (sortings.size() == 1)
	{
		if (sortings[0].ascending)
		{
			cout << "yes" << endl;
		}
		else
		{
			cout << "yes" << endl;
			if (sortings[0].start_idx == sortings[0].end_idx - 1)
			{
				cout << "swap 1 2" << endl;
			}
			else
			{
				cout << "reverse 1 " << sortings[0].end_idx << endl;
			}
		}
		return 0;
	}

	if (sortings.size() == 3)
	{
		if (sortings[0].ascending && !sortings[1].ascending && sortings[2].ascending)
		{
			if (values[sortings[0].end_idx - 1] <= values[sortings[1].end_idx])
			{
				if (values[sortings[1].start_idx] <= values[sortings[2].start_idx + 1])
				{
					cout << "yes" << endl;
					if (sortings[1].start_idx == sortings[1].end_idx - 1)
						cout << "swap " << sortings[1].start_idx + 1 << " " << sortings[1].end_idx + 1 << endl;
					else
						cout << "reverse " << sortings[1].start_idx + 1 << " " << sortings[1].end_idx + 1 << endl;
					return 0;
				}
			}
		}
	}

	if (sortings.size() == 2)
	{
		if (sortings[0].ascending)
		{
			if (values[sortings[1].end_idx] >= values[sortings[0].end_idx - 1])
			{
				cout << "yes" << endl;
				if (sortings[1].start_idx == sortings[1].end_idx - 1)
					cout << "swap " << sortings[1].start_idx + 1 << " " << sortings[1].end_idx + 1 << endl;
				else
					cout << "reverse " << sortings[1].start_idx + 1 << " " << sortings[1].end_idx + 1 << endl;
				return 0;
			}
		}
		else
		{
			if (values[sortings[0].start_idx] <= values[sortings[1].start_idx + 1])
			{
				cout << "yes" << endl;
				if (sortings[0].start_idx == sortings[0].end_idx - 1)
					cout << "swap " << sortings[0].start_idx + 1 << " " << sortings[0].end_idx + 1 << endl;
				else
					cout << "reverse " << sortings[0].start_idx + 1 << " " << sortings[0].end_idx + 1 << endl;
				return 0;
			}
		}
	}

	if (sortings.size() == 5)
	{
		if (sortings[1].start_idx == sortings[1].end_idx - 1 &&
				sortings[3].start_idx == sortings[3].end_idx - 1)
		{
			int first_large = values[sortings[1].start_idx];
			int first_large_idx = sortings[1].start_idx;

			int second_small = values[sortings[3].end_idx];
			int second_small_idx = sortings[3].end_idx;

			if (first_large >= values[second_small_idx - 1] &&
					first_large <= values[second_small_idx + 1])
			{
				if (second_small >= values[first_large_idx - 1] &&
						second_small <= values[first_large_idx + 1])
				{
					cout << "yes" << endl;
					cout << "swap " << first_large_idx + 1 << " " << second_small_idx + 1 << endl;
					return 0;
				}
			}
		}
	}

	cout << "no" << endl;

	return 0;
}
