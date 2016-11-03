#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct node
{
	node * left;
	node * right;
	int val;
	int num;
};


int find_rightmost_val(node * root)
{
	if (root->right)
	{
		return find_rightmost_val(root->right);
	}
	else
	{
		return root->val;
	}
}

int find_leftmost_val(node * root)
{
	if (root->left)
	{
		return find_leftmost_val(root->left);
	}
	else
	{
		return root->val;
	}
}

void calc_num(node * root)
{
	root->num = 1;
	if (root->right)
	{
		root->num += root->right->num;
	}
	if (root->left)
	{
		root->num += root->left->num;
	}
}

node * rotate_left(node *);

node * rotate_right(node * root)
{
	while (root->right && root->right->left)
	{
		root->right = rotate_left(root->right);
	}

	node * temp;

	temp = root;

	root = root->right;
	temp->right = root->left;
	root->left = temp;
	
	calc_num(root->left);
	calc_num(root);

	return root;
}


node * rotate_left(node * root)
{
	while (root->left && root->left->right)
	{
		root->left = rotate_right(root->left);
	}

	node * temp;

	temp = root;

	root = root->left;
	temp->left = root->right;
	root->right = temp;

	calc_num(root->right);
	calc_num(root);

	return root;
}

pair<int, int> left_right_height(node * root)
{
	int left_ht, right_ht;

	if (root->right)
	{
		right_ht = root->right->num;
	}
	else
	{
		right_ht = 0;
	}
	if (root->left)
	{
		left_ht = root->left->num;
	}
	else
	{
		left_ht = 0;
	}

	return {left_ht, right_ht};
}

node * rotate(node * root)
{
	auto heights = left_right_height(root);
	while (abs(heights.first - heights.second) > 1)
	{
		if (heights.second > heights.first + 1)
		{
			root = rotate_right(root);
		}
		else if (heights.second + 1 < heights.first)
		{
			root = rotate_left(root);
		}
		heights = left_right_height(root);
	}
	return root;
}

node * insert_heap(node * root, int val, bool top)
{
	if (!root)
	{
		root = new node;
		root->val = val;
		root->num = 1;
		root->right = nullptr;
		root->left = nullptr;
		return root;
	}
	root->num++;

	if (root->val > val)
	{
		root->left = insert_heap(root->left, val, false);
	}
	else
	{
		root->right = insert_heap(root->right, val, false);
	}
	
	if (top)
	{
		root = rotate(root);
	}
	return root;
}


double find_median(node * root)
{
	pair<int, int> target;
	if (root->num % 2 == 0)
	{
		target.first = root->num/2;
		target.second = root->num/2 + 1;
	}
	else
		target.first = target.second = (root->num + 1)/2;

	auto left_right = left_right_height(root);

	int left_val = left_right.first;
	int right_val = left_right.second;

	pair<int, int> values;

	if (left_val > right_val)
	{
		values.first = root->val;
		values.second = find_rightmost_val(root->left);
	}
	else if (left_val < right_val)
	{
		values.second = root->val;
		values.first = find_leftmost_val(root->right);
	}
	else
		values.first = values.second = root->val;

	
	return (double)(values.first + values.second)/2.0;
	
}
		
	
int count;

void count_nodes(node * root)
{
	count++;
	if (root->left)
	{
		count_nodes(root->left);
	}
	if (root->right)
	{
		count_nodes(root->right);
	}
}

void free_tree(node * root)
{
	if (root->right)
	{
		free_tree(root->right);
	}
	if (root->left)
	{
		free_tree(root->left);
	}
	delete root;
}

int main(int argc, char *argv [])
{
	default_random_engine generator;
	auto int_rand = uniform_int_distribution<int>(0, 10000);
	node * root = nullptr;
	for (int i = 10000; i > 0; i--)
	{
		root = insert_heap(root, int_rand(generator), true);
	}
	cout << "root value: " << root->val << endl;
	cout << "num elements: " << root->num << endl;

	count = 0;
	count_nodes(root);
	cout << "nodes counted: " << count << endl;

	/*
	int num_values;
	int value;

	node * root = nullptr;

	cin >> num_values;

	vector<double> medians;

	for (int i = 0; i < num_values; i++)
	{
		double temp;
		cin >> value;
		root = insert_heap(root, value, true);
		medians.push_back(find_median(root));
	}

	for (auto &med : medians)
	{
		cout << med << endl;
	}
	*/

	free_tree(root);

	return 0;
}

