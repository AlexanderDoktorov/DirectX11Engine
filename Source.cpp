#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <list>
#include <functional>
#include <stack>
#include <algorithm>

// Create a vector of size n with vector<int> vect(n, 10) all values as 10.

// cout << charsums[i] << '\t' << i << endl;
/*cout << pair.first << '\t';
for (auto words : pair.second)
{
	cout << words;
}
cout << endl;*/

// cout << pair.first << '\t' << pair.second << endl;

using namespace std;

int Factorial(const int& n)
{
	if (n == 0 || n == 1) return 1;
	return(n * Factorial(n - 1));
}

struct ListNode {
	int val;
	ListNode* next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
	ListNode* reverseList(ListNode* head) {

		ListNode* temp = head;
		ListNode* newhead = head;
		ListNode* beforenewhead = head;

		while (temp->next != nullptr)
		{
			beforenewhead = temp;
			temp = temp->next;
		}

		newhead = temp; // newhead = tail
		newhead->next = beforenewhead;
		beforenewhead->next = nullptr;

		int b = 0;

		return newhead;
	}
};


int main(int argc, char* argv[])
{
	Solution sln;

	ListNode second(7);
	ListNode first(4, &second);
	ListNode head(1, &first);

	head.next = &first;
	first.next = &second;

	ListNode* tmp = &head;

	do
	{
		cout << tmp->val;
		tmp = tmp->next;
	} while (tmp != nullptr);

	sln.reverseList(&head);

	tmp = &head;

	do 
	{
		cout << tmp->val;
		tmp = tmp->next;
	} while (tmp != nullptr);

	return 0;
}

