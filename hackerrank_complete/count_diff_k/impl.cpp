#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;


int count_K_diff(const vector<int> &nums, int K)
{
	int fwd_idx, back_idx;

	int curr_idx = 0;
	bool next_idx = false;

	fwd_idx = curr_idx + 1;
	back_idx = nums.size() - 1;

	int count = 0;

	while (curr_idx < nums.size())
	{
		int fwd_diff = abs(nums[curr_idx] - nums[fwd_idx]);
		int back_diff = abs(nums[curr_idx] - nums[back_idx]);
		
		if (((long long)(fwd_diff - K))*((long long)(back_diff - K)) > 0)
		{
			//cout << "mult same at curr_idx = " << curr_idx << endl;
			next_idx = true;
		}
		
		//else
		//{
			//cout << "doing some checking..." << endl;
		//}
		if (!next_idx && (abs(fwd_diff) == K))
		{
			next_idx = true;
			while ((abs(nums[curr_idx] - nums[fwd_idx]) == K) && (fwd_idx <= back_idx))
			{
				fwd_idx++;
				count++;
			}
		}
		else if (!next_idx && (abs(back_diff) == K))
		{
			next_idx = true;
			while ((abs(nums[curr_idx] - nums[back_idx]) == K) && (back_idx >= fwd_idx))
			{
				back_idx--;
				count++;
			}
		}
		if (next_idx || (fwd_idx >= back_idx))
		{
			curr_idx++;
			fwd_idx = curr_idx + 1;
			back_idx = nums.size() - 1;
			next_idx = false;
		}
		else
		{
			fwd_idx++;
			back_idx--;
		}
	}
	return count;
}

int main(int argc, char * argv [])
{
	int size, K;

	cin >> size;
	cin >> K;

	vector<int> nums(size);
	
	for (int i = 0; i < size; i++)
	{
		cin >> nums.at(i);
	}
	sort(nums.begin(), nums.end());

	cout << count_K_diff(nums, K) << endl;
	
	return 0;
}
