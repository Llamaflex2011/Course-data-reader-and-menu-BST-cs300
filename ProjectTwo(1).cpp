/**
 * ============================================================================
 * Name        : ProjectTwo.cpp
 * Developer   : [Nick Galan]
 * Description : ABCU Advising System 
 * ============================================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <limits>

 // ============================================================================ 
 // STRUCT Course
 // ============================================================================ 
struct Course {
    std::string courseNumber;              // e.g., "CSCI100"
    std::string title;                     // e.g., "Introduction to Computer Science" (Mapped to courseTitle)
    std::vector<std::string> prerequisites; // Resizable list to hold a dynamic number of prerequisites
};

// ============================================================================ 
// STRUCT Node
// ============================================================================ 
struct Node {
    Course course;
    Node* leftChild;
    Node* rightChild;

    // Helper constructor matching CREATE Node(newCourse, NULL, NULL)
    Node(Course aCourse) {
        this->course = aCourse;
        this->leftChild = nullptr;
        this->rightChild = nullptr;
    }
};

// ============================================================================ 
// DATA STRUCTURE: Binary Search Tree Class
// ============================================================================ 
class BinarySearchTree {
private:
    Node* root; // Head root pointer location

    // Helper destructor to clear heap allocations safely
    void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->leftChild);
            destroyTree(node->rightChild);
            delete node;
        }
    }

    // ============================================================================ 
    // FUNCTION execute_in_order(current_node)
    // ============================================================================ 
    void execute_in_order(Node* current_node) {
        // Recursion base escape case: halt if target leaf path points out of bounds
        if (current_node == nullptr) {
            return;
        }

        // 1. Traverse down toward the lower alphanumeric keys recursively (Left)
        execute_in_order(current_node->leftChild);

        // 2. Process and output the data structural properties of this position
        std::cout << current_node->course.courseNumber << ", " << current_node->course.title << std::endl;

        // 3. Traverse down toward the higher alphanumeric keys recursively (Right)
        execute_in_order(current_node->rightChild);
    }

public:
    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        destroyTree(root);
    }

    // Provide a public getter check method for menu 
    bool is_empty() {
        return root == nullptr;
    }

    // ============================================================================ 
    // METHOD insert(Course newCourse)
    // ============================================================================ 
    void insert(Course newCourse) {
        Node* newNode = new Node(newCourse);

        if (root == nullptr) {
            root = newNode;
            return;
        }

        Node* current = root;

        while (true) {
            // Sort alphanumerically by course number string
            if (newCourse.courseNumber < current->course.courseNumber) {
                if (current->leftChild == nullptr) {
                    current->leftChild = newNode;
                    break;
                }
                else {
                    current = current->leftChild;
                }
            }
            else {
                if (current->rightChild == nullptr) {
                    current->rightChild = newNode;
                    break;
                }
                else {
                    current = current->rightChild;
                }
            }
        }
    }

    // ============================================================================ 
    // METHOD searchCourse(String courseNumber)
    // ============================================================================ 
    void searchCourse(std::string courseNumber) {
        Node* current = root;

        while (current != nullptr) {
            if (courseNumber == current->course.courseNumber) {
                std::cout << current->course.courseNumber << ", " << current->course.title << std::endl;

                if (current->course.prerequisites.empty()) {
                    std::cout << "Prerequisites: None" << std::endl;
                }
                else {
                    std::cout << "Prerequisites: ";
                    for (size_t i = 0; i < current->course.prerequisites.size(); ++i) {
                        std::cout << current->course.prerequisites[i];
                        if (i < current->course.prerequisites.size() - 1) {
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl;
                }
                return;
            }
            else if (courseNumber < current->course.courseNumber) {
                current = current->leftChild;
            }
            else {
                current = current->rightChild;
            }
        }

        std::cout << "Course " << courseNumber << " not found." << std::endl;
    }

    // ============================================================================ 
    // FUNCTION print_ordered_courses_bst(course_tree)
    // ============================================================================ 
    void print_ordered_courses_bst() {
        // Confirm the tree has a valid head root location before attempting recursion
        if (root == nullptr) {
            std::cout << "System warning: Tree data structure is empty." << std::endl;
            return;
        }

        std::cout << "--- Alphanumerically Sorted Course List (BST) ---" << std::endl;
        // Trigger the recursive engine pass beginning directly at the root point
        execute_in_order(root);
    }
};

// ============================================================================ 
// Helper Utility Functions
// ============================================================================ 

// String cleaning utility to map "TRIM" methods
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Case insensitivity formatting matching standard application requirements
std::string toUpperCase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

// ============================================================================ 
// METHOD loadCoursesFromFile(filename, bst)
// ============================================================================ 
void loadCoursesFromFile(std::string filename, BinarySearchTree* bst, bool& isDataLoaded) {
    // --- Pass 1: Split tokens and map existing courses --- 
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: File could not be opened, check path" << std::endl;
        return;
    }

    std::set<std::string> existingCourseNumbers;
    std::vector<Course> temporaryCourseList;
    std::string line;

    while (std::getline(file, line)) {
        line = trim(line);

        if (line.empty()) {
            continue; // Skip empty lines
        }

        // Parse line by comma delimiter
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Must have at least a course number and title
        if (tokens.size() < 2) {
            std::cout << "File format error: line must contain at least a course number and title. " << line << std::endl;
            file.close();
            return;
        }

        // Extract properties
        Course currentCourse;
        currentCourse.courseNumber = trim(tokens[0]);
        currentCourse.title = trim(tokens[1]);

        // Add prerequisites   
        for (size_t i = 2; i < tokens.size(); ++i) {
            std::string prereq = trim(tokens[i]);
            if (!prereq.empty()) {
                currentCourse.prerequisites.push_back(prereq);
            }
        }

        // Track main course
        existingCourseNumbers.insert(currentCourse.courseNumber);
        temporaryCourseList.push_back(currentCourse);
    }

    file.close();

    // --- Pass 2: Validate prerequisites and insert into BST --- 
    int insertedCount = 0;

    for (const Course& courseItem : temporaryCourseList) {
        bool isValid = true;

        for (const std::string& prereq : courseItem.prerequisites) {
            // Validation: Every prerequisite must exist as a course entry
            if (existingCourseNumbers.find(prereq) == existingCourseNumbers.end()) {
                std::cout << "Validation Error: Prerequisite " << prereq << " for course " << courseItem.courseNumber << std::endl;
                isValid = false;
            }
        }

        // Store valid courses into tree structure   
        if (isValid == true) {
            bst->insert(courseItem);
            insertedCount = insertedCount + 1;
        }
    }

    if (insertedCount > 0) {
        isDataLoaded = true;
        std::cout << "Data successfully loaded into system structure." << std::endl; std::cout << insertedCount << " courses successfully processed." << std::endl;
    }
    else {
        std::cout << "Error: File could not be loaded or parsed." << std::endl;
    }
}
// ============================================================================
// FUNCTION main_menu()
// ============================================================================
int main() {
    // Global or high-level variable initialization
    BinarySearchTree* bst = new BinarySearchTree(); // Holds course records (BST)
    bool is_data_loaded = false;                    // Flag track state
    int user_choice = 0;                            // Stores the numeric option chosen
    std::string filename;                           // Stores input text file target path
    std::string target_course;                      // Stores individual course lookup keys
    // Infinite loop keeps the program active until option 9 is explicitly triggered
    while (true) {
        // Display the standard user interface menu options
        std::cout << "========================================" << std::endl;
        std::cout << "   ABCU Advising System - Main Menu     " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "1. Load course data into system" << std::endl;
        std::cout << "2. Print alphanumerically ordered list" << std::endl;
        std::cout << "3. Search and print individual course info" << std::endl;
        std::cout << "9. Exit program" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Enter selection: ";
        // Protection against entering character strings into numeric options
        if (!(std::cin >> user_choice)) {
            std::cout << "Invalid option. Please input a valid choice (1, 2, 3, or 9)." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // Route actions cleanly based on user input selection
        switch (user_choice) {
        case 1:
            std::cout << "Enter the file name containing course data: ";
            std::cin >> filename;
            std::cout << "Attempting to load data file..." << std::endl;
            loadCoursesFromFile(filename, bst, is_data_loaded);
            break;
        case 2:
            // Pre-condition validation: Data must be loaded before a sort can occur
            if (is_data_loaded == true) {
                bst->print_ordered_courses_bst();
            }
            else {
                std::cout << "Error: No data available. Please select Option 1 first." << std::endl;
            }
            break;
        case 3:
            // Pre-condition validation: Data must be loaded before lookup can execute
            if (is_data_loaded == true) {
                std::cout << "Enter the exact course ID to search (e.g., CSCI100): ";
                std::cin >> target_course;
                target_course = toUpperCase(target_course);
                bst->searchCourse(target_course);
            }
            else {
                std::cout << "Error: No data available. Please select Option 1 first." << std::endl;
            }
            break;
        case 9:
            std::cout << "Thank you for using the ABCU Advising System. Goodbye!" << std::endl;
            delete bst; // Free memory safely
            return 0;   // Halts execution and safely shuts down system resources
        default:
            // Error handling fallback for any input outside of 1, 2, 3, or 9
            std::cout << "Invalid option. Please input a valid choice (1, 2, 3, or 9)." << std::endl;
            break;
        }
    }
    return 0;
}
