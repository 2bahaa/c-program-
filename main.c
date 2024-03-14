#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a room
struct Room {
    int room_number;
    int is_empty;
    int patient_id;
};

// Structure to represent a user
struct User {
    char username[50];
    char password[50];
    char role[50];  // Added role information
};

// Structure to represent a patient
struct Patient {
    int patient_id;
    char name[50];
    char status[50];
};

// Function to read an integer from the user with input validation
int readIntInRange(int min, int max) {
    int value;
    char buffer[100];
    int conversionStatus;

    // Loop until a valid integer in the specified range is entered
    while (1) {
        // Read a line from the user
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error reading input. Exiting...\n");
            exit(EXIT_FAILURE);
        }

        // Check if the input is a valid integer
        conversionStatus = sscanf(buffer, "%d", &value);

        // Check for invalid input
        if (conversionStatus != 1 || value < min || value > max) {
            printf(" Please enter a valid number between %d and %d.\n", min, max);
        } else {
            break; // Exit the loop if a valid integer is entered
        }
    }

    return value;
}

// Function to login
int login(struct User users[], int num_users, char username[], char password[]) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return i; // Return the index of the user
        }
    }
    return -1; // Return -1 if login fails
}

// Function to add a new patient
void addNewPatient(struct Patient patients[], int *num_patients) {
    if (*num_patients >= 10) {
        printf("Cannot add more patients. Maximum limit reached.\n");
        return;
    }

    printf("Enter patient name: ");
   gets(patients[*num_patients].name);

    printf("Enter patient status: ");
    gets(patients[*num_patients].status);

    patients[*num_patients].patient_id = 100+ *num_patients + 1;

    printf("New patient added successfully. Patient ID: %d\n", patients[*num_patients].patient_id);

    (*num_patients)++;
    // Display information of all patients
    printf("\nInformation of all patients:\n");
    for (int i = 0; i < *num_patients; i++) {
        printf("Patient ID: %d, Name: %s, Status: %s\n", patients[i].patient_id, patients[i].name, patients[i].status);
    }
}

// Function to remove current reservation (for nurses)
void removeReservation(struct Room rooms[], int num_rooms) {
    int room_number;

    // Display current room status
    printf("Current Room Status:\n");
    for (int i = 0; i < num_rooms; i++) {
        printf("Room %d: %s\n", rooms[i].room_number, (rooms[i].is_empty ? "Empty" : "Occupied"));
    }

    // Ask the nurse to choose a room to remove
    printf("Enter the room number to remove reservation: ");
    room_number = readIntInRange(1, num_rooms);

    int room_index = -1;
    for (int i = 0; i < num_rooms; i++) {
        if (rooms[i].room_number == room_number) {
            room_index = i;
            break;
        }
    }

    if (room_index != -1 && !rooms[room_index].is_empty) {
        // Update the chosen room
        rooms[room_index].is_empty = 1;
        rooms[room_index].patient_id = 0;
        printf("Reservation removed for room %d\n", room_number);
    } else {
        printf("Invalid room number or the room is already empty. No reservation removed.\n");
    }
}

// Function to update patient's data (for doctors)
void updatePatientData(struct Patient patients[], int num_patients) {
    int patient_id;
    printf("Enter patient ID: ");
    scanf("%d", &patient_id);

    int patient_index = -1;
    for (int i = 0; i < num_patients; i++) {
        if (patients[i].patient_id == patient_id) {
            patient_index = i;
            break;
        }
    }

    if (patient_index != -1) {
        printf("Enter new status for patient %d: ", patient_id);
        fflush(stdin);
       gets(patients[patient_index].status);
               fflush(stdin);

        printf("Patient data updated successfully\n");
        // Display information of all patients
    printf("\nInformation of all patients:\n");
    for (int i = 0; i < num_patients; i++) {
        printf("Patient ID: %d, Name: %s, Status: %s\n", patients[i].patient_id, patients[i].name, patients[i].status);
    }
    } else {
        printf("Patient with ID %d not found\n", patient_id);
    }
}


// Function to check empty rooms
void checkEmptyRooms(struct Room rooms[], int num_rooms) {
    printf("Empty Rooms:\n");
    for (int i = 0; i < num_rooms; i++) {
        if (rooms[i].is_empty) {
            printf("Room %d\n", rooms[i].room_number);
        }
    }
}

int main() {
    // Initialize rooms, users, and patients
    struct Room rooms[10] = {{1, 1, 0}, {2, 1, 0}, {3, 0, 101}, {4, 1, 0}, {5, 0, 102},
                             {6, 0, 103}, {7, 1, 0}, {8, 1, 0}, {9, 0, 104}, {10, 1, 0}};
  struct Patient patients[4] = {{101, "tolba", "unstable"}, {102, "bahaa", "Stable"},
                                  {103, "7eeta", "mentally Injured"}, {104, "bahram", "insane"}};

    struct User users[3] = {
        {"main", "m123", "main"},    // main role
        {"doctor", "d123", "doctor"}, // Doctor role
        {"nurse", "n123", "nurse"}     // Nurse role
    };

int num_patients = sizeof(patients) / sizeof(patients[0]);
    int num_users = sizeof(users) / sizeof(users[0]);
    int num_rooms = sizeof(rooms) / sizeof(rooms[0]);

    // Login variables
    char username[50];
    char password[50];
    int user_index;

    printf("-----------------------Welcome to El B&B International Hospital----------------------\n");

    do {
        printf("Please enter your username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        user_index = login(users, num_users, username, password);

        if (user_index == -1) {
            printf("Invalid username or password. Please try again.\n");
        }

    } while (user_index == -1);

    // Perform operations based on user privileges
    int choice;

    do {
        printf("\Main Menu:\n");
        printf("1. Add New Patient\n");
        printf("2. Remove Current Reservation\n");
        printf("3. Check Empty Rooms\n");
        printf("4. Update Patient's Data\n");
        printf("5. Logout\n");
                printf("6. Exit\n");

        printf("Enter your choice: ");
        choice = readIntInRange(1, 6); // Restrict the input to be within the range of 1 to 5

        switch (choice) {
            case 1:
                if (strcmp(users[user_index].role, "doctor") == 0|| strcmp(users[user_index].role,"nurse")==0) {
                    addNewPatient(patients, &num_patients);
                } else {
                    printf("You do not have permission to add a new patient.\n");
                }
                break;
            case 2:
                if (strcmp(users[user_index].role, "main") == 0) {
                    removeReservation(rooms, num_rooms);
                } else {
                    printf("You do not have permission to remove reservations.\n");
                }
                break;
            case 3:
                if (strcmp(users[user_index].role, "main") == 0)
                checkEmptyRooms(rooms, num_rooms);
                else {
                    printf("You do not have permission to remove reservations.\n");
                }
                break;
            case 4:
                if (strcmp(users[user_index].role, "doctor") == 0 || strcmp(users[user_index].role,"nurse")==0) {
                    updatePatientData(patients, num_patients);
                } else {
                    printf("You do not have permission to update patient data.\n");
                }
                break;
            case 5:
                printf("Logged out successfully\n");
                do {
        printf("Please enter your username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        user_index = login(users, num_users, username, password);

        if (user_index == -1) {
            printf("Invalid username or password. Please try again.\n");
        }

    } while (user_index == -1);
                break;
            case 6:
                printf("Exit Whole Program");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 6);

    // Free dynamically allocated memory for patients
    //free(patients);

    return 0;
}
