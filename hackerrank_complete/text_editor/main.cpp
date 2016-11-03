#include <iostream>
#include <string>
#include <vector>
#include <list>

using namespace std;

typedef enum
{
	APPEND = 1,
	DELETE,
	PRINT,
	UNDO
} op;

struct operation
{
	operation(op type, int added, string deleted)
				: type(type), added(added), deleted(deleted)
	{}
	op type;
	int added;
	string deleted;
};

class stack
{
public:
	void append(string W)
	{
		ops.push_back({APPEND, (int)W.size(), string()});
		main_str += W;
	}

	void undo_append()
	{
		main_str.erase(main_str.size() - ops.back().added);
		ops.pop_back();
	}

	void del(unsigned int k)
	{
		ops.push_back({DELETE, 0, string(next(main_str.begin(), main_str.size() - k), main_str.end())});
		main_str.erase(main_str.size() - k);
	}

	void undo_del()
	{
		main_str += ops.back().deleted;
		ops.pop_back();
	}
	
	void print(unsigned int k)
	{
		print_chars += main_str.at(k - 1);
	}

	void undo()
	{
		if (ops.back().type == APPEND)
		{
			undo_append();
		}
		else
		{
			undo_del();
		}
	}

	void output()
	{
		for (auto &chr : print_chars)
		{
			cout << chr << endl;
		}
	}

	void take_input()
	{
		int num_ops;
		cin >> num_ops;

		for (int i = 0; i < num_ops; i++)
		{
			int op_type_temp;
			string str_arg;
			int int_arg;

			cin >> op_type_temp;
			op op_type = (op)op_type_temp;
			switch (op_type)
			{
				case (APPEND):
				{
					cin >> str_arg;
					append(str_arg);
				} break;
				case (DELETE):
				{
					cin >> int_arg;
					del(int_arg);
				} break;
				case (PRINT):
				{
					cin >> int_arg;
					print(int_arg);
				} break;
				case (UNDO):
				{
					undo();
				} break;
				default:
				{}
			}
		}
		output();
	}

private:
	string main_str;
	list<operation> ops;
	string print_chars;
};


int main(int argc, char * argv [])
{
	stack obj;
	obj.take_input();
	return 0;
}
