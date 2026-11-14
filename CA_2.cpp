
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

const int MAX = 20;

// Binary File Record
struct Record {
    int id;
    char title[50], name[50], cls[20], category[30];
};

class ScienceProject {
private:
    int projectID;
    string projectTitle, studentName, studentClass, scienceCategory;
    static int totalProjects;

public:

    // Default Constructor
    ScienceProject() {
        projectID = 0;
        projectTitle = studentName = studentClass = scienceCategory = "";
    }

    // Parameterized Constructor
    ScienceProject(int id, string title, string name, string cls, string cat) {
        projectID = id;
        projectTitle = title;
        studentName = name;
        studentClass = cls;
        scienceCategory = cat;
    }

    // Copy Constructor
    ScienceProject(const ScienceProject &p) {
        projectID = p.projectID;
        projectTitle = p.projectTitle;
        studentName = p.studentName;
        studentClass = p.studentClass;
        scienceCategory = p.scienceCategory;
    }

    // Destructor
    ~ScienceProject() {}

    // this Pointer
    void setDetails(int id, string title, string name, string cls, string cat) {
        this->projectID = id;
        this->projectTitle = title;
        this->studentName = name;
        this->studentClass = cls;
        this->scienceCategory = cat;
    }

    // Inline Function
    inline int getProjectID() const {
        return projectID;
    }

    // Static Functions
    static int getTotalProjects() {
        return totalProjects;
    }

    static void setTotalProjects(int n) {
        totalProjects = n;
    }

    static void showTotalProjects() {
        cout << "\nTotal Projects: " << totalProjects << endl;
    }

    // Friend Function
    friend void displayProject(const ScienceProject &p);

    // Convert Object into Binary Record
    Record getRecord() const {
        Record r = {};
        r.id = projectID;
        strncpy(r.title, projectTitle.c_str(), 49);
        strncpy(r.name, studentName.c_str(), 49);
        strncpy(r.cls, studentClass.c_str(), 19);
        strncpy(r.category, scienceCategory.c_str(), 29);
        return r;
    }
};

int ScienceProject::totalProjects = 0;


// Friend Function
void displayProject(const ScienceProject &p) {
    cout << "\nProject ID       : " << p.projectID;
    cout << "\nProject Title    : " << p.projectTitle;
    cout << "\nStudent Name     : " << p.studentName;
    cout << "\nStudent Class    : " << p.studentClass;
    cout << "\nScience Category : " << p.scienceCategory << endl;
}

// Convert Record into Object
ScienceProject makeProject(Record r) {
    return ScienceProject(r.id, r.title, r.name, r.cls, r.category);
}

// Input Validation
int getNumber(string msg) {
    string s;
    int n;
    char extra;

    while (true) {
        cout << msg;
        getline(cin, s);
        stringstream ss(s);

        if (ss >> n && !(ss >> extra))
            return n;

        cout << "Enter a valid number!\n";
    }
}

string getText(string msg, int limit) {
    string s;

    while (true) {
        cout << msg;
        getline(cin, s);

        if (s.empty() || s.find_first_not_of(" \t") == string::npos)
            cout << "Field cannot be empty!\n";
        else if (s.length() > limit)
            cout << "Maximum characters: " << limit << endl;
        else
            return s;
    }
}

// Load Previous Records
void loadProjects(ScienceProject p[]) {
    ifstream file("project.dat", ios::binary);
    Record r;
    int count = 0;

    while (file.read((char*)&r, sizeof(r)) && count < MAX)
        p[count++] = makeProject(r);

    ScienceProject::setTotalProjects(count);
    file.close();
}

// Check Duplicate ID
bool checkDuplicate(int id, ScienceProject p[]) {
    for (int i = 0; i < ScienceProject::getTotalProjects(); i++)
        if (p[i].getProjectID() == id)
            return true;

    return false;
}

// Add Project
void addProject(ScienceProject p[]) {
    int count = ScienceProject::getTotalProjects();

    if (count >= MAX) {
        cout << "Maximum limit reached!\n";
        return;
    }

    int id;

    while (true) {
        id = getNumber("Enter Project ID: ");

        if (id <= 0)
            cout << "ID must be positive!\n";
        else if (checkDuplicate(id, p))
            cout << "Duplicate ID! Try again.\n";
        else
            break;
    }

    string title = getText("Enter Project Title: ", 49);
    string name = getText("Enter Student Name: ", 49);
    string cls = getText("Enter Student Class: ", 19);
    string cat = getText("Enter Science Category: ", 29);

    ScienceProject obj(id, title, name, cls, cat);
    Record r = obj.getRecord();

    ofstream file("project.dat", ios::binary | ios::app);

    if (!file) {
        cout << "File opening failed!\n";
        return;
    }

    file.write((char*)&r, sizeof(r));

    if (!file) {
        cout << "Saving failed!\n";
        return;
    }

    file.close();

    p[count] = obj;
    ScienceProject::setTotalProjects(count + 1);

    cout << "Project Added Successfully!\n";
}

// Display All Projects
void displayAll(ScienceProject p[]) {
    int count = ScienceProject::getTotalProjects();

    if (count == 0) {
        cout << "No Projects Available!\n";
        return;
    }

    ScienceProject *ptr = p;

    for (int i = 0; i < count; i++) {
        ScienceProject temp(*(ptr + i));
        displayProject(temp);
    }
}

// Search Project using seekg()
void searchProject() {
    int id = getNumber("Enter Project ID: ");

    ifstream file("project.dat", ios::binary);

    if (!file) {
        cout << "File not found!\n";
        return;
    }

    Record r;
    int pos = 0;
    bool found = false;

    while (true) {
        file.seekg(pos * sizeof(r), ios::beg);

        if (!file.read((char*)&r, sizeof(r)))
            break;

        if (r.id == id) {
            ScienceProject obj = makeProject(r);
            displayProject(obj);
            found = true;
            break;
        }

        pos++;
    }

    if (!found)
        cout << "Project Not Found!\n";

    file.close();
}

// Update Project using seekp()
void updateProject(ScienceProject p[]) {
    int id = getNumber("Enter Project ID to Update: ");

    fstream file("project.dat", ios::binary | ios::in | ios::out);

    if (!file) {
        cout << "File opening failed!\n";
        return;
    }

    Record r;
    streampos pos;
    bool found = false;

    while (true) {
        pos = file.tellg();

        if (!file.read((char*)&r, sizeof(r)))
            break;

        if (r.id == id) {
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Project Not Found!\n";
        file.close();
        return;
    }

    displayProject(makeProject(r));

    cout << "\nEnter Updated Details:\n";

    string title = getText("New Title: ", 49);
    string name = getText("New Student Name: ", 49);
    string cls = getText("New Class: ", 19);
    string cat = getText("New Category: ", 29);

    ScienceProject obj(id, title, name, cls, cat);
    Record newRecord = obj.getRecord();

    file.clear();
    file.seekp(pos);
    file.write((char*)&newRecord, sizeof(newRecord));
    file.close();

    for (int i = 0; i < ScienceProject::getTotalProjects(); i++) {
        if (p[i].getProjectID() == id) {
            p[i].setDetails(id, title, name, cls, cat);
            break;
        }
    }

    cout << "Project Updated Successfully!\n";
}


// Main Function
int main() {
    ScienceProject projects[MAX];
    loadProjects(projects);

    int choice;

    do {
        cout << " SCHOOL SCIENCE FAIR PROJECT EVALUATOR\n";
        cout << "1. Add New Project\n";
        cout << "2. View All Projects\n";
        cout << "3. Search Project\n";
        cout << "4. Update Project\n";
        cout << "5. Total Projects\n";
        cout << "0. Exit\n";

        choice = getNumber("Enter Your Choice: ");

        switch (choice) {
            case 1: addProject(projects); break;
            case 2: displayAll(projects); break;
            case 3: searchProject(); break;
            case 4: updateProject(projects); break;
            case 5: ScienceProject::showTotalProjects(); break;
            case 0: cout << "Program Closed!\n"; break;
            default: cout << "Invalid Choice!\n";
        }

    } while (choice != 0);

    return 0;
}
