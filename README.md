# ClientServer-and-Scheduling-OS-Project

A practical implementation of CPU scheduling algorithms and socket programming for Operating Systems, developed by Muhammad Abdullah Aamir.

## Overview

This project focuses on two key areas of Operating Systems:
- **CPU Scheduling Algorithms**: Implementation and analysis of First-Come-First-Served (FCFS), Shortest Job First (SJF), Priority Scheduling, Round Robin (RR), and Priority with Round Robin.
- **Socket Programming**: Establishing communication using sockets in local and distributed systems.

The objective is to deepen the understanding of process scheduling and inter-process communication through hands-on implementation.

## Features

- Implementation of multiple CPU scheduling algorithms with metrics like Completion Time, Waiting Time, Turnaround Time, and Average Waiting/Turnaround Times.
- Client-server communication using sockets for local and distributed systems.
- Robust error handling for socket operations and file I/O.
- Configurable time quantum for Round Robin and Priority with Round Robin scheduling.

## Technologies Used

- **Language**: C
- **Libraries**: `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<sys/socket.h>`, `<arpa/inet.h>`, `<unistd.h>`, `<netdb.h>`, `<limits.h>`

- **Environment**: Unix-like systems (e.g., Linux) with GCC compiler

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/abdullahaamir13/ClientServer-and-Scheduling-OS-Project.git
   ```

2. **Navigate to the Project Directory**:
   ```bash
   cd ClientServer-and-Scheduling-OS-Project
   ```

3. **Compile the Code**:
   - For CPU Scheduling:
     ```bash
     gcc -o scheduling scheduling.c
     ```
   - For Server:
     ```bash
     gcc -o server server.c
     ```
   - For Client:
     ```bash
     gcc -o client client.c
     ```

4. **Prepare the Task File**:
   - Create a file named `schedule.txt` in the project directory with the format:
     ```
     TaskName, Priority, CPUBurst
     T1, 2, 5
     T2, 1, 3
     ```
   - Example tasks are included for testing.

## Usage

### CPU Scheduling
1. Run the compiled scheduling program:
   ```bash
   ./scheduling
   ```
2. Select an algorithm (1-5) from the menu:
   - 1: FCFS
   - 2: SJF
   - 3: Priority Scheduling
   - 4: Round Robin (enter time quantum)
   - 5: Priority with Round Robin (enter time quantum)
   - 6: Exit
3. View the output metrics (Completion Time, Waiting Time, Turnaround Time, etc.).

### Socket Programming
1. Start the server:
   ```bash
   ./server
   ```
   - The server listens on port 9090.
2. Run the client with hostname and port as arguments:
   ```bash
   ./client localhost 9090
   ```
3. Communicate with the server:
   - Type messages to send to the server.
   - Type "Bye" to exit.
   - The server echoes back the messages until disconnected.

## Project Structure

```
ClientServer-and-Scheduling-OS-Project/
├── scheduling.c
├── server.c
├── client.c
├── schedule.txt
├── README.md
```

## Results and Screenshots

For detailed results and screenshots, refer to the project report: [22P_9235_Abdullah_Aamir_Project.pdf](22P_9235_Abdullah_Aamir_Project.pdf).  
Key outputs include:
- Sample outputs for FCFS, SJF, Priority Scheduling, Round Robin, and Priority with Round Robin.
- Gantt Chart for scheduling execution.
- Server and Client communication screenshots.
- Error handling examples.

## Conclusion

This project provided hands-on experience with CPU scheduling algorithms and socket programming, demonstrating efficient process management and inter-process communication in Operating Systems.

## Author

**Muhammad Abdullah Aamir**  
- FAST University Peshawar  
- Email: [abdullahaamir977@gmail.com](mailto:abdullahaamir977@gmail.com)  
- LinkedIn: [linkedin.com/in/abdullah-aamir/](https://linkedin.com/in/abdullah-aamir/)