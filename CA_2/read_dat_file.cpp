
#include <iostream>
#include <fstream>
using namespace std;

const int MAX = 20;

struct Record {
    int id;
    char title[50];
    char name[50];
    char cls[20];
    char category[30];
};

int main() {

    ifstream file("project.dat", ios::binary);

    if (!file) {
        cout << "Unable to open project.dat file!";
        return 1;
    }

    Record r;
    int count = 0;

    cout << "\nSAVED PROJECT RECORDS\n";

    while (file.read((char*)&r, sizeof(Record))) {

        count++;

        cout << "\nRecord Number: " << count << endl;
        cout << "Project ID       : " << r.id << endl;
        cout << "Project Title    : " << r.title << endl;
        cout << "Student Name     : " << r.name << endl;
        cout << "Student Class    : " << r.cls << endl;
        cout << "Science Category : " << r.category << endl;

        cout << "--------------------------------\n";
    }

    if (count == 0) {
        cout << "No records found!\n";
    }

    cout << "\nTotal Records: " << count << endl;

    file.close();

    return 0;
}
