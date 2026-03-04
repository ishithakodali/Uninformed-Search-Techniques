#include <stdio.h>
#include <limits.h>

#define MAX 10

int graph[MAX][MAX] = {
    {0,1,4,0,0,0,0},
    {1,0,2,5,0,0,0},
    {4,2,0,1,3,0,0},
    {0,5,1,0,2,6,0},
    {0,0,3,2,0,4,7},
    {0,0,0,6,4,0,1},
    {0,0,0,0,7,1,0}
};

int visited[MAX];
int n = 7;

/* ---------- BFS ---------- */

void bfs(int start){

    int queue[MAX];
    int front=0,rear=0;

    for(int i=0;i<n;i++)
        visited[i]=0;

    queue[rear++] = start;
    visited[start]=1;

    printf("\nBFS Order: ");

    while(front<rear){

        int node = queue[front++];
        printf("%d ",node);

        for(int i=0;i<n;i++){
            if(graph[node][i] && !visited[i]){
                visited[i]=1;
                queue[rear++] = i;
            }
        }
    }
}

/* ---------- DFS ---------- */

void dfs_util(int node){

    visited[node]=1;
    printf("%d ",node);

    for(int i=0;i<n;i++){
        if(graph[node][i] && !visited[i])
            dfs_util(i);
    }
}

void dfs(int start){

    for(int i=0;i<n;i++)
        visited[i]=0;

    printf("\nDFS Order: ");
    dfs_util(start);
}

/* ---------- DLS ---------- */

int dls(int node,int goal,int limit){

    printf("%d ",node);

    if(node==goal)
        return 1;

    if(limit<=0)
        return 0;

    visited[node]=1;

    for(int i=0;i<n;i++){
        if(graph[node][i] && !visited[i]){
            if(dls(i,goal,limit-1))
                return 1;
        }
    }

    return 0;
}

/* ---------- IDS ---------- */

void ids(int start,int goal){

    printf("\nIDS Path: ");

    for(int depth=0;depth<n;depth++){

        for(int i=0;i<n;i++)
            visited[i]=0;

        if(dls(start,goal,depth)){
            printf("\nGoal found at depth %d\n",depth);
            return;
        }
    }
}

/* ---------- UCS ---------- */

void ucs(int start,int goal){

    int cost[MAX];
    int visited[MAX]={0};

    for(int i=0;i<n;i++)
        cost[i]=INT_MAX;

    cost[start]=0;

    while(1){

        int min=INT_MAX;
        int node=-1;

        for(int i=0;i<n;i++){
            if(!visited[i] && cost[i]<min){
                min=cost[i];
                node=i;
            }
        }

        if(node==-1)
            break;

        visited[node]=1;

        if(node==goal){
            printf("\nUCS Cost from %d to %d = %d\n",start,goal,cost[node]);
            return;
        }

        for(int i=0;i<n;i++){
            if(graph[node][i]){
                int new_cost = cost[node] + graph[node][i];

                if(new_cost < cost[i])
                    cost[i] = new_cost;
            }
        }
    }
}

/* ---------- MAIN ---------- */

int main(){

    int start = 0;
    int goal = 6;

    bfs(start);
    dfs(start);

    for(int i=0;i<n;i++)
        visited[i]=0;

    printf("\nDLS Path (limit=3): ");
    if(!dls(start,goal,3))
        printf("\nGoal not found within limit");

    ids(start,goal);

    ucs(start,goal);

    return 0;
}
