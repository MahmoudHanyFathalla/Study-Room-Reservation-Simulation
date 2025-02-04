# Study Room Reservation Simulation

## Overview
This project simulates a dynamic study room reservation system using multithreading in C. It features **5 study rooms** and **10 students**, where students continuously attempt to reserve and release study rooms in a loop. The simulation runs indefinitely until stopped manually.

## Features
- **Multi-threaded simulation** using `pthread`
- **Mutex and condition variables** for synchronized room reservation
- **Signal handling** for graceful termination (`Ctrl+C` support)
- **Randomized reservation attempts** by students

## Prerequisites
Ensure you have a C compiler installed, such as `gcc`, and support for POSIX threads (`pthread`).

## Compilation and Execution
### Compile the Code
```sh
gcc -o main main.c -pthread
```

### Run the Program
```sh
./main
```

### Stop the Simulation
To stop the simulation, press:
```sh
Ctrl + C
```

## Code Explanation
### 1. **Study Room Structure**
Each study room has:
- `id`: Unique identifier
- `is_reserved`: Status flag (reserved or not)
- `mutex`: To ensure safe access
- `cond`: Condition variable for managing waiting students

### 2. **Room Reservation Mechanism**
- Students randomly pick a study room.
- If the room is available, they reserve it.
- After some time, they release the room.

### 3. **Thread Functionality**
Each student runs in a separate thread and:
1. Randomly selects a room to reserve.
2. Checks if the room is available.
3. Reserves and then releases the room after some time.
4. Repeats the process until the program is terminated.

### 4. **Signal Handling**
The program listens for `SIGINT` (Ctrl+C) and gracefully stops execution by setting a `stop` flag.

## Sample Output
```
Student 3: Attempting to reserve Room 2...
Student 3: Room 2 reserved.
Student 5: Attempting to reserve Room 2...
Student 5: Room 2 is not available.
Student 3: Reservation for room 2 canceled.
```

## Possible Enhancements
- Implement **priority queues** to prioritize waiting students.
- Add **room usage time tracking**.
- Integrate **real-time room status visualization**.

## Author
**Mahmoud Hany** - Computer Engineering Student & AI Enthusiast

