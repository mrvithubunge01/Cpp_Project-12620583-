
#include <iostream>
#include <string>
using namespace std;

class ScienceProject
{
private:
    int projectID;
    string projectTitle;
    string studentName;
    string studentClass;
    string scienceCategory;

    static int totalProjects;

public:
    // Function to enter project details
    void setDetails(int id, string title, string name,
                    string cls, string category)
    {
        this->projectID = id;
        this->projectTitle = title;
        this->studentName = name;
        this->studentClass = cls;
        this->scienceCategory = category;
    }

    // Function to get project ID
    int getID()
    {
        return projectID;
    }

    // Static function to count projects
    static void increaseCount()
    {
        totalProjects++;
    }

    static int getTotal()
    {
        return totalProjects;
    }

    // Friend function
    friend void displayProject(ScienceProject p);
};

// Initialize static variable
int ScienceProject::totalProjects = 0;

// Display project details
void displayProject(ScienceProject p)
{
    cout << "\nProject ID: " << p.projectID << endl;
    cout << "Project Title: " << p.projectTitle << endl;
    cout << "Student Name: " << p.studentName << endl;
    cout << "Student Class: " << p.studentClass << endl;
    cout << "Science Category: " << p.scienceCategory << endl;
}

// Main function
int main()
{
    ScienceProject projects[100];

    int count = 0;
    int choice;
    int id;
    bool duplicate;

    do
    {
        cout << "\n===== SCHOOL SCIENCE FAIR PROJECT EVALUATOR =====\n";
        cout << "1. Add Science Project\n";
        cout << "2. View All Projects\n";
        cout << "3. View Total Projects\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice))
        {
            cout << "Invalid input. Please restart the program and enter a number.\n";
            return 0;
        }

        switch (choice)
        {
            case 1:
                if (count >= 100)
                {
                    cout << "Project storage is full!\n";
                    break;
                }

                cout << "Enter Project ID: ";

                if (!(cin >> id) || id <= 0)
                {
                    cout << "Invalid ID! Enter a positive number.\n";
                    return 0;
                }

                duplicate = false;

                for (int i = 0; i < count; i++)
                {
                    if (projects[i].getID() == id)
                    {
                        duplicate = true;
                        break;
                    }
                }

                if (duplicate)
                {
                    cout << "Project ID already exists!\n";
                    break;
                }

                cin.ignore(1000, '\n');

                {
                    string title, name, cls, category;

                    cout << "Enter Project Title: ";
                    getline(cin, title);

                    cout << "Enter Student Name: ";
                    getline(cin, name);

                    cout << "Enter Student Class: ";
                    getline(cin, cls);

                    cout << "Enter Science Category: ";
                    getline(cin, category);

                    if (title.empty() || name.empty() ||
                        cls.empty() || category.empty())
                    {
                        cout << "Fields cannot be empty. Project not saved.\n";
                        break;
                    }

                    projects[count].setDetails(
                        id, title, name, cls, category
                    );

                    count++;
                    ScienceProject::increaseCount();

                    cout << "Project added successfully!\n";
                }
                break;

            case 2:
                if (count == 0)
                {
                    cout << "No projects available.\n";
                }
                else
                {
                    for (int i = 0; i < count; i++)
                    {
                        ScienceProject* ptr = &projects[i];
                        displayProject(*ptr);
                    }
                }
                break;

            case 3:
                cout << "Total Projects: "
                     << ScienceProject::getTotal() << endl;
                break;

            case 4:
                cout << "Thank you! Exiting program.\n";
                break;

            default:
                cout << "Invalid choice! Enter 1 to 4.\n";
        }

    } while (choice != 4);

    return 0;
}
