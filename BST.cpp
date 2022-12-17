//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Blake kemp
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Project 2 in C++, Reads the CSV and prints required information
//============================================================================

#include <iostream>

#include <fstream>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

/* Each course contains and Id, Name and a vector to hold
   as many prerequisites as the course contains.
   */
struct Course {
    string courseNum;
    string courseName;
    vector<string> preReqs;

    Course() {}
};
void displayCourse(Course course);

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) : Node() {
        this->course = aCourse;
    }

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;
    void addNode(Node* node, Course course);
    void displayInOrder(Node* node);
    void deleteTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void DisplayInOrder();
    Course Search(string courseNum);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //Initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
void BinarySearchTree::deleteTree(Node* node) {
    if (node == nullptr) {
        return;
    }

    //delete left sub tree
    deleteTree(node->left);

    //delete right sub tree
    deleteTree(node->right);

    //delete node
    delete node;
}

BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteTree(root);
}

/**
* Insert a node into the BST
*/
void BinarySearchTree::Insert(Course course) {
    //if the root isnt initialized
    if (root == nullptr) {
        root = new Node(course); //initialize it with the course info
    }
    else { //send the node to addNode method
        this->addNode(root, course);
    }
}
/**
* Print node(s) in order
*
* @param node starting node
*/
void BinarySearchTree::displayInOrder(Node* node) {

    if (node == nullptr) {
        return;
    }

    // print left sub tree
    displayInOrder(node->left);

    // display course information
    displayCourse(node->course);

    // print right sub tree
    displayInOrder(node->right);
}

/**
* Print the tree in order
*/
void BinarySearchTree::DisplayInOrder() {

    // recursively print tree in order
    displayInOrder(root);
}

/**
* Search for a specific course number
*
* @param courseNumber course to be searched
*/
Course BinarySearchTree::Search(string courseNum) {
    Course course;

    // start with root node
    Node* currNode = root;

    // loop through tree
    while (currNode != nullptr) {
        if (courseNum.compare(currNode->course.courseNum) == 0) {
            // if found, return course
            course = currNode->course;
            break;
        }
        // if courseNumber < current node->couse.courseNumber,
        // loop through left sub tree
        else if (courseNum.compare(currNode->course.courseNum) < 0) {
            currNode = currNode->left;
        }
        else { // loop through right sub tree
            currNode = currNode->right;
        }
    }

    return course;
}

/**
* Display course information to console
*
* @param course struct containing course information
*/
void displayCourse(Course course) {

    cout << "Course Number: " << course.courseNum;
    cout << ", Course Name: " << course.courseName << endl;
    cout << "Pre-Requisite(s): ";

    if (course.preReqs.size() == 0) {
        cout << "None.";
    }
    else {
        cout << course.preReqs.at(0);

        for (unsigned int i = 1; i < course.preReqs.size(); ++i) {
            cout << ", " << course.preReqs.at(i);
        }
    }
    cout << endl;
    cout << endl;
}


/**
 * Using the public insert method determine the alphanumeric location and if the current child equals nullptr then add it to the BST
 * otherwise keep traversing until a nullptr is found
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Current courseNum is less tahn the current node courseNum
    Node* currNode = node;

    while (currNode != nullptr) {

        if (course.courseNum.compare(currNode->course.courseNum) < 0) {
            if (currNode->left == nullptr) {  // if left node is empty
                // assign new course to left node 
                currNode->left = new Node(course);
                break;
            }
            // assign current node to left node
            currNode = currNode->left;
        }
        else {
            // if right node is empty
            if (currNode->right == nullptr) {
                // assign new course to right node                 
                currNode->right = new Node(course);
                break;
            }
            // assign current node to right node
            currNode = currNode->right;
        }
    }
}


// error handling
struct FileParserError : public exception {
    string  msg;
    FileParserError(string err) {
        msg = string("File Parser: ").append(err);
    }
    const char* what() const throw () {
        return msg.c_str();
    }
};



/**
* split string by ',' delimiter
*/
vector<string> parseLine(string line) {
    vector<string> row;
    string delimiter = ",";
    int startIndex = 0;
    int endIndex = line.find(delimiter);

    while (endIndex != -1) {
        row.push_back(line.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + delimiter.size();
        endIndex = line.find(delimiter, startIndex);
    }
    row.push_back(line.substr(startIndex, endIndex - startIndex));
    return row;
}

vector<vector<string>> readFile(string file) {
    vector<vector<string>> fileContents;
    fstream csvFile;

    // open file for reading
    csvFile.open(file, ios::in);

    if (csvFile.is_open()) {
        string line;
        // read data from file, line by line
        while (getline(csvFile, line)) {
            if (line != "") {
                // string string by ','
                vector<string> row = parseLine(line);
                fileContents.push_back(row);
            }
        }

        // close file
        csvFile.close();
    }
    else {
        throw FileParserError(string("Failed to open file: ").append(file));
    }

    return fileContents;
}
//============================================================================
// Static methods used for testing
//Load Courses from cin choice for csv file
void loadCourses(string file, BinarySearchTree* coursesBST) {
    // Open course file, get/ separate each line & insert into BST
    vector<string> courseTitles;
    try {
        // read file file
        vector<vector<string>> fileContents = readFile(file);

        // extract course titles
        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                courseTitles.push_back(row.at(0));
            }
        }

        for (unsigned int i = 0; i < fileContents.size(); ++i) {
            vector<string> row = fileContents.at(i);
            if (row.size() >= 2) {
                Course course;

                // course number
                course.courseNum = row.at(0);

                // course name
                course.courseName = row.at(1);

                // add pre-requisite(s)
                for (unsigned int j = 2; j < row.size(); ++j) {
                    if (find(courseTitles.begin(), courseTitles.end(), row.at(j)) != courseTitles.end()) {
                        course.preReqs.push_back(row.at(j));
                    }
                }

                // insert into binary search tree
                coursesBST->Insert(course);
            }
        }
    }
    catch (FileParserError& err) {
        cerr << err.what() << endl;
    }


}
/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {
    // Process command line argument
    string file, courseId;
    switch (argc) {
    case 2:
        file = argv[1];
        break;
    case 3:
        file = argv[1];
        courseId = argv[2];
        break;
    default:
        file = "";
        break;
    }
    // Define Binary Search Tree and  Welcome User
    BinarySearchTree* searchTree = new BinarySearchTree();
    cout << "\nWelcome to the course planner!\n" << endl;
    // Making user choice a string and converting first char to int prevents invalid data
    string choice = "0";
    int userChoice = choice[0] - '0';
    //if not 9 or exit
    while (userChoice != 9) {
        string file, courseNum;
        Course course;
        string number;

        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << "\nPlease make a choice ";
        cin >> choice;
        // Check if user choice is a double digit
        if (choice.length() == 1) {
            userChoice = choice[0] - '0';
        }
        else {
            userChoice = 0;
        }
        // Handle users choice from menu
        switch (userChoice) {
            // Instantiate Binary Search Tree
            // get file path name
            //and load courses into Binary Search Tree
        case 1:
            searchTree = new BinarySearchTree;
            cout << "Enter the file name that contains the course data: ";
            cin.ignore();
            getline(cin, file);

            // Determine if the file was opened- load successful
            loadCourses(file, searchTree);
            cout << "Data Loaded" << endl;
            break;
            // Validate that a Binary Search Tree  exists
            //and print a sample schedule
        case 2:
            searchTree->DisplayInOrder();
            break;
            // Validate that a Binary Search Tree  exists
            //and search/ display course info
        case 3:
            // search for a specific course number
            cout << "Enter course number: ";
            cin.ignore();
            getline(cin, courseNum);

            // check if input is empty
            number = courseNum;

            number.erase(remove_if(number.begin(), number.end(), isspace), number.end());

            if (number == "") {
                cout << "Course number cannot be empty" << endl;
            }
            else {
                course = searchTree->Search(courseNum);
                if (!course.courseNum.empty()) {
                    displayCourse(course);
                }
                else {
                    cout << "Course number " << courseNum << " not found." << endl;
                }
            }
            break;

        default:
            if (userChoice != 9) {
                cout << choice << " is not a valid choice." << endl;
            }
            break;
        }
    }
    cout << "Thank you for using the course planner!" << endl;
    return 0;
}