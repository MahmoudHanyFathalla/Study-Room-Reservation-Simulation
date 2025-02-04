#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep function
#include <time.h>   // for srand function
#include <signal.h> // for signal handling

#define NUM_ROOMS 5
#define NUM_STUDENTS 10
/*
dy simulation 3ndk 5 rooms w 10 students by7awlo y7gzo mnhom fe Loop,
To run this simulation : gcc -o main main.c -pthread
To stop the running Loop simulation click on {Ctrl C}
*/ 
volatile sig_atomic_t stop = 0;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int signum) {
    stop = 1;
}

// Structure to represent a study room
typedef struct {
    int id;
    int is_reserved;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} StudyRoom;

StudyRoom rooms[NUM_ROOMS];

// Function to initialize study rooms
void initialize_rooms() {
    for (int i = 0; i < NUM_ROOMS; i++) {
        rooms[i].id = i + 1;
        rooms[i].is_reserved = 0;
        pthread_mutex_init(&rooms[i].mutex, NULL);
        pthread_cond_init(&rooms[i].cond, NULL);
    }
}

// Function to check room availability
int check_availability(int room_id) {
    pthread_mutex_lock(&rooms[room_id - 1].mutex);
    int available = !rooms[room_id - 1].is_reserved;
    pthread_mutex_unlock(&rooms[room_id - 1].mutex);
    return available;
}

// Function to reserve a room
void reserve_room(int room_id, int student_id) {
    pthread_mutex_lock(&rooms[room_id - 1].mutex);
    while (rooms[room_id - 1].is_reserved) {
        pthread_cond_wait(&rooms[room_id - 1].cond, &rooms[room_id - 1].mutex);
    }
    rooms[room_id - 1].is_reserved = 1;
    printf("Student %d: Room %d reserved.\n", student_id, room_id);
    pthread_mutex_unlock(&rooms[room_id - 1].mutex);
}

// Function to cancel a reservation
void cancel_reservation(int room_id, int student_id) {
    pthread_mutex_lock(&rooms[room_id - 1].mutex);
    rooms[room_id - 1].is_reserved = 0;
    printf("Student %d: Reservation for room %d canceled.\n", student_id, room_id);
    pthread_cond_signal(&rooms[room_id - 1].cond);
    pthread_mutex_unlock(&rooms[room_id - 1].mutex);
}

// Thread function to simulate student reservation requests
void *student_thread(void *arg) {
    int student_id = *(int *)arg;
    srand(time(NULL) + student_id);

    int reserved_room = 0;

    while (!stop) {
        int room_id = (rand() % NUM_ROOMS) + 1;

        printf("Student %d: Attempting to reserve Room %d...\n", student_id, room_id);

        if (check_availability(room_id)) {
            reserve_room(room_id, student_id);
            reserved_room = room_id; // Record the reserved room
        } else {
            printf("Student %d: Room %d is not available.\n", student_id, room_id);
        }

        
        sleep(1);

        // Cancel reservation only if a room was reserved
        if (reserved_room != 0) {
            cancel_reservation(reserved_room, student_id);
            reserved_room = 0; // Reset reserved room
        }

        // Simulate some more work
        sleep(1);
    }

    return NULL;
}

int main() {
    initialize_rooms();

    // Set up signal handler for SIGINT (Ctrl+C)
    signal(SIGINT, handle_sigint);

    pthread_t students[NUM_STUDENTS];
    int student_ids[NUM_STUDENTS];

    // Create threads for each student
    for (int i = 0; i < NUM_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_STUDENTS; i++) {
        pthread_join(students[i], NULL);
    }

    return 0;
}
