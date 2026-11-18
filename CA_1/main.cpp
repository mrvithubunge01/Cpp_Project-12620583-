
#include <iostream>
#include <string>
using namespace std;

class ScienceProject {
private:
    int projectID;
    string projectTitle;
    string studentName;
    string studentClass;
    string scienceCategory;

    static int totalProjects;

public:

    // Default constructor
    ScienceProject() {
        projectID = 0;
        projectTitle = "";
        studentName = "";
        studentClass = "";
        scienceCategory = "";
    }

    // Using this pointer
    void setDetails(int projectID, string projectTitle,
                    string studentName, string studentClass,
                    string scienceCategory) {

        this->projectID = projectID;
        this->projectTitle = projectTitle;
        this->studentName = studentName;
        this->studentClass = studentClass;
        this->scienceCategory = scienceCategory;

        totalProjects++;
    }

    // Inline function
    inline int getProjectID() {
        return projectID;
    }

    // Friend function
    friend void displayProject(ScienceProject p);

    // Static function
    static void showTotalProjects() {
        cout << "\nTotal Projects: " << totalProjects << endl;
    }
};

// Initialize static variable
int ScienceProject::totalProjects = 0;

// Friend function to display project details
void displayProject(ScienceProject p) {

    cout<<endl;
    cout <<endl<< "Project ID       : " << p.projectID;
    cout <<endl<< "Project Title    : " << p.projectTitle;
    cout <<endl<< "Student Name     : " << p.studentName;
    cout <<endl<< "Student Class    : " << p.studentClass;
    cout <<endl<< "Science Category : " << p.scienceCategory;
    cout<<endl;
}

int main() {

    ScienceProject projects[5];

    ScienceProject *ptr;

    int count = 0;
    int choice;

    do {

        cout << endl;
        cout << "SCHOOL SCIENCE FAIR PROJECTS EVALUATOR";
        cout << endl;

        cout <<endl<< "1. Add New Project";
        cout <<endl<< "2. View All Projects";
        cout <<endl<< "3. View Total Projects";
        cout <<endl<< "4. Exit";

        cout <<endl<< "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {

            if (count >= 5) {
                cout <<endl<< "Maximum project limit reached!"<<endl;
                continue;
            }

            int id;
            string title, name, studentClass, category;

            cout << "Enter Project ID: ";
            cin >> id;

            // Check positive ID
            if (id <= 0) {
                cout <<endl<< "Invalid Project ID!";
                continue;
            }

            // Check duplicate ID
            bool duplicate = false;

            for (int i = 0; i < count; i++) {
                if (projects[i].getProjectID() == id) {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate) {
                cout <<endl<< "Project ID already exists!";
                continue;
            }

            cin.ignore();

            cout << "Enter Project Title: ";
            getline(cin, title);

            cout << "Enter Student Name: ";
            getline(cin, name);

            cout << "Enter Student Class: ";
            getline(cin, studentClass);

            cout << "Enter Category : ";
            getline(cin, category);

            // Empty field validation
            if (title.empty() || name.empty() ||
                studentClass.empty() || category.empty()) {

                cout <<endl<< "All fields are required!";
                continue;
            }

            projects[count].setDetails(
                id, title, name, studentClass, category
            );

            count++;

            cout <<endl<< "Project Added Successfully!"<<endl;

        }

        else if (choice == 2) {

            if (count == 0) {
                cout <<endl<< "No Projects Available!";
            }
            else {

                for (int i = 0; i < count; i++) {

                    ptr = &projects[i];

                    displayProject(*ptr);
                }
            }
        }

        else if (choice == 3) {

            ScienceProject::showTotalProjects();

        }

        else if (choice == 4) {

            cout <<endl<< "Thank You! Exiting Program.";

        }

        else {

            cout <<endl<< "Invalid Choice! Try Again.";

        }

    } while (choice != 4);

    return 0;
}
