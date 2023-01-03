
#include <iostream>
using namespace std;

struct Node {
    int *A;
    int index;
    int size;
    bool(*push)(struct Node* self, int data);
    bool(*pop)(struct Node* self);
    bool(*top)(struct Node* self, int& data);
    void(*show)(struct Node* self);
};
bool push_memberfunction(struct Node* self, int data)
{
    if (self->index == self->size) return false;
    self->A[self->index] = data;
    self->index++;
    return true;
}
bool pop_memberfunction(struct Node* self)
{
    if (self->index == 0) return false;
    self->index--;
    return true;
}
bool top_memberfunction(struct Node* self, int& data)
{
    if (self->index == 0) return false;
    data = self->A[self->index - 1];
    return true;
}
void show_memberfunction(struct Node* self)
{
    int i;
    for (i = 0; i < self->index; i++)
        cout << self->A[i] << " ";
    cout << endl;
}
void init_stack(struct Node* s, int size)
{
    s->A = new int[size];
    s->size = size;
    s->index = 0;
    s->push = push_memberfunction;
    s->pop = pop_memberfunction;
    s->top = top_memberfunction;
    s->show = show_memberfunction;
}
void free(struct Node* p) {
    delete []p->A;
    delete p;
}
int main()
{
    struct Node* s1 = { 0 };
    s1 = new struct Node;
    struct Node* s2 = { 0 };
    s2 = new struct Node;
    int size;
    cout << "輸入陣列大小 : " << endl;
    cin >> size;
    init_stack(s1,size);
    init_stack(s2,size);
    s1->push(s1, 1); s1->show(s1);
    s2->push(s2, 10); s2->show(s2);
    s2->push(s2, 20); s2->show(s2);
    s2->push(s2, 30); s2->show(s2);
    s2->pop(s2); s2->show(s2);
    s2->push(s2, 40); s2->show(s2);
    s1->push(s1, 2); s1->show(s1);
    s1->push(s1, 3); s1->show(s1);
    s1->pop(s1); s1->show(s1);
    s1->push(s1, 4); s1->show(s1);
    s1->push(s1, 5); s1->show(s1);
    free(s1);
    free(s2);
}
