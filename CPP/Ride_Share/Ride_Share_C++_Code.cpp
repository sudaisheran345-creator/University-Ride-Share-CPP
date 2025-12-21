#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <cstdio>

using namespace std;

// --- BASIC CONSTRUCT: RECORDS (STRUCTS) ---
struct Student {
    char name[50];
    char destination[50];
    char currentLocation[50];
};

// --- FUNCTION PROTOTYPES ---
void registerStudent();
void findRidePartners();
void viewAllStudents();
void clearAllData();
void showMenu();
void ensureFileExists();
int loadStudentsFromFile(Student arr[], int maxSize);
void saveAllStudentsToFile(Student arr[], int count);

const char* DB_FILE = "ride_share_data.txt";

int main() {
    // Ensure the database file exists before starting
    ensureFileExists();
    
    int choice;
    
    cout << "*** Welcome to University Ride Share System ***\n";
    cout << "Data is permanently stored in: " << DB_FILE << "\n";
    
    while (true) {
        showMenu();
        cout << "\nEnter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        cin.ignore();

        if (choice == 1) {
            registerStudent();
        } else if (choice == 2) {
            findRidePartners();
        } else if (choice == 3) {
            viewAllStudents();
        } else if (choice == 4) {
            clearAllData();
        } else if (choice == 5) {
            cout << "Exiting application. Goodbye!\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
        cout << "\n----------------------------------------\n";
    }

    return 0;
}

void showMenu() {
    cout << "\n=== UNIVERSITY RIDE SHARE SYSTEM ===\n";
    cout << "1. Register / Update My Location\n";
    cout << "2. Find Students Going to My Destination\n";
    cout << "3. View All Registered Students\n";
    cout << "4. Clear All Data\n";
    cout << "5. Exit\n";
}

// Create the file if it doesn't exist
void ensureFileExists() {
    ifstream testFile(DB_FILE);
    if (!testFile) {
        // File doesn't exist, create it
        ofstream createFile(DB_FILE);
        if (createFile) {
            createFile.close();
            cout << "[System] Database file created successfully.\n";
        } else {
            cout << "[ERROR] Cannot create database file. Check folder permissions!\n";
            cout << "[INFO] Trying alternative location...\n";
            // You could try a different path here if needed
        }
    } else {
        testFile.close();
    }
}

// Load all students from file into an array
int loadStudentsFromFile(Student arr[], int maxSize) {
    ifstream inFile(DB_FILE);
    if (!inFile) {
        return 0; // No file or can't open
    }

    int count = 0;
    char line[300];

    while (inFile.getline(line, 300) && count < maxSize) {
        if (strlen(line) == 0) continue; // Skip empty lines
        
        char d_name[50], d_dest[50], d_loc[50];
        
        int part = 0, j = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '|') {
                if(part == 0) d_name[j] = '\0';
                else if(part == 1) d_dest[j] = '\0';
                part++;
                j = 0;
            } else {
                if(part == 0) d_name[j++] = line[i];
                else if(part == 1) d_dest[j++] = line[i];
                else if(part == 2) d_loc[j++] = line[i];
            }
        }
        d_loc[j] = '\0';

        strcpy(arr[count].name, d_name);
        strcpy(arr[count].destination, d_dest);
        strcpy(arr[count].currentLocation, d_loc);
        count++;
    }

    inFile.close();
    return count;
}

// Save all students from array to file (overwrites)
void saveAllStudentsToFile(Student arr[], int count) {
    ofstream outFile(DB_FILE);
    
    if (!outFile) {
        cout << "[ERROR] Cannot write to database file!\n";
        cout << "[INFO] Make sure the program has write permissions.\n";
        return;
    }

    for (int i = 0; i < count; i++) {
        outFile << arr[i].name << "|" 
                << arr[i].destination << "|" 
                << arr[i].currentLocation << endl;
    }

    outFile.close();
}

void registerStudent() {
    cout << "\n--- REGISTER / UPDATE STATUS ---\n";
    
    // Load existing students
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);

    Student newStudent;
    
    cout << "Enter your Name: ";
    cin.getline(newStudent.name, 50);

    cout << "Enter your Destination (e.g., Saddar, Hayatabad): ";
    cin.getline(newStudent.destination, 50);

    cout << "Enter your Current Location (e.g., Library, Cafe): ";
    cin.getline(newStudent.currentLocation, 50);

    // Check if student exists (update) or add new
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (strcasecmp(students[i].name, newStudent.name) == 0) {
            // Update existing student
            strcpy(students[i].destination, newStudent.destination);
            strcpy(students[i].currentLocation, newStudent.currentLocation);
            found = true;
            cout << "\n✓ SUCCESS: Your details have been UPDATED!\n";
            break;
        }
    }

    if (!found) {
        if (studentCount >= 100) {
            cout << "[ERROR] Database full (100 students max).\n";
            return;
        }
        students[studentCount] = newStudent;
        studentCount++;
        cout << "\n✓ SUCCESS: You have been registered!\n";
    }

    // Save back to file
    saveAllStudentsToFile(students, studentCount);
    cout << "[Info] Data saved to disk permanently.\n";
}

void findRidePartners() {
    char targetDest[50];
    cout << "\n--- FIND RIDE PARTNERS ---\n";
    
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    if (studentCount == 0) {
        cout << "No data found. Be the first to register!\n";
        return;
    }
    
    cout << "Where do you want to go? ";
    cin.getline(targetDest, 50);

    bool found = false;

    cout << "\nSearching for students going to: " << targetDest << "...\n";
    cout << "-----------------------------------------------------------\n";
    cout << "Name\t\tCurrent Location\n";
    cout << "----\t\t----------------\n";

    for (int i = 0; i < studentCount; i++) {
        if (strcasecmp(students[i].destination, targetDest) == 0) {
            cout << students[i].name << "\t\t" 
                 << students[i].currentLocation << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "No students found going to '" << targetDest << "' yet.\n";
    } else {
        cout << "-----------------------------------------------------------\n";
    }
}

void viewAllStudents() {
    cout << "\n--- ALL REGISTERED STUDENTS ---\n";
    
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    if (studentCount == 0) {
        cout << "No students registered yet. Database is empty.\n";
        return;
    }

    cout << "-----------------------------------------------------------\n";
    cout << "#\tName\t\tDestination\tCurrent Location\n";
    cout << "-\t----\t\t-----------\t----------------\n";

    for (int i = 0; i < studentCount; i++) {
        cout << (i+1) << "\t" 
             << students[i].name << "\t\t" 
             << students[i].destination << "\t\t" 
             << students[i].currentLocation << "\n";
    }

    cout << "-----------------------------------------------------------\n";
    cout << "Total students: " << studentCount << "\n";
}

void clearAllData() {
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    if (studentCount == 0) {
        cout << "No data to clear.\n";
        return;
    }

    char confirm;
    cout << "\n⚠️  WARNING: This will delete ALL " << studentCount << " student records!\n";
    cout << "Are you sure? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') {
        // Clear file by opening in truncate mode
        ofstream outFile(DB_FILE, ios::trunc);
        outFile.close();
        cout << "✓ All data has been permanently deleted.\n";
    } else {
        cout << "Operation cancelled.\n";
    }
}
