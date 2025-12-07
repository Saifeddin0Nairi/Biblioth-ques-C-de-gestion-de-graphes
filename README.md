# Graph Management Libraries in C

This project implements **four reusable C libraries** for manipulating graphs  
(unweighted / weighted, adjacency list / adjacency matrix) and several  
**classical graph algorithms** that use these libraries.

This project was done as part of the course *Bibliothèques C de gestion de graphes*.

---

## 📁 Project Structure
/
├── Algorithms/
│ ├── algos_non_values.c # DFS, BFS, connectivity
│ ├── algos_values.c # Dijkstra
│ └── main.c # Test program
│
└── Libraries/
├── GrapheListe.c
├── GrapheListe.h
├── GrapheMatrice.c
├── GrapheMatrice.h
├── GrapheValueListe.c
├── GrapheValueListe.h
├── GrapheValueMatrice.c
└── GrapheValueMatrice.h

---

## 📚 Part A — Implemented Graph Libraries

The project includes **four modular graph libraries**:

### 1. `GrapheMatrice`  
Representation: **adjacency matrix**  
Type: **unweighted**, **directed or undirected**

### 2. `GrapheListe`  
Representation: **adjacency list**  
Type: **unweighted**, **directed or undirected**

### 3. `GrapheValueMatrice`  
Representation: **adjacency matrix**  
Type: **weighted**

### 4. `GrapheValueListe`  
Representation: **adjacency list**  
Type: **weighted**

---

## ✨ Core Features of All Libraries

Each graph library provides:

- Dynamic creation and destruction  
- Adding and removing vertices  
- Adding and removing edges / arcs  
- Checking if an edge exists  
- Getting graph order (number of vertices)  
- Computing in-degree, out-degree, total degree  
- Checking if the graph is directed  

### Weighted graph libraries also provide:
- Getting the weight of an edge  
- Modifying the weight of an edge  

---

## 🧠 Part B — Implemented Algorithms

All algorithms are implemented **using the above libraries**, not raw pointers.

### ✔ Depth-First Search (DFS)
- Full graph exploration  
- Printing connected components (non-oriented graphs)

### ✔ Breadth-First Search (BFS)
- Level-order exploration  
- Computing shortest distances in **unweighted** graphs

### ✔ Connectivity
- Check if a graph is connected (undirected)
- Check if a graph is strongly connected (directed)

### ✔ Dijkstra’s Algorithm
- Computes shortest paths in **weighted** graphs  
- Implemented with adjacency lists (`GrapheValueListe`)

---

## ▶️ How to Compile (MSYS2 / GCC)

From the project root directory:

```bash
gcc -Wall -Wextra -std=c11 \
    -I Libraries \
    Libraries/GrapheMatrice.c \
    Libraries/GrapheListe.c \
    Libraries/GrapheValueMatrice.c \
    Libraries/GrapheValueListe.c \
    Algorithms/algos_non_values.c \
    Algorithms/algos_values.c \
    Algorithms/main.c \
    -o graph_project
Then run:

./graph_project
🧪 Testing

The file main.c contains sample tests demonstrating:

Creating graphs

Adding/removing vertices and edges

Running DFS, BFS, Dijkstra

Displaying degrees, connectivity, distances

You can modify main.c to build additional tests.

🛠 Requirements

GCC (MinGW-w64 for Windows users)

MSYS2 recommended

C11 standard