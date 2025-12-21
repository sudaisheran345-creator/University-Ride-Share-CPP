/*
===============================================================================
UNIVERSITY RIDE SHARE SYSTEM
===============================================================================
Purpose: Help students find ride-sharing partners going to the same destination
Features: Register students, search by destination, view all students, clear data
Data Storage: Text file (ride_share_data.txt) for persistent storage
===============================================================================
*/

#include <iostream>  // For input/output operations (cin, cout)
#include <fstream>   // For file operations (reading/writing files)
#include <cstring>   // For C-style string functions (strcpy, strcmp, strlen)
#include <string>    // For modern C++ string class (not heavily used here)
#include <cstdio>    // For standard C input/output functions

using namespace std; // Allows us to write 'cout' instead of 'std::cout'

/*
===============================================================================
STRUCT DEFINITION: Student Record
===============================================================================
This is like a template/blueprint for storing student information.
Think of it as a form with 3 fields that we'll fill out for each student.
*/
struct Student {
    char name[50];            // Student's name (max 49 characters + null terminator)
    char destination[50];     // Where they want to go (e.g., "Saddar", "Hayatabad")
    char currentLocation[50]; // Where they are now (e.g., "Library", "Cafe")
};

/*
===============================================================================
FUNCTION PROTOTYPES (Forward Declarations)
===============================================================================
These tell the compiler: "These functions exist below, trust me!"
It's like a table of contents for your program.
*/
void registerStudent();                              // Register new student or update existing
void findRidePartners();                             // Search for students going to same destination
void viewAllStudents();                              // Display all registered students
void clearAllData();                                 // Delete all student records
void showMenu();                                     // Display menu options
void ensureFileExists();                             // Create database file if it doesn't exist
int loadStudentsFromFile(Student arr[], int maxSize); // Load students from file into array
void saveAllStudentsToFile(Student arr[], int count); // Save students from array to file

// Global constant: The name of our database file
// Using const means this value can never be changed
const char* DB_FILE = "ride_share_data.txt";

/*
===============================================================================
MAIN FUNCTION - Program Entry Point
===============================================================================
This is where the program ALWAYS starts executing.
Every C++ program must have exactly one main() function.
*/
int main() {
    // Step 1: Make sure our database file exists before we start
    ensureFileExists();
    
    // Variable to store user's menu choice (1-5)
    int choice;
    
    // Welcome message
    cout << "*** Welcome to University Ride Share System ***\n";
    cout << "Data is permanently stored in: " << DB_FILE << "\n";
    
    /*
    Main Program Loop - Keeps running until user chooses to exit
    while(true) creates an infinite loop that only stops when we use 'break'
    */
    while (true) {
        // Display the menu options to user
        showMenu();
        
        // Ask user to enter their choice
        cout << "\nEnter your choice: ";
        
        /*
        Input Validation: Check if user entered a valid number
        !(cin >> choice) returns true if input FAILS (e.g., user types letters)
        */
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();              // Clear the error flag on cin
            cin.ignore(1000, '\n');   // Discard invalid input up to 1000 chars or newline
            continue;                 // Skip rest of loop, go back to start (show menu again)
        }
        
        /*
        Clear the newline character left in buffer after reading the number
        When user types "3" and presses Enter, cin >> choice reads "3" but leaves "\n"
        cin.ignore() removes that leftover newline so it doesn't interfere with getline() later
        */
        cin.ignore();

        /*
        Menu Selection Logic
        Based on what number user entered, call the appropriate function
        */
        if (choice == 1) {
            registerStudent();    // Option 1: Register or update student info
        } 
        else if (choice == 2) {
            findRidePartners();   // Option 2: Search for ride partners
        } 
        else if (choice == 3) {
            viewAllStudents();    // Option 3: View all registered students
        } 
        else if (choice == 4) {
            clearAllData();       // Option 4: Delete all data
        } 
        else if (choice == 5) {
            cout << "Exiting application. Goodbye!\n";
            break;                // Exit the while loop (ends program)
        } 
        else {
            cout << "Invalid choice. Try again.\n";
        }
        
        // Print a separator line for better readability
        cout << "\n----------------------------------------\n";
    }

    // Return 0 to operating system indicating successful program completion
    return 0;
}

/*
===============================================================================
FUNCTION: showMenu()
===============================================================================
Purpose: Display the menu options to the user
Parameters: None
Returns: void (nothing)
*/
void showMenu() {
    cout << "\n=== UNIVERSITY RIDE SHARE SYSTEM ===\n";
    cout << "1. Register / Update My Location\n";
    cout << "2. Find Students Going to My Destination\n";
    cout << "3. View All Registered Students\n";
    cout << "4. Clear All Data\n";
    cout << "5. Exit\n";
}

/*
===============================================================================
FUNCTION: ensureFileExists()
===============================================================================
Purpose: Create the database file if it doesn't exist
Why needed: Program needs this file to store student data permanently
Parameters: None
Returns: void (nothing)
*/
void ensureFileExists() {
    // Try to open file for reading (input file stream)
    ifstream testFile(DB_FILE);
    
    // If file doesn't exist or can't be opened
    if (!testFile) {
        // Create the file by opening it in output mode
        // Opening with ofstream automatically creates the file if it doesn't exist
        ofstream createFile(DB_FILE);
        
        if (createFile) {
            createFile.close(); // Close the file (good practice)
            cout << "[System] Database file created successfully.\n";
        } 
        else {
            // Error: Can't create file (maybe no write permissions)
            cout << "[ERROR] Cannot create database file. Check folder permissions!\n";
            cout << "[INFO] Trying alternative location...\n";
            // Could try a different path here (e.g., user's home directory)
        }
    } 
    else {
        // File already exists, just close it
        testFile.close();
    }
}

/*
===============================================================================
FUNCTION: loadStudentsFromFile()
===============================================================================
Purpose: Read all student records from the database file into an array
Parameters: 
  - arr[]: Array to store the students
  - maxSize: Maximum number of students the array can hold
Returns: int - Number of students actually loaded
File Format: Each line is: Name|Destination|CurrentLocation
Example: John Smith|Saddar|Library
*/
int loadStudentsFromFile(Student arr[], int maxSize) {
    // Open file for reading
    ifstream inFile(DB_FILE);
    
    // If file doesn't exist or can't be opened, return 0 (no students loaded)
    if (!inFile) {
        return 0;
    }

    int count = 0;        // Counter for number of students loaded
    char line[300];       // Buffer to store each line (max 300 characters)

    /*
    Read file line by line until:
    1. End of file is reached, OR
    2. We've loaded maxSize students (array is full)
    */
    while (inFile.getline(line, 300) && count < maxSize) {
        // Skip empty lines
        if (strlen(line) == 0) continue;
        
        // Temporary arrays to store the parsed data
        char d_name[50], d_dest[50], d_loc[50];
        
        /*
        Parse the line character by character
        part: Which field we're currently reading (0=name, 1=destination, 2=location)
        j: Index for writing into current field array
        */
        int part = 0, j = 0;
        
        // Loop through each character in the line
        for (int i = 0; line[i] != '\0'; i++) {
            // If we encounter the delimiter '|', move to next field
            if (line[i] == '|') {
                // Null-terminate the current field
                if(part == 0) d_name[j] = '\0';
                else if(part == 1) d_dest[j] = '\0';
                
                part++; // Move to next field
                j = 0;  // Reset index for next field
            } 
            else {
                // Add character to appropriate field based on 'part'
                if(part == 0) d_name[j++] = line[i];      // Building name
                else if(part == 1) d_dest[j++] = line[i]; // Building destination
                else if(part == 2) d_loc[j++] = line[i];  // Building location
            }
        }
        // Null-terminate the last field (location)
        d_loc[j] = '\0';

        /*
        Copy the parsed data into the array
        strcpy() = string copy function
        Copies from source (d_name) to destination (arr[count].name)
        */
        strcpy(arr[count].name, d_name);
        strcpy(arr[count].destination, d_dest);
        strcpy(arr[count].currentLocation, d_loc);
        
        count++; // Increment student count
    }

    inFile.close(); // Close the file (good practice)
    return count;   // Return number of students loaded
}

/*
===============================================================================
FUNCTION: saveAllStudentsToFile()
===============================================================================
Purpose: Write all students from the array to the database file
Note: This OVERWRITES the entire file with current data
Parameters:
  - arr[]: Array containing all students
  - count: Number of students in the array
Returns: void (nothing)
*/
void saveAllStudentsToFile(Student arr[], int count) {
    // Open file for writing (output mode)
    // This automatically OVERWRITES the existing file
    ofstream outFile(DB_FILE);
    
    // Check if file opened successfully
    if (!outFile) {
        cout << "[ERROR] Cannot write to database file!\n";
        cout << "[INFO] Make sure the program has write permissions.\n";
        return; // Exit function early
    }

    /*
    Write each student to file in format: Name|Destination|CurrentLocation
    Example output: John Smith|Saddar|Library
    */
    for (int i = 0; i < count; i++) {
        outFile << arr[i].name << "|"              // Write name + delimiter
                << arr[i].destination << "|"        // Write destination + delimiter
                << arr[i].currentLocation << endl;  // Write location + newline
    }

    outFile.close(); // Close the file
}

/*
===============================================================================
FUNCTION: registerStudent()
===============================================================================
Purpose: Register a new student OR update existing student's information
Logic: If student name already exists, UPDATE their info; otherwise ADD new
Parameters: None
Returns: void (nothing)
*/
void registerStudent() {
    cout << "\n--- REGISTER / UPDATE STATUS ---\n";
    
    // Step 1: Load all existing students from file
    Student students[100];  // Array to hold up to 100 students
    int studentCount = loadStudentsFromFile(students, 100);

    // Step 2: Create a new Student structure for input
    Student newStudent;
    
    // Step 3: Get student information from user
    cout << "Enter your Name: ";
    cin.getline(newStudent.name, 50);  // Read up to 49 characters (50th is '\0')

    cout << "Enter your Destination (e.g., Saddar, Hayatabad): ";
    cin.getline(newStudent.destination, 50);

    cout << "Enter your Current Location (e.g., Library, Cafe): ";
    cin.getline(newStudent.currentLocation, 50);

    /*
    Step 4: Check if student already exists (case-insensitive search)
    strcasecmp() compares two strings ignoring case differences
    Returns 0 if strings are equal (ignoring case)
    */
    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        // If we find a student with the same name (case-insensitive)
        if (strcasecmp(students[i].name, newStudent.name) == 0) {
            // UPDATE existing student's information
            strcpy(students[i].destination, newStudent.destination);
            strcpy(students[i].currentLocation, newStudent.currentLocation);
            found = true;
            cout << "\n✓ SUCCESS: Your details have been UPDATED!\n";
            break; // Stop searching, we found the student
        }
    }

    // Step 5: If student not found, add as NEW student
    if (!found) {
        // Check if array is full (100 students max)
        if (studentCount >= 100) {
            cout << "[ERROR] Database full (100 students max).\n";
            return; // Exit function
        }
        
        // Add new student to the array
        students[studentCount] = newStudent;
        studentCount++; // Increment count
        cout << "\n✓ SUCCESS: You have been registered!\n";
    }

    // Step 6: Save all students back to file (permanent storage)
    saveAllStudentsToFile(students, studentCount);
    cout << "[Info] Data saved to disk permanently.\n";
}

/*
===============================================================================
FUNCTION: findRidePartners()
===============================================================================
Purpose: Search for students going to a specific destination
Shows: Student names and their current locations
Parameters: None
Returns: void (nothing)
*/
void findRidePartners() {
    char targetDest[50]; // To store destination user is searching for
    cout << "\n--- FIND RIDE PARTNERS ---\n";
    
    // Step 1: Load all students from file
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    // Step 2: Check if there's any data
    if (studentCount == 0) {
        cout << "No data found. Be the first to register!\n";
        return; // Exit function
    }
    
    // Step 3: Ask user where they want to go
    cout << "Where do you want to go? ";
    cin.getline(targetDest, 50);

    bool found = false; // Flag to track if we find any matches

    // Step 4: Display table header
    cout << "\nSearching for students going to: " << targetDest << "...\n";
    cout << "-----------------------------------------------------------\n";
    cout << "Name\t\tCurrent Location\n";
    cout << "----\t\t----------------\n";

    /*
    Step 5: Search through all students
    strcasecmp() does case-insensitive comparison
    Example: "Saddar" matches "saddar", "SADDAR", "SaDdAr"
    */
    for (int i = 0; i < studentCount; i++) {
        if (strcasecmp(students[i].destination, targetDest) == 0) {
            // Found a match! Display the student info
            cout << students[i].name << "\t\t" 
                 << students[i].currentLocation << "\n";
            found = true;
        }
    }

    // Step 6: Display appropriate message based on results
    if (!found) {
        cout << "No students found going to '" << targetDest << "' yet.\n";
    } else {
        cout << "-----------------------------------------------------------\n";
    }
}

/*
===============================================================================
FUNCTION: viewAllStudents()
===============================================================================
Purpose: Display all registered students in a formatted table
Shows: Serial number, Name, Destination, Current Location
Parameters: None
Returns: void (nothing)
*/
void viewAllStudents() {
    cout << "\n--- ALL REGISTERED STUDENTS ---\n";
    
    // Step 1: Load all students from file
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    // Step 2: Check if database is empty
    if (studentCount == 0) {
        cout << "No students registered yet. Database is empty.\n";
        return; // Exit function
    }

    // Step 3: Display table header
    cout << "-----------------------------------------------------------\n";
    cout << "#\tName\t\tDestination\tCurrent Location\n";
    cout << "-\t----\t\t-----------\t----------------\n";

    // Step 4: Loop through and display each student
    for (int i = 0; i < studentCount; i++) {
        cout << (i+1) << "\t"                           // Serial number (starting from 1)
             << students[i].name << "\t\t" 
             << students[i].destination << "\t\t" 
             << students[i].currentLocation << "\n";
    }

    // Step 5: Display footer with total count
    cout << "-----------------------------------------------------------\n";
    cout << "Total students: " << studentCount << "\n";
}

/*
===============================================================================
FUNCTION: clearAllData()
===============================================================================
Purpose: Delete ALL student records from the database
Warning: This is PERMANENT and cannot be undone!
Parameters: None
Returns: void (nothing)
*/
void clearAllData() {
    // Step 1: Load students to check if there's any data
    Student students[100];
    int studentCount = loadStudentsFromFile(students, 100);
    
    // Step 2: If database is already empty
    if (studentCount == 0) {
        cout << "No data to clear.\n";
        return; // Exit function
    }

    // Step 3: Ask for confirmation (safety feature)
    char confirm;
    cout << "\n⚠️  WARNING: This will delete ALL " << studentCount << " student records!\n";
    cout << "Are you sure? (y/n): ";
    cin >> confirm;
    cin.ignore(); // Clear newline from buffer

    // Step 4: Only proceed if user confirms with 'y' or 'Y'
    if (confirm == 'y' || confirm == 'Y') {
        /*
        Clear file by opening in truncate mode
        ios::trunc flag truncates (empties) the file
        This effectively deletes all content
        */
        ofstream outFile(DB_FILE, ios::trunc);
        outFile.close();
        cout << "✓ All data has been permanently deleted.\n";
    } 
    else {
        cout << "Operation cancelled.\n"; // User said no
    }
}

/*
===============================================================================
END OF PROGRAM
===============================================================================
Key Concepts Used in This Program:
1. Structures (struct) - Custom data types
2. Arrays - Fixed-size collections
3. File I/O - Reading/writing persistent data
4. String manipulation - C-style string functions
5. Loops - while, for
6. Conditionals - if/else
7. Functions - Code organization and reusability
8. Input validation - Handling user errors

Data Flow:
1. User input → Memory (array)
2. Memory (array) → File (permanent storage)
3. File → Memory (array) → Display to user

File Format (pipe-delimited):
Name|Destination|CurrentLocation
Example: John Smith|Saddar|Library
===============================================================================
*/
