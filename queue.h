typedef struct queue_node
{
    void* key;
    struct queue_node* next;
}queue_node;

typedef struct queue
{
    queue_node* front;
    queue_node* back;
    int (*size)(struct queue*);
}queue;

queue_node* new_node(void* thread);
queue* create_queue();
void enqueue(queue* q, void* thread);
void dequeue(queue* q);
int size(queue* q);
void free_queue(queue** q);