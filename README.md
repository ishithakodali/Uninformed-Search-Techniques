# Uninformed-Search-Techniques
This code implements several Uninformed Search Algorithms used in Artificial Intelligence to find paths in a graph.  
The program constructs a graph in code and demonstrates how different search algorithms explore the graph to find a path between a start node and a goal node.  

## Algorithms Implemented
The following search algorithms are implemented:  
1. Breadth-First Search (BFS)  
   Explores nodes level by level using a queue.  
2. Depth-First Search (DFS)  
   Explores as deep as possible using a stack.  
3. Depth-Limited Search (DLS)  
   DFS with a fixed depth limit.  
4. Iterative Deepening Search (IDS)  
   Repeatedly performs DLS with increasing depth limits.  
5. Uniform Cost Search (UCS)
   Expands the node with the lowest path cost using a priority queue.

## Graph Representation
The graph is represented using an adjacency list.  
Nodes used in program `A, B, C, D, E, F, G`  
Edges also have weights, which are used by Uniform Cost Search.  

## Output
For each algorithm, the program displays:    
- Path from start node to goal node  
- Number of nodes expanded  
- Total cost (for UCS)  

## Compilation
Compile the program using GCC  
`gcc search_algorithms.c -o search`  

## Running the Program
`./search` 
The program will automatically construct the graph and run all search algorithms.

## Project Structure
search_algorithms.c   -> Implementation of all search algorithms  
README.md             -> Project documentation  

## Concepts Used
- Graph representation using adjacency lists
- Queue implementation for BFS
- Stack implementation for DFS
- Priority queue for UCS
- Path reconstruction
- Node expansion tracking

## Future Improvements
- Allow user input for graph nodes and edges
- Allow user-selected start and goal nodes
- Visualize graph traversal
