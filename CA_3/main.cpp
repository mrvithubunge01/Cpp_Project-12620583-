#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
using namespace std;

const int MAX = 20;
const char FILE_NAME[] = "project.dat";
const char TEMP_NAME[] = "temp.dat";
const char BACKUP_NAME[] = "backup.dat";

struct Record {
    int id;
    char type;
    char title[50], name[50], studentClass[20], category[30];
};

void saveText(char target[], const string& value, int size) {
    strncpy(target, value.c_str(), size - 1);
    target[size - 1] = '\0';
}

string getText(const string& message, int limit) {
    string value;
    while (true) {
        cout << message;
        getline(cin >> ws, value);
        if (value.empty()) cout << "This field cannot be empty.\n";
        else if (value.length() >= static_cast<size_t>(limit))
            cout << "Maximum characters: " << limit - 1 << "\n";
        else return value;
    }
}

int getNumber(const string& message) {
    string line;
    int number;
    char extra;
    while (true) {
        cout << message;
        getline(cin, line);
        stringstream input(line);
        if (input >> number && !(input >> extra)) return number;
        cout << "Enter a valid number.\n";
    }
}

// ---------------- BASE CLASS ----------------
class Project {
protected:
    int id;
    string title, studentName, studentClass;
    static int total;

public:
    Project() : id(0) {}
    Project(int projectID, string projectTitle, string name, string cls) {
        this->id = projectID;
        this->title = projectTitle;
        this->studentName = name;
        this->studentClass = cls;
    }
    Project(const Project& other)
        : id(other.id), title(other.title), studentName(other.studentName),
          studentClass(other.studentClass) {}
    virtual ~Project() {}

    inline int getID() const { return id; }

    // Friend function: can access the protected project details.
    friend void showProjectDetails(const Project& p);
    static int getTotal() { return total; }
    static void setTotal(int value) { total = value; }

    virtual char getType() const = 0;
    virtual string getCategory() const = 0;
    virtual void readCategory() = 0;

    void showCommon(ostream& out) const {
        out << "\nProject ID       : " << id
            << "\nProject Title    : " << title
            << "\nStudent Name     : " << studentName
            << "\nStudent Class    : " << studentClass;
    }

    Record toRecord() const {
        Record r = {};
        r.id = id;
        r.type = getType();
        saveText(r.title, title, 50);
        saveText(r.name, studentName, 50);
        saveText(r.studentClass, studentClass, 20);
        saveText(r.category, getCategory(), 30);
        return r;
    }

    // Type conversion: Project object to its ID.
    operator int() const { return id; }

    friend ostream& operator<<(ostream& out, const Project& p);
    friend istream& operator>>(istream& in, Project& p);
    friend bool operator==(const Project& a, const Project& b);
    friend bool operator<(const Project& a, const Project& b);
    friend bool operator!=(const Project& a, const Project& b);
};

// ---------------- DERIVED CLASS 1 ----------------
class ScienceProject : public Project {
    string category;
public:
    ScienceProject() {}
    ScienceProject(int i, string t, string n, string c, string cat)
        : Project(i, t, n, c), category(cat) {}
    ScienceProject(const ScienceProject& p) : Project(p), category(p.category) {}

    char getType() const { return 'S'; }
    string getCategory() const { return category; }
    void readCategory() { category = getText("Science Category: ", 30); }
};

// ---------------- DERIVED CLASS 2 ----------------
class TechnologyProject : public Project {
    string category;
public:
    TechnologyProject() {}
    TechnologyProject(int i, string t, string n, string c, string cat)
        : Project(i, t, n, c), category(cat) {}
    TechnologyProject(const TechnologyProject& p) : Project(p), category(p.category) {}

    char getType() const { return 'T'; }
    string getCategory() const { return category; }
    void readCategory() { category = getText("Technology Area: ", 30); }
};

// ---------------- OPERATOR OVERLOADING ----------------
void showProjectDetails(const Project& p) {
    cout << p;
}

ostream& operator<<(ostream& out, const Project& p) {
    p.showCommon(out);
    out << "\nProject Type     : "
        << (p.getType() == 'S' ? "Science" : "Technology")
        << "\nCategory / Area  : " << p.getCategory() << "\n";
    return out;
}

istream& operator>>(istream& in, Project& p) {
    p.title = getText("Project Title: ", 50);
    p.studentName = getText("Student Name: ", 50);
    p.studentClass = getText("Student Class: ", 20);
    p.readCategory();
    return in;
}

bool operator==(const Project& a, const Project& b) { return a.id == b.id; }
bool operator<(const Project& a, const Project& b) { return a.id < b.id; }
bool operator!=(const Project& a, const Project& b) { return !(a == b); }

int Project::total = 0;

// ---------------- FILE AND ARRAY FUNCTIONS ----------------
Project* makeProject(Record r) {
    if (r.type == 'T')
        return new TechnologyProject(r.id, r.title, r.name, r.studentClass, r.category);
    return new ScienceProject(r.id, r.title, r.name, r.studentClass, r.category);
}

bool duplicateID(int id, Project* list[]) {
    for (int i = 0; i < Project::getTotal(); i++)
        if (static_cast<int>(*list[i]) == id) return true;
    return false;
}

void loadProjects(Project* list[]) {
    ifstream file(FILE_NAME, ios::binary);
    Record r;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(r)) && count < MAX)
        list[count++] = makeProject(r);
    Project::setTotal(count);
}

void showProjects(Project* list[]) {
    if (Project::getTotal() == 0) {
        cout << "\nNo projects available.\n";
        return;
    }
    for (int i = 0; i < Project::getTotal(); i++)
        cout << "\n--------------------\n" << *list[i];
}

void addProject(Project* list[]) {
    int count = Project::getTotal();
    if (count == MAX) { cout << "Project limit reached.\n"; return; }

    int id;
    do {
        id = getNumber("Project ID: ");
        if (id <= 0 || duplicateID(id, list))
            cout << "ID must be positive and unique.\n";
        else break;
    } while (true);

    int choice;
    do {
        cout << "1. Science Project\n2. Technology Project\n";
        choice = getNumber("Choose type: ");
    } while (choice != 1 && choice != 2);

    Project* p = (choice == 1)
        ? static_cast<Project*>(new ScienceProject(id, "", "", "", ""))
        : static_cast<Project*>(new TechnologyProject(id, "", "", "", ""));

    cout << "Enter project details:\n";
    cin >> *p;
    Record r = p->toRecord();

    ofstream file(FILE_NAME, ios::binary | ios::app);
    if (!file) { cout << "Cannot open data file.\n"; delete p; return; }
    file.write(reinterpret_cast<char*>(&r), sizeof(r));
    file.close();

    if (!file) { cout << "Save failed.\n"; delete p; return; }
    list[count] = p;
    Project::setTotal(count + 1);
    cout << "Project saved successfully.\n";
}

void searchProject() {
    int id = getNumber("Project ID to search: ");
    ifstream file(FILE_NAME, ios::binary);
    Record r;
    int position = 0;
    while (file) {
        file.seekg(static_cast<streamoff>(position) * sizeof(r), ios::beg);
        if (!file.read(reinterpret_cast<char*>(&r), sizeof(r))) break;
        if (r.id == id) {
            Project* p = makeProject(r);
            cout << *p;
            delete p;
            return;
        }
        position++;
    }
    cout << "Project not found.\n";
}

void updateProject(Project* list[]) {
    int id = getNumber("Project ID to update: ");
    fstream file(FILE_NAME, ios::binary | ios::in | ios::out);
    Record r;
    streampos position;
    bool found = false;

    while (file) {
        position = file.tellg();
        if (!file.read(reinterpret_cast<char*>(&r), sizeof(r))) break;
        if (r.id == id) { found = true; break; }
    }
    if (!found) { cout << "Project not found.\n"; return; }

    Project* changed = makeProject(r);
    cout << "\nCurrent details:\n" << *changed;
    cout << "\nEnter new details:\n";
    cin >> *changed;

    char confirm;
    cout << "Save these changes? (Y/N): ";
    cin >> confirm;
    cin.ignore(10000, '\n');

    if (confirm != 'Y' && confirm != 'y') {
        cout << "Update cancelled.\n";
        delete changed;
        return;
    }

    Record updated = changed->toRecord();
    file.clear();
    file.seekp(position);
    file.write(reinterpret_cast<char*>(&updated), sizeof(updated));
    file.flush();
    bool saved = static_cast<bool>(file);
    file.close();

    if (!saved) { cout << "Update failed.\n"; delete changed; return; }

    for (int i = 0; i < Project::getTotal(); i++) {
        if (static_cast<int>(*list[i]) == id) {
            delete list[i];
            list[i] = changed;
            changed = NULL;
            break;
        }
    }
    delete changed;
    cout << "Project updated successfully.\n";
}

void deleteProject(Project* list[]) {
    int id = getNumber("Project ID to delete: ");
    int count = Project::getTotal(), index = -1;

    for (int i = 0; i < count; i++)
        if (static_cast<int>(*list[i]) == id) { index = i; break; }

    if (index == -1) { cout << "Project not found.\n"; return; }
    cout << "\nSelected project:\n" << *list[index];

    char confirm;
    cout << "Permanently delete this project? (Y/N): ";
    cin >> confirm;
    cin.ignore(10000, '\n');
    if (confirm != 'Y' && confirm != 'y') {
        cout << "Delete cancelled.\n";
        return;
    }

    ifstream oldFile(FILE_NAME, ios::binary);
    ofstream tempFile(TEMP_NAME, ios::binary | ios::trunc);
    Record r;
    while (oldFile.read(reinterpret_cast<char*>(&r), sizeof(r)))
        if (r.id != id) tempFile.write(reinterpret_cast<char*>(&r), sizeof(r));
    oldFile.close();
    tempFile.close();

    if (!tempFile) {
        cout << "Delete failed; original file was not replaced.\n";
        remove(TEMP_NAME);
        return;
    }

    remove(BACKUP_NAME);
    if (rename(FILE_NAME, BACKUP_NAME) != 0) {
        cout << "Could not protect original file.\n";
        remove(TEMP_NAME);
        return;
    }
    if (rename(TEMP_NAME, FILE_NAME) != 0) {
        rename(BACKUP_NAME, FILE_NAME);
        cout << "Delete failed; original file restored.\n";
        return;
    }
    remove(BACKUP_NAME);

    delete list[index];
    for (int i = index; i < count - 1; i++) list[i] = list[i + 1];
    list[count - 1] = NULL;
    Project::setTotal(count - 1);
    cout << "Project deleted successfully.\n";
}

int main() {
    Project* projects[MAX] = {};
    loadProjects(projects);

    int choice;
    do {
        cout << "\n===== SCHOOL SCIENCE FAIR PROJECT EVALUATOR =====\n"
             << "1. Add Project\n2. View All Projects\n3. Search Project\n"
             << "4. Update Project\n5. Delete Project\n6. Total Projects\n0. Exit\n";

        choice = getNumber("Your choice: ");
        switch (choice) {
            case 1: addProject(projects); break;
            case 2: showProjects(projects); break;
            case 3: searchProject(); break;
            case 4: updateProject(projects); break;
            case 5: deleteProject(projects); break;
            case 6: cout << "Total Projects: " << Project::getTotal() << "\n"; break;
            case 0: cout << "Program closed. Data is saved.\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    for (int i = 0; i < Project::getTotal(); i++) delete projects[i];
    return 0;
}
