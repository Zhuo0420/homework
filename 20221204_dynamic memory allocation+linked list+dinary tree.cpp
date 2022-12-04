#include <iostream>
using namespace std;

char ch = 'a';
int counter = 1;
struct Node {
    char data;
    struct Node* pLeft;
    struct Node* pRight;
    struct Node* pup;
};

void addNodeLeft(struct Node* tmp) {
    tmp->pLeft = new struct Node;
    tmp->pLeft->data = ch;
    ch++;
    tmp->pLeft->pup = tmp;
    tmp->pLeft->pLeft = 0;
    tmp->pLeft->pRight = 0;
}

void addNodeRight(struct Node* tmp) {
    tmp->pRight = new struct Node;
    tmp->pRight->data = ch;
    ch++;
    tmp->pRight->pup = tmp;
    tmp->pRight->pLeft = 0;
    tmp->pRight->pRight = 0;
}

void showNode(struct Node* tmp) {
    cout << tmp->data << " ";
    counter = 1;
    int a = 1;
    while (a <= 7) {
        if (tmp!=0 && tmp->pLeft != 0 && counter != 3) {
            a++;
            cout << tmp->pLeft->data << " ";           
            if (a == 7)
                break;
        }
        if (tmp != 0 && tmp->pRight != 0 && counter != 3) {
            cout << tmp->pRight->data << " ";
            a++;
            if (a == 7)
                break;
        }
        if (tmp != 0 && counter != 3 ) {
            tmp = tmp->pLeft;
            counter++;
        }
        else if (tmp != 0 && counter == 3) {
            tmp = tmp->pup->pup->pRight;
            counter -= 1;
        }
    }
}

void freeNode(struct Node* tmp) {
    struct Node* p;
    p = tmp;        //p=root
    while (p->pLeft != 0 && p->pRight != 0) {
        if (tmp->pLeft != 0) {
            tmp = tmp->pLeft;
            if (!tmp->pLeft && !tmp->pRight) {
                tmp->pup->pLeft = 0;
                delete tmp;
                tmp = p;
            }
            
        }
        else if (tmp->pRight != 0) {
            tmp = tmp->pRight;
            if (!tmp->pLeft && !tmp->pRight) {
                tmp->pup->pRight = 0;
                delete tmp;
                tmp = p;
            }
        }
    }
}
int main()
{
    struct Node* root;
    root = new struct Node;
    struct Node* tmp;
    root->data = ch;
    ch++;
    tmp = root;
    tmp->pLeft = 0;
    tmp->pRight = 0;
    while (ch <= 'g') {
        if (tmp->pLeft == 0 && counter != 3) {
            addNodeLeft(tmp);
        }
        if (tmp->pRight == 0 && counter != 3) {
            addNodeRight(tmp);
        }
        if(counter != 3 && tmp->pLeft !=0 && tmp->pRight != 0) {
            tmp = tmp->pLeft;
            counter++;
        }
        else if (counter == 3) {
            tmp = tmp->pup->pup->pRight;
            counter -= 1;
        }
    }
    showNode(root);
    freeNode(root);
    delete root;
    return 0;
}
