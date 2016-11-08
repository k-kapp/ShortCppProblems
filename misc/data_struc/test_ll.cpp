#include "linked_list.h"
#include <vector>
#include <iostream>

int main(int argc, char * argv [])
{
	auto list_obj = linked_list<int>({4, 5, 6, 8, 9});
	//auto found = list_obj.find<int>(6, [](int fst, int snd) {return fst == snd;});
	auto found = list_obj.find(6);
	cout << *found << endl;
	list_obj.print();
	list_obj.pop_back();
	list_obj.print();
	list_obj.pop_front();
	list_obj.print();
	list_obj.erase(1);
	list_obj.print();
	list_obj.erase(1);
	list_obj.print();
	list_obj.erase(1);
	list_obj.print();
	list_obj.erase(0);
	list_obj.print();


	return 0;
}
