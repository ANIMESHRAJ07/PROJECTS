#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BUSES 10
#define MAX_SEATS 50
#define MAX_WAITLIST 10
#define MAX_USERS 6
#define CANCELLATION_WINDOW 24 // Allowed cancellation time in hours
#define CANCELLATION_FEE 100   // Fee for late cancellations

// Define the BusSchedule structure
typedef struct BusSchedule {
    int bus_id;                // Unique Bus ID (e.g., 1001)
    char source[50];           // Source city
    char destination[50];      // Destination city
    char departure_time[10];   // Departure time
    char arrival_time[10];     // Arrival time
    struct BusSchedule *next;  // Pointer to the next schedule
} BusSchedule;

// Define the User structure
typedef struct User {
    char username[20];
    char password[20];
} User;

// Head of the linked list
BusSchedule *schedule_head = NULL;

// Function prototype for find_schedule_by_bus_id
BusSchedule *find_schedule_by_bus_id(int bus_id);

// Define the Ticket structure
typedef struct Ticket {
    char name[50];
    int bus_id;
    struct Ticket *next;
} Ticket;

// Define the Bus structure
typedef struct Bus {
    int id;                   // Unique Bus ID
    int seats_available;      // Number of available seats
    Ticket *booked_tickets;   // Linked list of booked tickets
} Bus;

// Define the BusNode structure
typedef struct BusNode {
    Bus bus;                  // Bus information
    struct BusNode *next;     // Pointer to the next node in the list
} BusNode;

// Function prototype for find_bus_by_id
BusNode *find_bus_by_id(int bus_id);

typedef struct Queue {
    char name[50];
    int bus_id;
    struct Queue *next;
} Queue;

// Define the WaitingList structure
typedef struct WaitingList {
    char name[50];
    int bus_id;
    struct WaitingList *next;
} WaitingList;

WaitingList *waiting_list_head = NULL; // Head of the waiting list
WaitingList *waiting_list_tail = NULL; // Tail of the waiting list

BusNode *bus_list = NULL;

Bus buses[MAX_BUSES];
Queue *waiting_list = NULL;
char upi_ids[MAX_USERS][20] = {"preeti@upi", "animesh@upi", "shambhavi@upi", "shreyansh@upi"};
int upi_pins[MAX_USERS] = {1234, 2345, 3456, 4567};

int waiting_list_size = 0;

// Function prototype for find_bus_by_id
BusNode *find_bus_by_id(int bus_id);

// Function prototype for find_schedule_by_bus_id
BusSchedule *find_schedule_by_bus_id(int bus_id);

// Function prototypes for waiting list operations
void add_to_waiting_list(const char *name, int bus_id);
WaitingList *remove_from_waiting_list(int bus_id);

// Function to get the current timestamp as a string
void get_current_time(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

// Function to calculate the difference in hours between two timestamps
double calculate_time_difference(const char *start_time) {
    struct tm start_tm = {0};
    time_t start, now;

    // Parse the start time using sscanf
    sscanf(start_time, "%d-%d-%d %d:%d:%d",
           &start_tm.tm_year, &start_tm.tm_mon, &start_tm.tm_mday,
           &start_tm.tm_hour, &start_tm.tm_min, &start_tm.tm_sec);

    // Adjust year and month for struct tm
    start_tm.tm_year -= 1900;
    start_tm.tm_mon -= 1;

    start = mktime(&start_tm);
    now = time(NULL);

    // Calculate the difference in hours
    return difftime(now, start) / 3600.0;
}

// Function to authenticate UPI ID and PIN
int authenticate_upi(char *upi_id, int pin) {
    for (int i = 0; i < MAX_USERS; i++) {
        if (strcmp(upi_ids[i], upi_id) == 0 && upi_pins[i] == pin) {
            return 1;
        }
    }
    return 0;
}

// Function to initialize the bus list
void initialize_bus_list() {
    for (int i = 0; i < MAX_BUSES; i++) {
        BusNode *new_node = (BusNode *)malloc(sizeof(BusNode));
        new_node->bus.id = 1001 + i; // Assign Bus IDs starting from 1001
        new_node->bus.seats_available = MAX_SEATS; // Set available seats
        new_node->bus.booked_tickets = NULL; // Initialize booked tickets
        new_node->next = NULL;

        if (!bus_list) {
            bus_list = new_node;
        } else {
            BusNode *temp = bus_list;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
}

// Function to book a ticket
void book_ticket() {
    char name[50], upi_id[20], timestamp[20];
    int bus_id, pin, num_tickets;

    printf("Enter your name: ");
    scanf("%49s", name);
    printf("Enter Bus ID (e.g., 1001): ");
    scanf("%d", &bus_id);
    printf("Enter the number of tickets to book (max 10): ");
    scanf("%d", &num_tickets);

    if (num_tickets < 1 || num_tickets > 10) {
        printf("Invalid number of tickets! You can book between 1 and 10 tickets.\n");
        return;
    }

    // Find the bus schedule by bus_id
    BusSchedule *schedule = find_schedule_by_bus_id(bus_id);
    if (!schedule) {
        printf("Invalid Bus ID! Please try again.\n");
        return;
    }

    // Find the bus node by bus_id
    BusNode *bus_node = find_bus_by_id(bus_id);
    if (!bus_node) {
        printf("Bus not found in the system!\n");
        return;
    }

    if (bus_node->bus.seats_available >= num_tickets) {
        printf("Enter UPI ID: ");
        scanf("%19s", upi_id);
        printf("Enter UPI PIN: ");
        scanf("%d", &pin);

        if (!authenticate_upi(upi_id, pin)) {
            printf("Invalid UPI credentials!\n");
            return;
        }

        for (int i = 0; i < num_tickets; i++) {
            // Create a new ticket
            Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));
            strcpy(new_ticket->name, name);
            new_ticket->bus_id = bus_id;
            new_ticket->next = bus_node->bus.booked_tickets;
            bus_node->bus.booked_tickets = new_ticket;
            bus_node->bus.seats_available--;

            // Get the current timestamp
            get_current_time(timestamp, sizeof(timestamp));

            // Log the booking details to a file
            FILE *file = fopen("Booked.txt", "a");
            if (!file) {
                printf("Error: Could not open Booked.txt for writing.\n");
                return;
            }
            fprintf(file, "Booked: %s, Bus ID: %d, Time: %s\n", name, bus_id, timestamp);
            fclose(file);
        }

        printf("%d ticket(s) booked successfully!\n", num_tickets);
    } else {
        printf("Not enough seats available. Adding to waiting list.\n");
        for (int i = 0; i < num_tickets; i++) {
            add_to_waiting_list(name, bus_id); // Add to waiting list
        }
    }
}

// Function to cancel a ticket
void cancel_ticket() {
    char name[50], upi_id[20], line[256], timestamp[20];
    int bus_id, pin, num_tickets;

    printf("Enter your name: ");
    scanf("%49s", name);
    printf("Enter Bus ID (e.g., 1001): ");
    scanf("%d", &bus_id);
    printf("Enter the number of tickets to cancel: ");
    scanf("%d", &num_tickets);

    if (num_tickets < 1) {
        printf("Invalid number of tickets to cancel!\n");
        return;
    }

    // Find the bus node by bus_id
    BusNode *bus_node = find_bus_by_id(bus_id);
    if (!bus_node) {
        printf("Bus not found in the system!\n");
        return;
    }

    printf("Enter UPI ID for refund: ");
    scanf("%19s", upi_id);
    printf("Enter UPI PIN: ");
    scanf("%d", &pin);

    if (!authenticate_upi(upi_id, pin)) {
        printf("Invalid UPI credentials!\n");
        return;
    }

    // Read the booking time from the file
    FILE *file = fopen("Booked.txt", "r");
    if (!file) {
        printf("Error: Could not open Booked.txt for reading.\n");
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        char booked_name[50];
        int booked_bus_id;
        sscanf(line, "Booked: %49[^,], Bus ID: %d, Time: %19[^\n]", booked_name, &booked_bus_id, timestamp);

        if (strcmp(booked_name, name) == 0 && booked_bus_id == bus_id) {
            found++;
            if (found == num_tickets) {
                break;
            }
        }
    }
    fclose(file);

    if (found < num_tickets) {
        printf("You do not have enough tickets to cancel!\n");
        return;
    }

    // Calculate the time difference
    double time_difference = calculate_time_difference(timestamp);

    // Check if cancellation is within the allowed time
    if (time_difference <= CANCELLATION_WINDOW) {
        printf("%d ticket(s) canceled successfully! Refund processed to UPI ID: %s\n", num_tickets, upi_id);
    } else {
        printf("%d ticket(s) canceled successfully! A cancellation fee of ₹%d has been deducted.\n", num_tickets, CANCELLATION_FEE * num_tickets);
    }

    // Free the seats
    bus_node->bus.seats_available += num_tickets;

    // Check the waiting list for this bus
    for (int i = 0; i < num_tickets; i++) {
        WaitingList *waiting_person = remove_from_waiting_list(bus_id);
        if (waiting_person) {
            printf("Seat is now available. Confirming ticket for %s from the waiting list.\n", waiting_person->name);

            // Automatically book the ticket for the waiting person
            Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));
            strcpy(new_ticket->name, waiting_person->name);
            new_ticket->bus_id = bus_id;
            new_ticket->next = bus_node->bus.booked_tickets;
            bus_node->bus.booked_tickets = new_ticket;
            bus_node->bus.seats_available--;

            // Log the booking details to a file
            file = fopen("Booked.txt", "a");
            if (!file) {
                printf("Error: Could not open Booked.txt for writing.\n");
                free(waiting_person);
                return;
            }
            get_current_time(timestamp, sizeof(timestamp));
            fprintf(file, "Booked From Waiting List : %s, Bus ID: %d, Time: %s\n", waiting_person->name, bus_id, timestamp);
            fclose(file);

            free(waiting_person); // Free the waiting list node
        }
    }

    // Log the cancellation to the file
    file = fopen("Cancelled.txt", "a");
    if (!file) {
        printf("Error: Could not open Cancelled.txt for writing.\n");
        return;
    }
    fprintf(file, "Cancelled: %s, Bus ID: %d, Tickets: %d, Time: %s\n", name, bus_id, num_tickets, timestamp);
    fclose(file);
}

// Manually stored users
User users[4] = {
    {"Preeti", "preeti123"},
    {"Animesh", "animesh123"},
    {"Shambhavi", "shambhavi123"},
    {"Shreyansh", "shreyansh123"}
};

// Function for user login
int user_login() {
    char entered_username[20], entered_password[20];
    int login_success = 0;

    printf("\n--- User Login ---\n");
    printf("Enter Username: ");
    scanf("%19s", entered_username);
    printf("Enter Password: ");
    scanf("%19s", entered_password);

    // Validate credentials
    for (int i = 0; i < 2; i++) {
        if (strcmp(users[i].username, entered_username) == 0 &&
            strcmp(users[i].password, entered_password) == 0) {
            login_success = 1;
            break;
        }
    }

    if (login_success) {
        printf("Login successful! Welcome, %s.\n", entered_username);
        return 1; // Login successful
    } else {
        printf("Invalid username or password. Please try again.\n");
        return 0; // Login failed
    }
}

// Function to add a bus schedule to the linked list
void add_bus_schedule(int bus_id, const char *source, const char *destination, const char *departure_time, const char *arrival_time) {
    // Create a new node
    BusSchedule *new_schedule = (BusSchedule *)malloc(sizeof(BusSchedule));
    new_schedule->bus_id = bus_id;
    strcpy(new_schedule->source, source);
    strcpy(new_schedule->destination, destination);
    strcpy(new_schedule->departure_time, departure_time);
    strcpy(new_schedule->arrival_time, arrival_time);
    new_schedule->next = NULL;

    // Add the new node to the end of the linked list
    if (!schedule_head) {
        schedule_head = new_schedule;
    } else {
        BusSchedule *temp = schedule_head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_schedule;
    }
}

// Function to display the bus schedule
void display_bus_schedule() {
    if (!schedule_head) {
        printf("No bus schedules available.\n");
        return;
    }

    printf("\nBus Schedule:\n");
    printf("-------------------------------------------------------------\n");
    printf("| Bus ID |   Source    |  Destination  | Departure | Arrival |\n");
    printf("-------------------------------------------------------------\n");

    BusSchedule *temp = schedule_head;
    while (temp) {
        printf("|   %2d   | %-11s | %-13s | %-9s | %-7s |\n",
               temp->bus_id, temp->source, temp->destination, temp->departure_time, temp->arrival_time);
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
}

// Function to initialize the bus schedule
void initialize_bus_schedule() {
    add_bus_schedule(1001, "Dehradun", "Delhi", "06:00 AM", "10:00 AM");
    add_bus_schedule(1002, "Delhi", "Jaipur", "08:00 AM", "12:00 PM");
    add_bus_schedule(1003, "Jaipur", "Mumbai", "10:00 AM", "02:00 PM");
    add_bus_schedule(1004, "Mumbai", "Chennai", "12:00 PM", "04:00 PM");
    add_bus_schedule(1005, "Chennai", "Kolkata", "02:00 PM", "06:00 PM");
    add_bus_schedule(1006, "Kolkata", "Bangalore", "04:00 PM", "08:00 PM");
    add_bus_schedule(1007, "Bangalore", "Hyderabad", "06:00 PM", "10:00 PM");
    add_bus_schedule(1008, "Hyderabad", "Pune", "08:00 PM", "12:00 AM");
    add_bus_schedule(1009, "Pune", "Lucknow", "10:00 PM", "02:00 AM");
    add_bus_schedule(1010, "Lucknow", "Dehradun", "11:00 PM", "03:00 AM");
}

// Function to find a bus schedule by bus ID
BusSchedule *find_schedule_by_bus_id(int bus_id) {
    BusSchedule *temp = schedule_head;
    while (temp != NULL) { // Traverse the linked list
        if (temp->bus_id == bus_id) {
            return temp;  // Return the matching schedule
        }
        temp = temp->next; // Move to the next node
    }
    return NULL;  // Return NULL if no match is found
}

// Function to find a bus by its ID
BusNode *find_bus_by_id(int bus_id) {
    BusNode *temp = bus_list; // Start from the head of the bus list
    while (temp != NULL) {    // Traverse the linked list
        if (temp->bus.id == bus_id) {
            return temp;      // Return the matching BusNode
        }
        temp = temp->next;    // Move to the next node
    }
    return NULL;              // Return NULL if no match is found
}

// Function to display seat availability for all buses
void display_seat_availability() {
    if (!bus_list) {
        printf("No buses available in the system.\n");
        return;
    }

    printf("\nSeat Availability:\n");
    printf("-------------------------------------------------------------\n");
    printf("| Bus ID |   Source    |  Destination  | Seats Available |\n");
    printf("-------------------------------------------------------------\n");

    BusNode *temp = bus_list;
    while (temp) {
        // Find the corresponding schedule for the bus
        BusSchedule *schedule = find_schedule_by_bus_id(temp->bus.id);
        if (schedule) {
            printf("|   %4d  | %-11s | %-13s |       %2d       |\n",
                   temp->bus.id, schedule->source, schedule->destination, temp->bus.seats_available);
        }
        temp = temp->next;
    }
    printf("-------------------------------------------------------------\n");
}

// Function to add a person to the waiting list
void add_to_waiting_list(const char *name, int bus_id) {
    WaitingList *new_node = (WaitingList *)malloc(sizeof(WaitingList));
    strcpy(new_node->name, name);
    new_node->bus_id = bus_id;
    new_node->next = NULL;

    if (!waiting_list_head) {
        waiting_list_head = new_node;
        waiting_list_tail = new_node;
    } else {
        waiting_list_tail->next = new_node;
        waiting_list_tail = new_node;
    }

    printf("Added to waiting list for Bus ID: %d\n", bus_id);
}

// Function to remove the first person from the waiting list for a specific bus
WaitingList *remove_from_waiting_list(int bus_id) {
    WaitingList *temp = waiting_list_head;
    WaitingList *prev = NULL;

    while (temp) {
        if (temp->bus_id == bus_id) {
            if (prev) {
                prev->next = temp->next;
            } else {
                waiting_list_head = temp->next;
            }

            if (temp == waiting_list_tail) {
                waiting_list_tail = prev;
            }

            return temp; // Return the removed node
        }
        prev = temp;
        temp = temp->next;
    }

    return NULL; // No match found
}

// Menu function
void menu() {
    int choice;

    do {
        // Clear the screen (system-specific)
        system("cls"); // Use "clear" for Linux/Mac

        // Display the real-time clock
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char current_time[20];
        strftime(current_time, sizeof(current_time), "%Y-%m-%d %H:%M:%S", t);

        // Display the menu header with the clock
        printf("=========================================\n");
        printf(" Bus Reservation System       %s\n", current_time);
        printf("=========================================\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Ticket\n");
        printf("3. Display Bus Schedule\n");
        printf("4. Show Seat Availability\n");
        printf("5. Exit\n");
        printf("=========================================\n");
        printf("Enter your choice (1-5): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\n--- Book Ticket ---\n");
                book_ticket();
                break;
            case 2:
                printf("\n--- Cancel Ticket ---\n");
                cancel_ticket();
                break;
            case 3:
                printf("\n--- Bus Schedule ---\n");
                display_bus_schedule();
                break;
            case 4:
                printf("\n--- Seat Availability ---\n");
                display_seat_availability();
                break;
            case 5:
                printf("\nThank you for using the Bus Reservation System. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please enter a number between 1 and 5.\n");
        }

        // Pause before refreshing the menu
        if (choice != 5) {
            printf("\nPress Enter to return to the menu...");
            getchar(); // Consume the newline character
            getchar(); // Wait for user input
        }
    } while (choice != 5);
}

// Main function
int main() {
    // User login
    if (!user_login()) {
        printf("Exiting the system due to failed login.\n");
        return 0; // Exit if login fails
    }

    // Initialize bus schedules and bus list
    initialize_bus_schedule();
    initialize_bus_list();

    // Display the menu
    menu();
    return 0;
}