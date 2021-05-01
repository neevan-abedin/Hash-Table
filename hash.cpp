/******************************************************************************

Hash Table by Nabhan Abedin
Hash table and random student generator constructed from my student list program
5.1.2021

*******************************************************************************/

#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <ctype.h> //for uppercase conversion

using namespace std;

struct student { //struct with all the info
    char Fname[100];
    char Lname[100];
    int id;
    float gpa;
    student* next;
    student* prev;
};

//function prototypes
void addStudent(student** list, student* newStudent, int size);
void printStudent(student** list, int size);
void deleteStudent(student** list, int id, int size);
bool collision(student** list, int size);

int main() {
    //initialize variables
    bool running = true;
    char input[10];
    int randomID = 123; //track random id
    student** list = new student*[100];
    int size = 100;
    
    while (running) {
        cout << endl << "What would you like to do? ('add', 'addrand', 'print', 'delete', 'quit): ." << endl;
        cin.get(input, 10);
        cin.clear();
        cin.ignore(10000, '\n');
        if (strcmp(input, "add") == 0) {  //manually add student
          student* newStudent = new student();
          newStudent->next = NULL;
          newStudent->prev = NULL;
          cout << "Enter first name: " << endl;
          cin >> newStudent->Fname;
          cin.clear();
          cin.ignore(100,'\n');
          cout << "Enter last name: " << endl;
          cin >> newStudent->Lname;
          cin.clear();
          cin.ignore(100,'\n');
          cout << "Enter ID: " << endl;
          cin >> newStudent->id;
          cin.clear();
          cin.ignore(100,'\n');
          cout << "Enter GPA: " << endl;
          cin >> newStudent->gpa;
          cin.clear();
          cin.ignore(100, '\n');
          addStudent(list, newStudent, size);
          cout << endl << "New student added to hash table." << endl;
          if (collision(list, size)) {
            cout << endl << "Changing hash table array to double the size." << endl;
            student** temp = new student*[size];  //create temp array in case of collision
            for (int m = 0; m < size; m++) {
                temp[m] = list[m];
            }
            int newsize = 2*size;
            list = new student*[newsize]; //create new table with double the number of slots
            for (int c = 0; c < newsize; c++) {
                  list[c] = NULL;
            }
            //rehash students into new table
            for (int a = 0; a < size; a++) {
              if (temp[a] != NULL) {
                student* move = temp[a];
                if (move->next != NULL) {
                  student* nxt = move->next;
                  move->next = NULL;
                  nxt->prev = NULL;
                  addStudent(list, nxt, newsize);
                  if (nxt->next != NULL) {
                    student* dnxt = nxt->next;
                    nxt->next = NULL;
                    dnxt->prev = NULL;
                    addStudent(list, dnxt, newsize);
                  }
                }
                addStudent(list, move, newsize);
              }
            }
            delete[] temp;
            size = newsize; //update list's size
          }
        } else if (strcmp(input, "delete") == 0) {  //remove student
          cout << "Input ID of student to remove: ";
          int in;
          cin >> in;
          cin.clear();
          cin.ignore(10000, '\n');
          deleteStudent(list, in, size);
        } else if (strcmp(input, "print") == 0) {  //print list with students
          printStudent(list, size);
        } else if (strcmp(input, "addrand") == 0) {  //add random student
          int num;
          //initialize new name arrays
          char** namef = new char*[20];
          char** namel = new char*[20];
          for (int i = 0; i < 20; i++) {
            namef[i] = new char[100];
            namel[i] = new char[100];
          }
          cout << endl << "How many random students should be added?";
          cin >> num;
          cin.clear();
          cin.ignore(10000, '\n');
          ifstream myfile("firstnames.txt"); //get first names
          int linenr = 0;
          char first[100];
          while (linenr < 20) {
            myfile.getline(first, 100);
            strcpy(namef[linenr], first);
            linenr++;
          }
          ifstream myfile1("lastnames.txt"); //get last names
          char last[100];
          linenr = 0; 
          while (linenr < 20) {
            myfile1.getline(last, 100);
            strcpy(namel[linenr], last);
            linenr++;
          }
          while (num > 0) {
            student* newStudent = new student();
            //create random numbers between 0 and 19
            int randomf = rand() % 19;
            int randoml = rand() % 19;
            //assign random data
            strcpy(newStudent->Fname, namef[randomf]);
            strcpy(newStudent->Lname, namel[randoml]);
            newStudent->id = randomID;
            newStudent->gpa = (float)rand()/(RAND_MAX)*5;
            randomID = randomID + 100;
            //add new student genrated
            addStudent(list, newStudent, size);
            //balance if collision
            if (collision(list, size)) {
              cout << endl << "Changing hash table array to double the size." << endl;
              student** temp = new student*[size];  //create temp array
              for (int m = 0; m < size; m++) {
                temp[m] = list[m];
              }
              int newsize = 2*size;
              list = new student*[newsize]; //recreate list to double
              for (int c = 0; c < newsize; c++) {
                list[c] = NULL;
              }
              //add all students to new list
              for (int a = 0; a < size; a++) {
                if (temp[a] != NULL) {
                    student* move = temp[a];
                    if (move->next != NULL) {
                        student* nxt = move->next;
                        move->next = NULL;
                        nxt->prev = NULL;
                        addStudent(list, nxt, newsize);
                        if (nxt->next != NULL) {
                            student* dnxt = nxt->next;
                            nxt->next = NULL;
                            dnxt->prev = NULL;
                            addStudent(list, dnxt, newsize);
                        }
                    }
                    addStudent(list, move, newsize);
                }
              }
              delete[] temp;
              size = newsize; //update list size
            }
            num--;
          }
        }
        else if (strcmp(input, "quit") == 0) 
        {
          cout << endl << "Quitting" << endl;
          running = false;
        } else {
          cout << endl << "Invalid input. Please enter a valid command." << endl;
        }
    }
    return 0;
}

void addStudent(student** list, student* newStudent, int size) 
{
  int index = (newStudent->id) % size;
  //if list is empty, make new student
  if (list[index] == NULL) {
    //if no colision add to list
    list[index] = newStudent;
  } else {
    //if there is collision, then add as next ptr
    if ((list[index])->next == NULL) {
      //no previous collision
      (list[index])->next = newStudent;
      (list[index]->next)->prev = (list[index])->next;
    } else {
      //previous collision
      ((list[index])->next)->next = newStudent;
      (((list[index])->next)->next)->prev = ((list[index])->next)->next;
    }
  }
}

//collision
bool collision(student** list, int size) {
  bool newlist = false;  //determine is there is a need to rehash in case of collision
  int ctr = 0;
  while (newlist == false && ctr < size) {
    if (list[ctr] != NULL) {
        if ((list[ctr])->next != NULL) {
            if (((list[ctr])->next)->next != NULL) {
                newlist = true;
            }
        }
    }
    ctr++;
  }
  return newlist;
}

void printStudent(student** list, int size) { //print list of students
  for (int i = 0; i < size; i++) {
    student* curr = list[i];
    if (curr != NULL) {
        cout << curr->Fname << " ";
        cout << curr->Lname;
        cout << " #" << curr->id << " GPA:";
        cout << fixed << setprecision(2) << curr->gpa;
        student* nxt = curr->next;
        if (nxt != NULL) {
            cout << '\t' << nxt->Fname << " ";
            cout << nxt->Lname;
            cout << "#" << nxt->id << " GPA:";
            cout << fixed << setprecision(2) << nxt->gpa;
        }
        cout << endl;
    }
  }
}

void deleteStudent(student** list, int id, int size) {
  int i = id % size;
  if (list[i] == NULL) {
    cout << endl << "Student does not exist." << endl;
  } else {
        if (list[i]->id == id) {
            //if id is valid, remove from list
            if (list[i]->next == NULL) {
                list[i] = NULL; 
            } else {
                //replace with next in collision
                student* newcurr = list[i]->next;
                newcurr->prev = NULL;
                list[i] = newcurr;
            }
        } else {
            if (list[i]->next == NULL) {
                cout << endl << "Student does not exist." << endl;
            } else {
                if (list[i]->next->id == id) {
                    //if id is valid, then remove
                    list[i]->next = NULL;
                } else {
                    cout << "Student does not exist." << endl;
                }
            }
        }
  }
}

