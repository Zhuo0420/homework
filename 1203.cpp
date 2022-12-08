#include "pch.h"
#include <iostream>
#include <iomanip>
using namespace std;
#define W 10
#define H 10
struct Node {
	int m;
	int i;
	int j;
	struct Node *pNext;
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

void fill(int A[][W],struct Node *tmp,int i, int j,int m)
{
	struct Node *tmpN;
	tmp->pNext = tmpN;
	tmpN->pNext = 0;
	if (A[i][j] == 0)
	{
		A[i][j] = m;
		tmpN->i = i;
		tmpN->j = j;
		fill(A,tmpN,i+1,j,m+1);
		fill(A,tmpN,i-1,j,m+1);
		fill(A,tmpN,i,j+1,m+1);
		fill(A,tmpN,i,j-1,m+1);
	}
}
void showWalk(struct Node *tmp) {
	while (tmp->pNext != 0) {
		cout << "(" << tmp->i << "," << tmp->j << ")" << " ";
	}
}

void run()
{
	struct Node *root;
	root->i = 3;
	root->j = 4;
	root->m = 2;
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
	fill(A,root,3,4,2);
	show(A);
	showWalk(root);
}

int main()
{
	run();
}
