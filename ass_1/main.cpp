#include <iostream>
#include<fstream>
#include<bits/stdc++.h>
using namespace std;
typedef struct Employee
{
    char Employee_Id[13];
    char Dept_Id[30];
    char Employee_Name[50];
    char Employee_Position[50];
};
typedef struct Department
{
    char Dept_Id[30];
    char Dept_Name[50];
    char Dept_Manager[50];
};
typedef struct Employee_PI
{
    char Employee_Id[13];
    int offset;
};
typedef struct Department_PI
{
    char Dept_Id[30];
    int offset;
};
typedef struct Node
{
    int item;
    Node* next = NULL;
};
class LinkedList
{
public:
    Node* Head, * Tail;
    int length = 0;
    LinkedList() : Head(NULL), Tail(NULL), length(0) {}
    bool isEmpty()
    {
        return length == 0;
    }
    void Display()
    {
        Node* cur = Head;
        cout << "[";
        while (cur != NULL)
        {
            cout << cur->item << " ";
            cur = cur->next;
        }
        cout << "]";
    }
    void write(fstream& f)
    {
        Node* cur = Head;
        while (cur != NULL)
        {
            f << cur->item << '|';
            cur = cur->next;
        }
        f << '$';
    }
    void insertNode(int element)
    {
        Node* newNode;
        newNode = new Node;
        newNode->item = element;

        if (length == 0)
        {
            Head = Tail = newNode;
            newNode->next = NULL;
        }
        else
        {
            newNode->next = NULL;
            Tail->next = newNode;
            Tail = newNode;
        }
        length++;
    }
};
typedef struct Sorting
{
    int Id;
    int offset;
};
typedef struct Department_SI
{
    char Dept_Name[50];
    LinkedList id;
};
typedef struct Employee_SI
{
    char Dept_ID[30];
    LinkedList EmployeeId;
};
vector<string> split(string stringWord, char delimiter)
{
    string word = "";
    vector<string> queryVector;
    int num = 0;
    int l = stringWord.size();
    for (int i = 0; i < l; i++)
    {
        if (stringWord[i] != delimiter)
            word = word + stringWord[i];
        else
        {
            queryVector.push_back(word);
            word = "";
        }
    }
    return queryVector;
}
int binarySearch(vector<Sorting>vec, int l, int r, int x)
{
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (vec[m].Id == x)
            return vec[m].offset;
        if (vec[m].Id < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return -1;
}
bool mySort(Sorting depart1, Sorting depart2)
{
    return (depart1.Id < depart2.Id);
}
bool sortDSI(Department_SI depart1, Department_SI depart2)
{
    return (depart1.Dept_Name[0] < depart2.Dept_Name[0]);
}
bool sortESI(Employee_SI depart1, Employee_SI depart2)
{
    int num1, num2;
    stringstream geek(depart1.Dept_ID);
    geek >> num1;
    stringstream geek1(depart2.Dept_ID);
    geek1 >> num2;
    return (num1 < num2);
}
bool operator ==(Sorting depart1, Sorting depart2)
{
    if (depart1.Id == depart2.Id)
        return true;
    else
        return false;
}
bool operator ==(Department_SI depart1, Department_SI depart2)
{
    if (string(depart1.Dept_Name) == string(depart2.Dept_Name))
        return true;
    else
        return false;

}
bool operator ==(Employee_SI depart1, Employee_SI depart2)
{
    if (string(depart1.Dept_ID) == string(depart2.Dept_ID))
        return true;
    else
        return false;
}
void getEmployeeById(int id, bool state)
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f;
    f.open("EmployeePrimaryIndex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f.close();
    string output;
    int offset = binarySearch(myVector, 0, myVector.size() - 1, id);
    f.open("Employee.txt", ios::in | ios::out);
    f.seekp(offset, ios::beg);
    char c;
    f.get(c);
    if (offset == -1 || c == '*')
    {
        cout << "not found" << endl;
    }
    else
    {
        if (state)
        {
            getline(f, output, '|');
            output = "";
            getline(f, output, '|');
            cout << "EmployeeName: " << output << endl;
            output = "";
        }
        else
        {
            getline(f, output, '|');
            cout << "EmployeeID: " << output << endl;
            output = "";
            getline(f, output, '|');
            cout << "EmployeeName: " << output << endl;
            output = "";
            getline(f, output, '|');
            cout << "EmployeePosition: " << output << endl;
            output = "";
            getline(f, output, '|');
            cout << "EmployeeDepartmentId: " << output << endl;
            output = "";
        }
    }
    f.close();
}
void insertIntoESI(Employee_SI secondaryIndexObj, string departId, int employeeId)
{
    fstream f3;
    f3.open("EmployeeSIndex.txt", ios::app | ios::in | ios::out);
    f3.seekg(0, ios::end);
    int length = f3.tellg();
    if (length == 0)
    {
        f3 << departId << '|';
        secondaryIndexObj.EmployeeId.write(f3);
    }
    else
    {
        f3.seekp(0, ios::beg);
        vector<Employee_SI> Vector;
        if (f3.is_open())
        {
            string s;
            while (!f3.eof())
            {
                Employee_SI index;
                s = "";
                getline(f3, s, '$');
                vector<string> record = split(s, '|');
                for (int i = 0; i < record.size(); i++)
                {
                    if (i == 0)
                        strcpy(index.Dept_ID, record[0].c_str());
                    else
                    {
                        stringstream geek(record[i]);
                        int newId;
                        geek >> newId;
                        index.EmployeeId.insertNode(newId);
                    }
                }
                Vector.push_back(index);
            }
        }
        bool exist = true;
        for (int i = 0; i < Vector.size(); i++)
        {
            string n = string(Vector[i].Dept_ID);
            if (n == departId)
            {
                Vector[i].EmployeeId.insertNode(employeeId);
                exist = false;
                break;
            }
        }
        if (exist)
        {
            Vector.push_back(secondaryIndexObj);
        }
        f3.close();
        Vector.erase(unique(Vector.begin(), Vector.end()), Vector.end());
        sort(Vector.begin(), Vector.end(), sortESI);
        f3.open("EmployeeSIndex.txt", fstream::out | fstream::trunc);
        for (int i = 0; i < Vector.size(); i++)
        {
            f3 << Vector[i].Dept_ID << '|';
            Vector[i].EmployeeId.write(f3);
        }
        f3.close();
    }
}
void insertIntoDSI(Department_SI secondaryIndexObj, string name, int id)
{
    fstream f3;
    f3.open("DepartmentSIndex.txt", ios::app | ios::in | ios::out);
    f3.seekg(0, ios::end);
    int length = f3.tellg();
    if (length == 0)
    {
        f3 << name << '|';
        secondaryIndexObj.id.write(f3);
    }
    else
    {
        f3.seekp(0, ios::beg);
        vector<Department_SI> Vector;
        if (f3.is_open())
        {
            string s;
            while (!f3.eof())
            {
                Department_SI index;
                s = "";
                getline(f3, s, '$');
                vector<string> record = split(s, '|');
                for (int i = 0; i < record.size(); i++)
                {
                    if (i == 0)
                        strcpy(index.Dept_Name, record[0].c_str());
                    else
                    {
                        stringstream geek(record[i]);
                        int newId;
                        geek >> newId;
                        index.id.insertNode(newId);
                    }
                }
                Vector.push_back(index);
            }
        }
        bool exist = true;
        for (int i = 0; i < Vector.size(); i++)
        {
            string n = string(Vector[i].Dept_Name);
            if (n == name)
            {
                Vector[i].id.insertNode(id);
                exist = false;
                break;
            }
        }
        cout << exist;
        if (exist)
        {
            Vector.push_back(secondaryIndexObj);
        }
        f3.close();
        Vector.erase(unique(Vector.begin(), Vector.end()), Vector.end());
        sort(Vector.begin(), Vector.end(), sortDSI);
        f3.open("DepartmentSIndex.txt", fstream::out | fstream::trunc);
        for (int i = 0; i < Vector.size(); i++)
        {
            f3 << Vector[i].Dept_Name << '|';
            Vector[i].id.write(f3);
        }
        f3.close();
    }
}
void sortForDepartmentIndex()
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f, f2;
    f.open("DepartmentPindex.txt", ios::in | ios::out);
    f2.open("DepartmentPindex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f2.close();
    f2.open("DepartmentPindex.txt", fstream::out | fstream::trunc);
    for (int i = 0; i < myVector.size(); i++)
    {
        f2 << myVector[i].Id << '|' << myVector[i].offset << '&';
    }
    f2.close();
    f.close();
}
void insertDepartment()
{
    fstream f, f2, f3, f4;
    f.open("Department.txt", ios::app | ios::in | ios::out);
    Department d;
    cout << "enter DepartmentId" << endl;
    cin.ignore();
    cin.getline(d.Dept_Id, 30);
    cout << "enter DepartmentName" << endl;
    cin.getline(d.Dept_Name, 50);
    cout << "enter DepartmentManager" << endl;
    cin.getline(d.Dept_Manager, 50);
    Department_SI secondaryIndexObj;
    strcpy(secondaryIndexObj.Dept_Name, d.Dept_Name);
    secondaryIndexObj.id.insertNode(atoi(d.Dept_Id));
    string name = string(secondaryIndexObj.Dept_Name);
    int id = atoi(d.Dept_Id);
    char buffer[150];
    strcpy(buffer, d.Dept_Id);
    strcat(buffer, "|");
    strcat(buffer, d.Dept_Name);
    strcat(buffer, "|");
    strcat(buffer, d.Dept_Manager);
    strcat(buffer, "|");
    strcat(buffer, "#");
    int len = strlen(buffer) + 1;
    f << len << '|';
    f << buffer;
    f.flush();
    Department_PI dIndex;
    strcpy(dIndex.Dept_Id, d.Dept_Id);
    f.seekp(0, ios::end);
    int position = f.tellp();
    char buffer2[20];
    f2.open("DepartmentPindex.txt", ios::app | ios::in | ios::out);
    f2.write(dIndex.Dept_Id, strlen(d.Dept_Id));
    strcpy(buffer2, dIndex.Dept_Id);
    string a, y;
    int x = position - len;
    f2 << '|' << x << '&';
    stringstream ss;
    ss << x;
    string str1 = ss.str();
    a += y.append(dIndex.Dept_Id) + '|' + str1 + '&';
    f.close();
    f2.close();
    sortForDepartmentIndex();
    insertIntoDSI(secondaryIndexObj, name, id);
}
void getDepartmentById(int id)
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f;
    f.open("DepartmentPindex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f.close();
    string output;
    int offset = binarySearch(myVector, 0, myVector.size() - 1, id);
    f.open("Department.txt", ios::in | ios::out);
    f.seekp(offset, ios::beg);
    char c;
    f.get(c);
    if (offset == -1 || c == '*')
    {
        cout << "not found" << endl;
    }
    else
    {
        getline(f, output, '|');
        cout << "DepartmentID: " << output << endl;
        output = "";
        getline(f, output, '|');
        cout << "DepartmentName: " << output << endl;
        output = "";
        getline(f, output, '|');
        cout << "DepartmentManager: " << output << endl;
        output = "";
    }
    f.close();
}
void searchByEmployeeDepartment(int departId)
{
    vector<Employee_SI> Vector;
    fstream f3;
    f3.open("EmployeeSIndex.txt", ios::app | ios::in | ios::out);
    if (f3.is_open())
    {
        string s;
        while (!f3.eof())
        {
            Employee_SI index;
            s = "";
            getline(f3, s, '$');
            vector<string> record = split(s, '|');
            for (int i = 0; i < record.size(); i++)
            {
                if (i == 0)
                    strcpy(index.Dept_ID, record[0].c_str());
                else
                {
                    stringstream geek(record[i]);
                    int newId;
                    geek >> newId;
                    index.EmployeeId.insertNode(newId);
                }
            }
            Vector.push_back(index);
        }
    }
    int l = 0;
    int r = Vector.size() - 1;
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (atoi(Vector[m].Dept_ID) == departId)
        {
            Node* current = Vector[m].EmployeeId.Head;
            while (current != NULL)
            {
                getEmployeeById(current->item, false);
                cout << endl;
                current = current->next;
            }
        }
        int num1, num2;
        stringstream geek(Vector[m].Dept_ID);
        geek >> num1;
        if (num1 < departId)
            l = m + 1;
        else
            r = m - 1;
    }
}
void deletEmployeeById(int id)
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f;
    f.open("EmployeePrimaryIndex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f.close();
    string output;
    int offset = binarySearch(myVector, 0, myVector.size() - 1, id);
    if (offset == -1)
    {
        cout << "not found" << endl;
    }
    else
    {
        f.open("Employee.txt", ios::in | ios::out);
        f.seekp(offset + 1, ios::beg);
        f << '*';
        f.close();
        f.open("EmployeePrimaryIndex.txt", fstream::out | fstream::trunc);
        for (int i = 0; i < myVector.size(); i++)
        {
            if (myVector[i].Id == id)
            {
                continue;
            }
            f << myVector[i].Id << '|' << myVector[i].offset << '&';
        }
        f.close();
        cout << "Employee deleted successfully" << endl;
    }
}
void sortForEmployeeIndex()
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f, f2;
    f.open("EmployeePrimaryIndex.txt", ios::in | ios::out);
    f2.open("EmployeePrimaryIndex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f2.close();
    f2.open("EmployeePrimaryIndex.txt", fstream::out | fstream::trunc);
    for (int i = 0; i < myVector.size(); i++)
    {
        f2 << myVector[i].Id << '|' << myVector[i].offset << '&';
    }
    f2.close();
    f.close();
}
void insertEmployee()
{
    fstream f, f2;
    f.open("Employee.txt", ios::app | ios::in | ios::out);
    Employee employee;
    cout << "enter EmployeeId" << endl;
    cin.ignore();
    cin.getline(employee.Employee_Id, 13);
    cout << "enter EmployeeName" << endl;
    cin.getline(employee.Employee_Name, 50);
    cout << "enter EmployeePosition" << endl;
    cin.getline(employee.Employee_Position, 50);
    cout << "enter EmployeeDepartmentId" << endl;
    cin.getline(employee.Dept_Id, 30);
    char buffer[150];
    strcpy(buffer, employee.Employee_Id);
    strcat(buffer, "|");
    strcat(buffer, employee.Employee_Name);
    strcat(buffer, "|");
    strcat(buffer, employee.Employee_Position);
    strcat(buffer, "|");
    strcat(buffer, employee.Dept_Id);
    strcat(buffer, "|");
    strcat(buffer, "#");
    int len = strlen(buffer) + 1;
    f << len << '|';
    f << buffer;
    f.flush();
    Employee_PI eIndex;
    strcpy(eIndex.Employee_Id, employee.Employee_Id);
    f.seekp(0, ios::end);
    int position = f.tellp();
    char buffer2[20];
    f2.open("EmployeePrimaryIndex.txt", ios::app | ios::in | ios::out);
    f2.write(eIndex.Employee_Id, strlen(employee.Employee_Id));
    strcpy(buffer2, eIndex.Employee_Id);
    string a, y;
    int x = position - len;
    f2 << '|' << x << '&';
    f.close();
    f2.close();
    sortForEmployeeIndex();
    string departId;
    int employeeId;
    Employee_SI secondaryIndexObj;
    strcpy(secondaryIndexObj.Dept_ID, employee.Dept_Id);
    secondaryIndexObj.EmployeeId.insertNode(atoi(employee.Employee_Id));
    departId = string(secondaryIndexObj.Dept_ID);
    employeeId = atoi(employee.Employee_Id);
    insertIntoESI(secondaryIndexObj, departId, employeeId);
}
void deletDepartmentById(int id)
{
    string idVector = "", offsetVector = "";
    vector<Sorting> myVector;
    Sorting index;
    fstream f;
    f.open("DepartmentPindex.txt", ios::in | ios::out);
    if (f.is_open())
    {
        while (!f.eof())
        {
            getline(f, idVector, '|');
            stringstream geek(idVector);
            geek >> index.Id;
            getline(f, offsetVector, '&');
            stringstream geek1(offsetVector);
            geek1 >> index.offset;
            myVector.insert(myVector.begin(), index);
            sort(myVector.begin(), myVector.end(), mySort);
            myVector.erase(unique(myVector.begin(), myVector.end()), myVector.end());
        }
    }
    f.close();
    string output;
    int offset = binarySearch(myVector, 0, myVector.size() - 1, id);
    if (offset == -1)
    {
        cout << "not found" << endl;
    }
    else
    {
        f.open("Department.txt", ios::in | ios::out);
        f.seekp(offset + 1, ios::beg);
        f << '*';
        f.close();
        f.open("DepartmentPindex.txt", fstream::out | fstream::trunc);
        for (int i = 0; i < myVector.size(); i++)
        {
            if (myVector[i].Id == id)
            {
                continue;
            }
            f << myVector[i].Id << '|' << myVector[i].offset << '&';
        }
        f.close();
        cout << "Department deleted successfully" << endl;
    }
}
void displayQueryResult(string wantedTubles, string wantedTable, string Attribute, string condition, string value)
{
    if (wantedTable == "Employee")
    {
        if (wantedTubles == "all")
        {
            if (Attribute == "id") {
                stringstream geek(value);
                int employeeId;
                geek >> employeeId;
                getEmployeeById(employeeId , false);
            }
            else if (Attribute == "Dept_ID")
            {
                int x = 0;
                stringstream geek(value);
                geek >> x;
                searchByEmployeeDepartment(x);
            }
        }
        else
        {
            // if user want to display name column only
            if (wantedTubles == "Employee_Name")
            {
                if (Attribute == "Employee_ID")
                {
                    int x = 0;
                    stringstream geek(value);
                    geek >> x;
                    getEmployeeById(x, true);
                }
            }
        }
    }
    else if (wantedTable == "Department")
    {
        if (wantedTubles == "all")
        {
            if (Attribute == "Dept_ID") {
                stringstream geek(value);
                int departmentId;
                geek >> departmentId;
                getDepartmentById(departmentId);
            }
        }
    }
}
void searchByDepartmentName(string dName)
{
    vector<Department_SI> Vector;
    fstream f3;
    f3.open("DepartmentSIndex.txt", ios::app | ios::in | ios::out);
    if (f3.is_open())
    {
        string s;
        while (!f3.eof())
        {
            Department_SI index;
            s = "";
            getline(f3, s, '$');
            vector<string> record = split(s, '|');
            for (int i = 0; i < record.size(); i++)
            {
                if (i == 0)
                    strcpy(index.Dept_Name, record[0].c_str());
                else
                {
                    stringstream geek(record[i]);
                    int newId;
                    geek >> newId;
                    index.id.insertNode(newId);
                }
            }
            Vector.push_back(index);
        }
    }
    int l = 0;
    int r = Vector.size() - 1;
    while (l <= r)
    {
        int m = l + (r - l) / 2;
        if (string(Vector[m].Dept_Name) == dName)
        {
            Node* current = Vector[m].id.Head;
            while (current != NULL)
            {
                getDepartmentById(current->item);
                cout << endl;
                current = current->next;
            }
        }
        if (Vector[m].Dept_Name[0] < dName[0])
            l = m + 1;
        else
            r = m - 1;
    }
}
int main()
{
    int x = 0 ;
    while (true)
    {
        cout << " 1- Add New Employee " << endl;
        cout << " 2- Add New Department " << endl;
        cout << " 3- Delete Employee (ID) " << endl;
        cout << " 4- Delete Department (ID) " << endl;
        cout << " 5- Print Employee (ID) " << endl;
        cout << " 6- Print Employee (Dept_ID)  " << endl;
        cout << " 7- Print Department (ID) " << endl;
        cout << " 8- Print Department (Name) " << endl;
        cout << " 9- Write A Query " << endl;
        cout << " 10- Exit " << endl;
        cout<<"Enter number"<<endl;
        cin >> x;
        if (x == 1)
        {
            insertEmployee();
            continue;
        }
        else if (x == 2)
        {
            insertDepartment();
            continue;
        }
        else if (x == 3)
        {
            cout << "enter Employee id" << endl;
            int id;
            cin >> id;
            deletEmployeeById(id);
            continue;
        }
        else if (x == 4)
        {
            cout << "enter department id" << endl;
            int id;
            cin >> id;
            deletDepartmentById(id);
            continue;
        }
        else if (x == 5)
        {
            int idNum;
            bool state = false;
            cout << "Enter the Employee id" << endl;
            cin >> idNum;
            getEmployeeById(idNum, state);
            continue;
        }
        else if (x == 6)
        {
            int departId;
            cout << "Enter the department id" << endl;
            cin >> departId;
            searchByEmployeeDepartment(departId);
            continue;
        }
        else if (x == 7)
        {
            int idNum;
            cout << "Enter the department id" << endl;
            cin >> idNum;
            getDepartmentById(idNum);
            continue;
        }
        else if (x == 8)
        {
            string n;
            cout << "Enter the department name" << endl;
            cin >> n;
            searchByDepartmentName(n);
            continue;
        }
        else if (x == 9)
        {
            // select all from Empolyee where id = 20200076
            vector <string> myQuery;
            string wantedTubles, wantedTable, wantedAttribute, condition, value, query, getquery;
            cout << "Enter your query " << endl;
            for (int i = 0;i < 8;i++)
            {
                cin >> getquery;
                query += getquery + " ";
            }
            myQuery = split(query, ' ');
            wantedTubles = myQuery[1];
            wantedTable = myQuery[3];
            wantedAttribute = myQuery[5];
            condition = myQuery[6];
            value = myQuery[7];
            displayQueryResult(wantedTubles, wantedTable, wantedAttribute, condition, value);
            continue;
        }
        else if (x == 10)
        {
            return 0;
        }
        else
        {
            cout << "Non-Valid Number Enter a valid one." << endl;
            continue;
        }
    }
    return 0;
}
