#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 20
#define QUEUE_CAP 512
#define STACK_CAP 512
#define INF INT_MAX

typedef struct EdgeNode{
    int to;
    int weight;
    struct EdgeNode *next;
}EdgeNode;

typedef struct{
    char name[16];
    EdgeNode *head;
}Node;

typedef struct{
    Node nodes[MAX_NODES];
    int n;
}Graph;

/* QUEUE */

typedef struct{
    int node[QUEUE_CAP];
    int parent[QUEUE_CAP];
    int front,rear;
}Queue;

void q_init(Queue *q){q->front=q->rear=0;}
int q_empty(Queue *q){return q->front==q->rear;}

void q_push(Queue *q,int node,int parent){
    q->node[q->rear]=node;
    q->parent[q->rear]=parent;
    q->rear=(q->rear+1)%QUEUE_CAP;
}

void q_pop(Queue *q,int *node,int *parent){
    *node=q->node[q->front];
    *parent=q->parent[q->front];
    q->front=(q->front+1)%QUEUE_CAP;
}

/* STACK */

typedef struct{
    int node[STACK_CAP];
    int parent[STACK_CAP];
    int depth[STACK_CAP];
    int top;
}Stack;

void s_init(Stack *s){s->top=-1;}
int s_empty(Stack *s){return s->top<0;}

void s_push(Stack *s,int node,int par,int d){
    s->top++;
    s->node[s->top]=node;
    s->parent[s->top]=par;
    s->depth[s->top]=d;
}

void s_pop(Stack *s,int *node,int *par,int *d){
    *node=s->node[s->top];
    *par=s->parent[s->top];
    *d=s->depth[s->top];
    s->top--;
}

/* PRIORITY QUEUE */

typedef struct{
    int node;
    int cost;
    int parent;
}PQItem;

typedef struct{
    PQItem items[QUEUE_CAP];
    int size;
}PQueue;

void pq_init(PQueue *pq){pq->size=0;}

void pq_push(PQueue *pq,int node,int cost,int parent){
    int i=pq->size++;
    pq->items[i].node=node;
    pq->items[i].cost=cost;
    pq->items[i].parent=parent;

    while(i>0){
        int p=(i-1)/2;
        if(pq->items[p].cost>pq->items[i].cost){
            PQItem tmp=pq->items[p];
            pq->items[p]=pq->items[i];
            pq->items[i]=tmp;
            i=p;
        }else break;
    }
}

PQItem pq_pop(PQueue *pq){
    PQItem top=pq->items[0];
    pq->items[0]=pq->items[--pq->size];

    int i=0;
    while(1){
        int l=2*i+1,r=2*i+2,small=i;

        if(l<pq->size && pq->items[l].cost<pq->items[small].cost)
            small=l;

        if(r<pq->size && pq->items[r].cost<pq->items[small].cost)
            small=r;

        if(small==i)break;

        PQItem tmp=pq->items[i];
        pq->items[i]=pq->items[small];
        pq->items[small]=tmp;
        i=small;
    }
    return top;
}

/* GRAPH */

Graph* graph_create(){
    Graph *g=calloc(1,sizeof(Graph));
    return g;
}

int graph_add_node(Graph *g,const char *name){
    int id=g->n++;
    strcpy(g->nodes[id].name,name);
    g->nodes[id].head=NULL;
    return id;
}

void graph_add_edge(Graph *g,int u,int v,int w){
    EdgeNode *e1=malloc(sizeof(EdgeNode));
    e1->to=v;
    e1->weight=w;
    e1->next=g->nodes[u].head;
    g->nodes[u].head=e1;

    EdgeNode *e2=malloc(sizeof(EdgeNode));
    e2->to=u;
    e2->weight=w;
    e2->next=g->nodes[v].head;
    g->nodes[v].head=e2;
}

void print_path(Graph *g,int *parent,int goal){
    if(parent[goal]==-1){
        printf("%s",g->nodes[goal].name);
        return;
    }
    print_path(g,parent,parent[goal]);
    printf(" -> %s",g->nodes[goal].name);
}

/* BFS */

void bfs(Graph *g,int start,int goal){

    int visited[MAX_NODES]={0};
    int parent[MAX_NODES];
    memset(parent,-1,sizeof(parent));

    Queue q;
    q_init(&q);

    q_push(&q,start,-1);
    visited[start]=1;

    int nodes_expanded=0;

    while(!q_empty(&q)){

        int curr,par;
        q_pop(&q,&curr,&par);

        parent[curr]=par;
        nodes_expanded++;

        if(curr==goal){
            printf("\nBFS Result\n");
            printf("Path: ");
            print_path(g,parent,goal);
            printf("\nNodes Expanded: %d\n",nodes_expanded);
            return;
        }

        for(EdgeNode *e=g->nodes[curr].head;e;e=e->next){
            if(!visited[e->to]){
                visited[e->to]=1;
                q_push(&q,e->to,curr);
            }
        }
    }
}

/* DFS */

void dfs(Graph *g,int start,int goal){

    int visited[MAX_NODES]={0};
    int parent[MAX_NODES];
    memset(parent,-1,sizeof(parent));

    Stack s;
    s_init(&s);
    s_push(&s,start,-1,0);

    int nodes_expanded=0;

    while(!s_empty(&s)){

        int curr,par,depth;
        s_pop(&s,&curr,&par,&depth);

        if(visited[curr])continue;

        visited[curr]=1;
        parent[curr]=par;
        nodes_expanded++;

        if(curr==goal){
            printf("\nDFS Result\n");
            printf("Path: ");
            print_path(g,parent,goal);
            printf("\nNodes Expanded: %d\n",nodes_expanded);
            return;
        }

        for(EdgeNode *e=g->nodes[curr].head;e;e=e->next){
            if(!visited[e->to]){
                s_push(&s,e->to,curr,depth+1);
            }
        }
    }
}

/* DLS */

int dls_recursive(Graph *g,int curr,int goal,int limit,
                  int *visited,int *parent,int *count){

    visited[curr]=1;
    (*count)++;

    if(curr==goal)return 1;
    if(limit==0)return 0;

    for(EdgeNode *e=g->nodes[curr].head;e;e=e->next){

        if(!visited[e->to]){

            parent[e->to]=curr;

            if(dls_recursive(g,e->to,goal,limit-1,
                             visited,parent,count))
                return 1;
        }
    }

    visited[curr]=0;
    return 0;
}

void dls(Graph *g,int start,int goal,int limit){

    int visited[MAX_NODES]={0};
    int parent[MAX_NODES];
    memset(parent,-1,sizeof(parent));

    int count=0;

    int result=dls_recursive(g,start,goal,limit,
                             visited,parent,&count);

    printf("\nDLS Result (limit=%d)\n",limit);

    if(result){
        printf("Path: ");
        print_path(g,parent,goal);
        printf("\nNodes Expanded: %d\n",count);
    }else{
        printf("Goal not found within depth limit\n");
    }
}

/* IDS */

void ids(Graph *g,int start,int goal){

    for(int depth=0;depth<=g->n;depth++){

        int visited[MAX_NODES]={0};
        int parent[MAX_NODES];
        memset(parent,-1,sizeof(parent));

        int count=0;

        if(dls_recursive(g,start,goal,depth,
                         visited,parent,&count)){

            printf("\nIDS Result\n");
            printf("Depth Found: %d\n",depth);
            printf("Path: ");
            print_path(g,parent,goal);
            printf("\nNodes Expanded: %d\n",count);
            return;
        }
    }
}

/* UCS */

void ucs(Graph *g,int start,int goal){

    int dist[MAX_NODES];
    int visited[MAX_NODES]={0};
    int parent[MAX_NODES];

    for(int i=0;i<g->n;i++){
        dist[i]=INF;
        parent[i]=-1;
    }

    dist[start]=0;

    PQueue pq;
    pq_init(&pq);
    pq_push(&pq,start,0,-1);

    int nodes_expanded=0;

    while(pq.size>0){

        PQItem item=pq_pop(&pq);
        int curr=item.node;

        if(visited[curr])continue;

        visited[curr]=1;
        parent[curr]=item.parent;
        nodes_expanded++;

        if(curr==goal){
            printf("\nUCS Result\n");
            printf("Path: ");
            print_path(g,parent,goal);
            printf("\nTotal Cost: %d\n",item.cost);
            printf("Nodes Expanded: %d\n",nodes_expanded);
            return;
        }

        for(EdgeNode *e=g->nodes[curr].head;e;e=e->next){

            if(!visited[e->to]){

                int new_cost=item.cost+e->weight;

                if(new_cost<dist[e->to]){

                    dist[e->to]=new_cost;

                    pq_push(&pq,e->to,new_cost,curr);
                }
            }
        }
    }
}

/* MAIN */

int main(){

    Graph *g=graph_create();

    int A=graph_add_node(g,"A");
    int B=graph_add_node(g,"B");
    int C=graph_add_node(g,"C");
    int D=graph_add_node(g,"D");
    int E=graph_add_node(g,"E");
    int F=graph_add_node(g,"F");
    int G=graph_add_node(g,"G");

    graph_add_edge(g,A,B,1);
    graph_add_edge(g,A,C,3);
    graph_add_edge(g,B,D,1);
    graph_add_edge(g,B,E,2);
    graph_add_edge(g,C,D,2);
    graph_add_edge(g,D,F,1);
    graph_add_edge(g,D,G,3);
    graph_add_edge(g,E,F,4);

    int start=A;
    int goal=G;

    printf("\nUNINFORMED SEARCH ALGORITHMS\n");

    bfs(g,start,goal);
    dfs(g,start,goal);
    dls(g,start,goal,3);
    ids(g,start,goal);
    ucs(g,start,goal);

    return 0;
}
