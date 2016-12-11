#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

template<typename T>
void print_vec(T vec)
{
	for (auto &val : vec)
	{
		cout << val << " ";
	}
	cout << endl;
}

vector<int> find_missing(vector<int> A, vector<int> B)
{

	vector<int> missing;

	/*
	reserve enough space so that push_back will not have such a large
	performance impact
	*/
	missing.reserve(B.size() - A.size());

	sort(A.begin(), A.end());
	sort(B.begin(), B.end());

	int a_idx, b_idx;

	a_idx = b_idx = 0;

	for (; a_idx < A.size(); a_idx++)
	{
		/*
		since we sorted both A and B, and A is a subset of B, then if A[a_idx] is
		larger than B[b_idx], it means that B[b_idx] is not in A.
		*/
		while (A[a_idx] > B[b_idx])
		{
			/*
			if the last element added to missing is not equal to B[b_idx] (meaning that we
			have not added the element previously to missing) then add it to missing
			*/
			if (missing.size() == 0 || missing.back() != B[b_idx])
			{
				missing.push_back(B[b_idx]);
			}
			b_idx++;
		}
		b_idx++;
	}

	/*
	if there are any elements in B that are not in A, after going through A
	*/
	for (; b_idx < B.size(); b_idx++)
	{
		missing.push_back(B[b_idx]);
	}

	return missing;
}


int main(int argc, char * argv [])
{
	int n, m;

	cin >> n;

	vector<int> A(n);

	for (int i = 0; i < n; i++)
	{
		cin >> A[i];
	}


	cin >> m;

	vector<int> B(m);

	for (int i = 0; i < m; i++)
	{
		cin >> B[i];
	}

	auto missing = find_missing(A, B);

	print_vec(missing);

	return 0;
}
