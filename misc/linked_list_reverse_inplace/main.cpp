#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct node
{
	node(shared_ptr<node> next, int val) : next(next), val(val)
	{}
	shared_ptr<node> next;
	int val;
};

void reverse_list(shared_ptr<node> &root)
{
	shared_ptr<node> temp;
	shared_ptr<node> prev = nullptr;
	shared_ptr<node> curr_node = root;

	while (curr_node)
	{
		if (!curr_node->next)
			root = curr_node;
		temp = curr_node->next;
		curr_node->next = prev;
		prev = curr_node;
		curr_node = temp;
	}
}

void print_list(shared_ptr<node> root)
{
	shared_ptr<node> curr_node = root;
	while (curr_node)
	{
		cout << curr_node->val;
		if (curr_node->next)
		{
			cout << " => ";
		}
		else
		{
			cout << endl;
		}
		curr_node = curr_node->next;
	}
}

shared_ptr<node> create_list(vector<int> inputs)
{
	shared_ptr<node> prev;
	shared_ptr<node> root;

	for (int i = 0; i < inputs.size(); i++)
	{
		if (i == 0)
		{
			root = make_shared<node>(nullptr, inputs.at(i));
			prev = root;
		}
		else
		{
			prev->next = make_shared<node>(nullptr, inputs.at(i));
			prev = prev->next;
		}
	}


	return root;
}

int main(int argc, char * argv [])
{
	auto my_list = create_list({4, 9, 5, 3, 2});
	cout << "Normal list: " << endl;
	print_list(my_list);
	reverse_list(my_list);
	cout << "Reversed list: " << endl;
	print_list(my_list);
	return 0;
}
