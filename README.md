# Movie Streaming Data Structures (C)

A low-level implementation of a movie streaming database system developed in C. This project focuses on manual memory management, pointer manipulation, and the transition from linear to hierarchical data structures to optimize performance.

## Project Overview  

The system was developed in two phases:

### **Phase A: Linked Lists**
* **Implementation:** Double Linked Lists.
* **Objective:** Establish the core logic for movie record management, including insertion, deletion, and filtering.

### **Phase B: Binary Search Trees**
* **Implementation:** Binary Search Trees.
* **Objective:** Refactor the backend to support logarithmic search times.
### Key Technical Features


##  Features
* **Manual Memory Management:** Use of `malloc` and `free` with attention to memory leaks.
* **Custom Search Algorithms:** Custom logic for filtering movie metadata.
* **Data Persistence:** Handling of input/output streams for movie databases.
* **Event-Driven Architecture:** The system processes specific event codes to manage the database state:
    * `R / U`: User Registration & Unregistration logic.
    * `A / D`: Movie addition and distribution across thematic categories.
    * `I / W`: Targeted search by ID and viewing history management.
    * `F / Q`: Complex filtering and user rating calculations.
* **File I/O & Parsing:** Initializes the system state by parsing user-provided configuration files.

  
## How to Run
Navigate to the specific phase directory and use the provided Makefile:
```bash
cd phase_a
make
./movie_service
```
---
*This project was developed for the Data Structures Course*
