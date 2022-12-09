#include <iostream>
#include <iomanip>
using namespace std;
#define W 10
#define H 10
struct Node {
	int m;
	int i;
	int j;
	struct Node* pNext;
	struct Node* pNext2;
	struct Node* pup;
};
void show(int A[][W])
{
	int i;
	int j;
	cout << "   ";
	for (i = 0; i < 10; i++)
		cout << setw(2) << i;
	cout << endl;
	cout << "-------------------" << endl;
	for (i = 0; i < H; i++)
	{
		cout << "[" << i << "]";
		for (j = 0; j < W; j++)
			cout << setw(2) << A[i][j];
		cout << endl;
	}
}

void fill(int A[][W], struct Node* p, int i, int j, int m)
{
	if (A[i][j] == 0) {
		struct Node* tmp;
		tmp = new struct Node;
		A[i][j] = m;
		tmp->i = i;
		tmp->j = j;
		tmp->m = m;
		tmp->pNext = 0;
		tmp->pNext2 = 0;
		if (p->pNext == 0) {
			p->pNext = tmp;
		}
		else {
			p->pNext2 = tmp;
		}
		tmp->pup = p;
		fill(A, tmp, i - 1, j, m + 1);
		fill(A, tmp, i + 1, j, m + 1);
		fill(A, tmp, i, j - 1, m + 1);
		fill(A, tmp, i, j + 1, m + 1);
	}
}
void showWalk(struct Node* tmp) {
	cout << "(" << tmp->i << "," << tmp->j << ")" << " ";
	if (tmp->pNext != 0) {
		showWalk(tmp->pNext);
	}
	if (tmp->pNext2 != 0) {
		showWalk(tmp->pNext2);
	}
}

void freeNode(struct Node* p) {
	struct Node *tmp;
	while (p->pNext !=0 || p->pNext2 != 0) {
		if (p->pNext != 0) {
			tmp = p->pNext;
			if (!tmp->pNext && !tmp->pNext2) {
				tmp->pup = 0;
				delete tmp;
				tmp = p;
			}
		}
		else if (p->pNext2 != 0) {
			tmp = p->pNext2;
			if (!tmp->pNext && !tmp->pNext2) {
				tmp->pup = 0;
				delete tmp;
				tmp = p;
			}
		}
	}

}
void run()
{
	struct Node* root;
	root = new struct Node;
	root->i = 3;
	root->j = 4;
	root->m = 2;
	root->pNext = 0;
	root->pNext2 = 0;
	int A[H][W] = { {1,1,1,1,1,1,1,1,1,1} ,
			{1,0,0,0,0,1,1,1,0,1} ,
			{1,0,0,1,1,1,0,1,0,1} ,
			{1,0,1,0,0,0,0,1,0,1} ,
			{1,0,1,0,0,0,1,0,0,1} ,
			{1,0,1,0,0,0,1,0,0,1} ,
			{1,0,1,1,1,1,1,0,0,1} ,
			{1,0,0,0,0,0,0,0,0,1} ,
			{1,0,0,0,0,0,0,0,0,1} ,
			{1,1,1,1,1,1,1,1,1,1} };
	show(A);
	fill(A, root, 3, 3, 2);
	show(A);
	showWalk(root);
	freeNode(root);
}

int main()
{
	run();
}
