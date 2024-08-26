// Martin Vaca
// CS 300 Analysis and Design
// 08/17/2024
// @SNHU

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Course Structure to store course information
struct Course
{
    string courseNum;       // Course number
    string courseName;      // Course name
    vector<string> PreReqs; // List of prerequisites for the course

    // Default constructor
    Course() {}

    // Constructor with course number and name
    Course(string aCourseNum, string aCourseName)
    {
        courseNum = aCourseNum;
        courseName = aCourseName;
    }

    // Constructor with course number, name, and prerequisites
    Course(string aCourseNum, string aCourseName, vector<string> prereqs)
    {
        courseNum = aCourseNum;
        courseName = aCourseName;
        PreReqs = prereqs;
    }
};

// Node Structure for Binary Search Tree (BST)
struct Node
{
    Course course; // Course information stored in the node
    Node *left;    // Pointer to the left child node
    Node *right;   // Pointer to the right child node

    // Default constructor initializing pointers to nullptr
    Node() : left(nullptr), right(nullptr) {}

    // Constructor with course initialization
    Node(Course acourse) : Node()
    {
        this->course = acourse;
    }
};

// Binary Search Tree (BST) Class
class BST
{
private:
    Node *root; // Root node of the BST

    // Helper function to recursively add a node to the BST
    void addNode(Course acourse, Node *node);

    // Helper function to recursively traverse the BST in order
    void inOrder(Node *node);

    // Helper function to recursively search and print course information
    void printCourse(Node *node, string courseNum);

public:
    // Constructor to initialize the BST
    BST();

    // Public method to start in-order traversal from the root
    void InOrder();

    // Public method to search and print a specific course
    void PrintCourse(string courseNum);

    // Public method to insert a course into the BST
    void insert(Course course);
};

// Constructor initializing root to nullptr
BST::BST()
{
    root = nullptr;
}

// Public method to start in-order traversal from the root
void BST::InOrder()
{
    this->inOrder(root);
}

// Public method to search and print course information for a specific course
void BST::PrintCourse(string courseNum)
{
    this->printCourse(root, courseNum);
}

// Public method to insert a course into the BST
void BST::insert(Course course)
{
    if (root == nullptr)
    {
        root = new Node(course); // If the BST is empty, the new course becomes the root
    }
    else
    {
        this->addNode(course, root); // Otherwise, add the course to the correct position
    }
}

// Helper function to recursively add a node to the BST
void BST::addNode(Course acourse, Node *node)
{
    // If the course number is smaller, go to the left subtree
    if (node->course.courseNum.compare(acourse.courseNum) > 0)
    {
        if (node->left == nullptr)
        {
            node->left = new Node(acourse);
        }
        else
        {
            this->addNode(acourse, node->left);
        }
    }
    // If the course number is larger, go to the right subtree
    else
    {
        if (node->right == nullptr)
        {
            node->right = new Node(acourse);
        }
        else
        {
            this->addNode(acourse, node->right);
        }
    }
}

// Helper function to recursively traverse the BST in order
void BST::inOrder(Node *node)
{
    if (node != nullptr)
    {
        inOrder(node->left); // Traverse the left subtree
        // Print course information
        cout << "Course Number: " << node->course.courseNum << " Course Name: " << node->course.courseName << " Prerequisites: ";
        for (const auto &prereq : node->course.PreReqs)
        {
            cout << prereq << " ";
        }
        if (node->course.PreReqs.empty())
        {
            cout << "None";
        }
        cout << "\n";
        inOrder(node->right); // Traverse the right subtree
    }
}

// Helper function to recursively search and print course information
void BST::printCourse(Node *node, string courseNum)
{
    while (node != nullptr)
    {
        if (node->course.courseNum.compare(courseNum) == 0)
        {
            // If course is found, print course information
            cout << "Course Number: " << node->course.courseNum << "  Course Name: " << node->course.courseName << "  Prerequisites: ";
            for (const auto &prereq : node->course.PreReqs)
            {
                cout << prereq << " ";
            }
            if (node->course.PreReqs.empty())
            {
                cout << "None";
            }
            cout << "\n";
            return;
        }
        else if (node->course.courseNum.compare(courseNum) > 0)
        {
            node = node->left; // Search in the left subtree
        }
        else
        {
            node = node->right; // Search in the right subtree
        }
    }
    // If course is not found
    cout << "Course not found. Are you sure you entered the right course number? \n";
}

// Function to load courses from a CSV file into a vector
vector<Course> loadCourses(string csvPath)
{
    vector<Course> courses; // Vector to store all the courses
    ifstream file(csvPath); // Input file stream to read the CSV file

    if (!file.is_open())
    {
        throw runtime_error("Failed to open file " + csvPath); // Error handling if the file cannot be opened
    }

    string line;
    getline(file, line); // Read the header line (if any)

    // Read each line from the CSV file
    while (getline(file, line))
    {
        stringstream ss(line); // String stream to tokenize the line
        string token;
        Course course;

        // Extract course number and name
        getline(ss, course.courseNum, ',');
        getline(ss, course.courseName, ',');

        // Extract prerequisites
        while (getline(ss, token, ','))
        {
            course.PreReqs.push_back(token);
        }

        courses.push_back(course); // Add the course to the vector
    }

    file.close(); // Close the file after reading
    return courses;
}

// Function to load courses into a hash table
unordered_map<string, Course> loadCoursesToHashTable(string csvPath)
{
    unordered_map<string, Course> courseTable;     // Hash table to store courses with course number as key
    vector<Course> courses = loadCourses(csvPath); // Load courses into a vector

    for (auto &course : courses)
    {
        courseTable[course.courseNum] = course; // Insert each course into the hash table
    }

    return courseTable;
}

// Function to load courses into a BST
BST loadCoursesToBST(string csvPath)
{
    BST bst;                                       // Create a Binary Search Tree
    vector<Course> courses = loadCourses(csvPath); // Load courses into a vector

    for (auto &course : courses)
    {
        bst.insert(course); // Insert each course into the BST
    }

    return bst;
}

// Main function to drive the program
int main()
{
    vector<Course> courses;                    // Vector to store course data
    unordered_map<string, Course> courseTable; // Hash table to store course data
    BST bst;                                   // Binary search tree to store course data

    char menuChoice = '0'; // Variable to store the user's menu choice
    while (menuChoice != '9')
    { // Continue looping until the user chooses to exit
        // Display the menu
        cout << "Menu:" << endl;
        cout << "1. Load data from CSV" << endl;
        cout << "2. Display all Courses" << endl;
        cout << "3. Display the title and prerequisites of a specific course" << endl;
        cout << "9. Exit" << endl;

        // Prompt the user for their choice
        cout << "Enter your choice: ";
        cin >> menuChoice;

        // Handle the user's menu choice
        switch (menuChoice)
        {
        case '1':
        {
            // Prompt the user to enter the file name
            string csvFilePath = "CS 300 ABCU_Advising_Program_Input.csv"; // Predefined CSV file path
            cout << "Loading data from file: " << csvFilePath << endl;

            // Load courses into vector, hash table, and BST
            try
            {
                courses = loadCourses(csvFilePath);
                courseTable = loadCoursesToHashTable(csvFilePath);
                bst = loadCoursesToBST(csvFilePath);
                cout << courses.size() << " courses loaded successfully." << endl;
            }
            catch (const runtime_error &e)
            {
                // Handle error if file cannot be opened
                cout << e.what() << endl;
            }
            break;
        }

        case '2':
        {
            // Ensure data is loaded before displaying courses
            if (courses.empty())
            {
                cout << "No data loaded. Please load the CSV file first (Option 1)." << endl;
                break;
            }

            // Display all courses in alphanumeric order using the vector
            cout << "Courses in alphanumeric order: " << endl;
            // Sort the vector based on course number
            sort(courses.begin(), courses.end(), [](Course a, Course b)
                 { return a.courseNum < b.courseNum; });

            // Print out each course in the sorted vector
            for (const auto &course : courses)
            {
                cout << course.courseNum << ": " << course.courseName << " Prerequisites: ";
                for (const auto &prereq : course.PreReqs)
                {
                    cout << prereq << " ";
                }
                cout << endl;
            }
            break;
        }

        case '3':
        {
            // Ensure data is loaded before searching for a course
            if (courses.empty())
            {
                cout << "No data loaded. Please load the CSV file first (Option 1)." << endl;
                break;
            }

            // Prompt the user for the course number they want to search
            string courseNum;
            cout << "Please enter the course number you would like to search: ";
            cin >> courseNum;

            // Search for the course in the BST and display its information
            bst.PrintCourse(courseNum);
            break;
        }

        case '9':
        {
            // Exit the program
            cout << "Exiting the application." << endl;
            break;
        }

        default:
        {
            // Handle invalid menu choices
            cout << "Invalid menu choice. Please select a valid option." << endl;
            break;
        }
        }
    }

    // Goodbye message when the program ends
    cout << "Goodbye" << endl;
    return 0;
}
