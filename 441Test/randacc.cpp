// Rachel Crosley
// CS 321
// Assignment 4
// 3/28/18
// randacc.cpp

#include <iostream>
#include <sys/mman.h>
#include <string>
#include <cstddef>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>
using std::size_t;
using std::cout;
using std::cin;
using std::endl;
using std::string;



//readFunc()
//reads from a file at specified index and prints it
void readFunc(void * &addr, size_t len) {
    int index = 0;
    while (true) {
        cout << "Please enter a record index (0 - 99): " << endl;
        cin >> index;
        if (index < 0 || index > 100) {
            cout << "Not valid index. Please choose a valid index. " << endl;
        }
        else break;
    }
    char * p = (char *) addr;
    cout << p[index];
    cout << endl << endl;
}


//writeFunc()
//writes specified item to specified index in file
void writeFunc(void * &addr, size_t len) {
    string recordThis = "";
    int index = 0;
    while (true) {
        cout << "Please enter a record index (0 - 99): " << endl;
        cin >> index;
        cout << "Please enter a value to be recorded (up to " << len << " chars): " << endl;
        cin >> recordThis;
        recordThis.resize(len, '-');
        if (index < 0 || index > 100) {
            cout << "Not valid index. Please choose a valid index. " << endl;
        }
        else break;
    }
    char * p = (char *) addr;
    for (int i = 0; i < len; ++i) {
        p[(index*len)+i] = recordThis[i];
    }
    cout << recordThis << " writen to file at " << index << endl;
    cout << endl;
}



int main() {
    string filepath = "abc.txt";
    const size_t record_length = 6;
    const size_t SIZE = 100 * record_length;
    char menu = 'a';
    std::ifstream f(filepath.c_str());
    bool fileExists = f.good();
    

    cout << "Opening file " << filepath << endl;
    int fileOpen = open (filepath.c_str(), O_RDWR | O_CREAT, 0644);
    if (fileOpen == -1) {
        cout << "Could not open file " << filepath << endl;
        return 0;
    }
    else cout << "File open" << endl;
    if(!fileExists) {
        for(std::size_t i=0; i<SIZE; ++i)
        {
            write(fileOpen, "-", 1);
            
        }
        write(fileOpen, "\n", 1);
    }
    
    void * addr = mmap(nullptr, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fileOpen, 0);
    
    if(addr == MAP_FAILED) {
        cout << "mmap failed " << endl;
        int close_fail = close(fileOpen);
        if (close_fail) {
            cout << "Failed to close file" << endl;
        }
        else cout << "File Closed " << endl;
        return 0;
    }
    
    while (true) {
        cout << "Would you like to read a record, write a record, or exit (r, w, or e)? " << endl;
        cin >> menu;
        if(menu == 'r') {
            readFunc(addr, record_length);
        }
        else if (menu == 'w') {
            writeFunc(addr, record_length);
        }
        else if (menu == 'e') {
            int close_fail = close(fileOpen);
            if (close_fail) {
                cout << "Failed to close file" << endl;
            }
            else cout << "File Closed " << endl;
            munmap(addr, SIZE);
            return 0;
        }
        else cout << "No valid option selected. Please choose a valid option. " << endl;
        
    }
    
    return 0;
}











