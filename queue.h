typedef struct node {
    int data; 
    int priority;
    struct node* next; 
} Node;
Node* newNode(int d, int p);
int peek(Node** head);
int getPriority(Node** head);
Node* getNode(Node** head);
void pop(Node** head);
void push(Node** head, int d, int p);
void pushFifo(Node** head, int d, int p);
int isEmpty(Node** head);
int queueSize(Node** head);
