# University-Ride-Share-CPP
A C++ console application connecting students for shared commutes.

# 🚗 University Ride Share System (C++)

## Overview

The **University Ride Share System** is a **console-based C++ application** designed to help university students **find and share rides** based on their destination and current location.
The system enables efficient ride coordination, reduces transportation costs, and promotes environmentally friendly commuting among students.

This project demonstrates **real-world problem solving**, **file-based data persistence**, and **structured program design** using core C++ concepts.

---
## UOM SharRider Visualizer 




The visualizer is actually the summary of our C++ code, it visually explains what our program is doing, for what purpose we make it, and how our program works.
🔗 Link to Visualizer:https://claude.ai/chat/404a971d-a320-4bfe-9143-3c6134c74fe2?artifactId=standalone_visualizer





## 🎯 Project Objectives

* Connect students traveling to the **same destination**
* Reduce transportation costs and traffic congestion
* Provide a **simple and reliable ride-matching system**
* Demonstrate practical use of **C++ file handling and data structures**

---

## ✨ Key Features

### 🚀 Core Functionality

* Student registration and profile updates
* Destination-based ride partner matching
* Persistent data storage using file handling
* View all registered students
* Secure data deletion with confirmation

### 💾 Data Management

* Permanent storage using text files
* Automatic file creation if database is missing
* Safe overwrite and truncation handling

### 🧭 User Interaction

* Menu-driven console interface
* Input validation and error handling
* Clear prompts and formatted output

---

## 🛠 Technologies & Concepts Used

* **C++**
* File Handling (`ifstream`, `ofstream`)
* Structures (`struct`)
* String handling (`cstring`)
* Conditional logic & loops
* Console-based UI design

---

## 🏗 System Architecture Overview

### University Ride Share System

```
University-Ride-Share-CPP
├── CPP
│   └── Ride_Share
│       ├── Ride_Share.cpp
│       └── README.md
├── Prototype
│   └── README.md
└── README.md
```

---

## 🔄 Application Workflow

### Registration Flow

```
User Input → Data Validation → File Storage → Confirmation Message
```

### Ride Matching Flow

```
User Destination → Search Database → Matching Students → Display Results
```

---

## 🧪 How to Run the Program

### Prerequisites

* C++ compiler supporting **C++11 or later**

  * GCC / MinGW / MSVC / Clang
* Command-line or terminal environment

### Compilation & Execution

```bash
g++ Ride_Share.cpp -o ride_share
./ride_share
```

---

## 📋 Usage Guide

### Main Menu

```
1. Register / Update My Location
2. Find Students Going to My Destination
3. View All Registered Students
4. Clear All Data
5. Exit
```

---

### 📝 Register a Student

* Enter name
* Enter destination
* Enter current location
* Optional photo filename
* Data is saved permanently

---

### 🔍 Find Ride Partners

* Enter your destination
* System lists students going to the same location
* Displays name, current location, and photo reference

---

### 📊 View All Students

* Displays all registered students
* Shows destination and current location
* Provides total count

---

### 🗑 Clear All Data

* Requires confirmation (`Y/N`)
* Safely deletes all stored records

---

## 📦 Data Storage Format

### Database File

```
ride_share_data.txt
```

### Record Format

```
Name|Destination|CurrentLocation|PhotoName
```

Example:

```
Ali|Saddar|Library|ali.png
```

---

## 🎨 Prototype






A **visual prototype** is included to represent a **future GUI / web or mobile version** of the system.


🔗 Live Prototype:
[https://cqk7zv5ge23tg.ok.kimi.link/](https://cqk7zv5ge23tg.ok.kimi.link/)





---

## 🔐 Data Integrity & Safety

* Input validation prevents invalid entries
* File operations include error checking
* Confirmation required before data deletion
* Safe handling of empty or missing files

---

## 🔮 Future Enhancements

* Graphical User Interface (GUI)
* Real-time ride matching
* Map and distance integration
* Database support (MySQL / SQLite)
* User authentication system
* Mobile or web-based version

---

## 🎓 Academic Context

This project is developed as part of a **University Programming ** to demonstrate:

* Structured programming in C++
* File-based data management
* Problem-solving using real-world scenarios
* Clean code organization and documentation

---

## 👤 Author

**Sudais Ahmad**
BS Artificial Intelligence at UOM (University Of Malakand)
University Project – Ride Sharing System (C++)
