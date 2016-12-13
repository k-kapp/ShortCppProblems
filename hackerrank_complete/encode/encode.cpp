#include <cmath>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

string de_space(string message)
{
	string new_message;
	new_message.reserve(message.size());
	
	for (auto &ch : message)
		if (ch != ' ')
			new_message.push_back(ch);

	return new_message;
}

void print_encoded(const string &message, int rows, int cols)
{
	
	for (int i = 0; i < cols; i++)
	{
		for (int j = i; j < message.size(); j += cols)
		{
			cout << message[j];
		}
		cout << " ";
	}
	cout << endl;
}
		


int main(int argc, char * argv [])
{
	string message;

	cin >> message;

	double sqroot = sqrt(message.size());

	int lower = floor(sqroot);
	int upper = ceil(sqroot);

	int rows = lower;
	int columns = upper;

	if (rows*columns < message.size())
		rows += 1;

	print_encoded(de_space(message), rows, columns);

	return 0;
}
