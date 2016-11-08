/*
	this is a procedural implementation of a singly linked list. A doubly linked list
		implemented in an object oriented fashion may be found in linked_list.h in this
		directory
*/

#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct node
{
	node(shared_ptr<node> next, int val) : next(next), val(val)
	{}
	node(int val) : next(nullptr), val(val)
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
			root = make_shared<node>(inputs.at(i));
			prev = root;
		}
		else
		{
			prev->next = make_shared<node>(inputs.at(i));
			prev = prev->next;
		}
	}


	return root;
}

void insert(shared_ptr<node> &root, int val, int idx)
{
	if (!root || idx == 0)
	{
		auto new_node = make_shared<node>(root, val);
		root = new_node;
	}
	else
		insert(root->next, val, idx - 1);
}

void append(shared_ptr<node> &root, int val)
{
	if (!root)
	{
		root = make_shared<node>(nullptr, val);
	}
	else
	{
		append(root->next, val);
	}
}

void erase(shared_ptr<node> &root, int idx)
{
	if (!root)
		return;
	if (idx > 0 && !root->next)
	{
		return;
	}
	else if (idx == 0)
	{
		root = root->next;
		return;
	}
	else
	{
		erase(root->next, idx - 1);
	}
}

void pop_back(shared_ptr<node> &root)
{
	if (!root)
		return;
	if (root->next)
	{
		pop_back(root->next);
	}
	else
	{
		root = root->next;
	}
}

void pop_front(shared_ptr<node> &root)
{
	if (!root)
		return;
	else
		root = root->next;
}

int main(int argc, char * argv [])
{
	auto my_list = create_list({4, 9, 5, 3, 2});
	cout << "Normal list: " << endl;
	print_list(my_list);
	reverse_list(my_list);
	cout << "Reversed list: " << endl;
	print_list(my_list);
	pop_back(my_list);
	print_list(my_list);
	pop_front(my_list);
	print_list(my_list);
	erase(my_list, 1);
	print_list(my_list);
	insert(my_list, 10, 1);
	print_list(my_list);
	append(my_list, 100);
	print_list(my_list);
	insert(my_list, 500, 0);
	print_list(my_list);
	return 0;
}
