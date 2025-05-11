# Cafe-Management-System
OOP-Project

Project Title: Cafe Management System
Student Name: Vaania Ahmed, Umna Syed, Abeer Fatima
Roll Number: 24k-0504, 24k-0909, 24k-0985
Class: BCS-2C
Subject: Object-Oriented Programming
Instructor: Sir Talha Shahid
Date: 25th April, 2025

1. Introduction
The Cafe Management System is a C++ console-based application designed to streamline the
management operations of a cafe. It incorporates essential functionalities such as menu
management, order placement, billing, and table reservations. This project applies objectoriented programming (OOP) principles including classes, encapsulation, and file handling.

2. Objectives
 To understand and apply object-oriented programming concepts in a real-life scenario.  To create a modular and extensible system for managing cafe operations.  To implement basic file handling for data persistence.  To enhance problem-solving and programming logic through system design.

3. Features Implemented
 Menu Management: The system can load and save a menu from a file. Items are
categorized (e.g., Drinks, Snacks, Main Course) and displayed neatly.  Order Management: Customers can place orders by selecting items and specifying
quantities. Orders over $50 receive a 10% discount.  Billing System: Generates and saves a detailed receipt for each order.  Table Reservation: Allows customers to reserve tables. Reservations are saved to and
loaded from a file.  File Handling: Supports loading and saving data to text files (Menu.txt and
Reservations.txt).

4. Tools and Technologies Used
 Programming Language: C++
 Compiler: g++ / any C++11-compatible compiler  Platform: Console-based (cross-platform)  File Format: Plain text files for data persistence

5. Code Structure and Explanation
 Item Class: Represents a menu item with attributes like name, price, and category.  Menu Class: Manages the list of items, including loading from and saving to files, and
displaying categorized items.  Order Class: Handles adding items, computing totals, applying discounts, and
generating bills.  TableReservation Class: Manages reservation statuses and saves/loads them to/from a
file.  Main Function: Provides a CLI for interaction, offering options to view menu, place
orders, reserve tables, and view reservations.

6. Challenges Faced
 Ensuring index validation for user inputs.  Avoiding buffer overflows and handling invalid data gracefully.  Maintaining readability and modularity of the code with increasing features.  Implementing data persistence with file operations.

7. Conclusion
The Cafe Management System project provided hands-on experience with object-oriented
programming concepts and reinforced understanding of file handling in C++. It demonstrated
how modular and class-based design simplifies complex real-world problems. This system can
be further extended to include admin controls, user authentication, and graphical interfaces.

9. References
 C++ Documentation - cplusplus.com
 Object-Oriented Programming Principles
 Online tutorials and classroom lectures
