
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <ctime>
#include <conio.h> 
#include <limits>   

using namespace std;

class Citizen {
private:
    char name[50];
    char aadhar[13];
    char address[100];
    char phone[15];  
    char gender[10];
    char dob[12];    
    bool isActive; 

public:
   
    Citizen() { memset(this, 0, sizeof(Citizen)); }

  
    const char* getName() const { return name; }
    const char* getAadhar() const { return aadhar; }
    const char* getAddress() const { return address; }
    const char* getPhone() const { return phone; }
    const char* getGender() const { return gender; }
    const char* getDOB() const { return dob; }
    bool getIsActive() const { return isActive; }

    
    void setName(const char* n) { strncpy(name, n, 49); name[49] = '\0'; }
    void setAadhar(const char* a) { strncpy(aadhar, a, 12); aadhar[12] = '\0'; }
    void setAddress(const char* addr) { strncpy(address, addr, 99); address[99] = '\0'; }
    void setPhone(const char* p) { strncpy(phone, p, 14); phone[14] = '\0'; }
    void setGender(const char* g) { strncpy(gender, g, 9); gender[9] = '\0'; }
    void setDOB(const char* d) { strncpy(dob, d, 11); dob[11] = '\0'; }
    void setIsActive(bool active) { isActive = active; }
};

const char* FILENAME = "citizens.dat";

bool isValidAadhar(const char aadhar[]) {
    if (strlen(aadhar) != 12) return false;
    for (int i = 0; i < 12; i++) {
        if (!isdigit(aadhar[i])) return false; 
    }
    return true;
}

bool isValidName(const char name[]) {
    if (strlen(name) == 0) return false;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ')
            return false;
    }
    return true;
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDOB(const char dob[]) {
   
    if (strlen(dob) != 10 || dob[2] != '-' || dob[5] != '-') return false;

  
    int day = (dob[0] - '0') * 10 + (dob[1] - '0');
    int month = (dob[3] - '0') * 10 + (dob[4] - '0');
    int year = (dob[6] - '0') * 1000 + (dob[7] - '0') * 100 + (dob[8] - '0') * 10 + (dob[9] - '0');

    
    if (month < 1 || month > 12) return false;
    if (day < 1) return false;

    
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) daysInMonth[1] = 29; 
    if (day > daysInMonth[month - 1]) return false; 

    
    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    
    if (year > currentYear || (year == currentYear && month > currentMonth) ||
        (year == currentYear && month == currentMonth && day > currentDay))
        return false;

    return true;
}

void addCitizen() {
    Citizen c;
    char name[50], aadhar[13], address[100], phone[15], gender[10], dob[12];

    bool isNameValid;
    do {
        isNameValid = true;
        cout << "\nEnter Name: ";
      
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.getline(name, 50);
        if (!isValidName(name)) {
            cout << " Invalid name! Only alphabets and spaces allowed.\n";
            isNameValid = false;
        }
    } while (!isNameValid);
    c.setName(name);

    do {
        cout << "Enter Aadhar Number (12 digits): ";
        cin.getline(aadhar, 13);
        if (!isValidAadhar(aadhar)) {
            cout << " Invalid Aadhar! Must be exactly 12 digits and contain only numbers.\n";
        }
    } while (!isValidAadhar(aadhar));
    c.setAadhar(aadhar);

  
    Citizen temp;
    bool isDuplicate = false;
    ifstream inCheck(FILENAME, ios::binary);
    if (inCheck.is_open()) {
        while (inCheck.read(reinterpret_cast<char*>(&temp), sizeof(Citizen))) {
           
            if (strcmp(temp.getAadhar(), c.getAadhar()) == 0 && temp.getIsActive()) {
                isDuplicate = true;
                break;
            }
        }
        inCheck.close();
    }

    if (isDuplicate) {
        cout << "\n Aadhar number already exists and is active!\n";
        return; 
    }

    do {
        cout << "Enter Address: ";
        cin.getline(address, 100);
        if (strlen(address) == 0) {
            cout << " Address cannot be empty.\n";
        }
    } while (strlen(address) == 0);
    c.setAddress(address);

    bool isPhoneValid;
    do {
        isPhoneValid = true;
        cout << "Enter Phone : ";
        cin.getline(phone, 15);
        int len = strlen(phone);

        if (len != 10) {
            cout << " Phone number must be exactly 10 digits.\n";
            isPhoneValid = false;
        } else if (phone[0] != '6' && phone[0] != '7' && phone[0] != '8' && phone[0] != '9') {
            cout << " Phone number must start with 6, 7, 8, or 9.\n";
            isPhoneValid = false;
        } else {
            for (int i = 0; i < len; i++) {
                if (!isdigit(phone[i])) {
                    cout << " Phone must contain only digits.\n";
                    isPhoneValid = false;
                    break;
                }
            }
        }
    } while (!isPhoneValid);
    c.setPhone(phone);

    do {
        cout << "Enter Gender (Male/Female/Other): ";
        cin.getline(gender, 10);
        string genderStr(gender);
        for (char& ch : genderStr) ch = tolower(ch); 
        if (genderStr != "male" && genderStr != "female" && genderStr != "other") {
            cout << " Invalid Gender! Choose Male, Female, or Other.\n";
        } else {
            c.setGender(gender);
            break;
        }
    } while (true);

    do {
        cout << "Enter Date of Birth (DD-MM-YYYY): ";
        cin.getline(dob, 12);
        if (!isValidDOB(dob)) {
            cout << " Invalid DOB! Make sure it's a real date in DD-MM-YYYY format and in the past.\n";
        } else {
            c.setDOB(dob);
            break;
        }
    } while (true);

    c.setIsActive(true); 

   
    ofstream out(FILENAME, ios::binary | ios::app);
    if (out.is_open()) {
        out.write(reinterpret_cast<char*>(&c), sizeof(Citizen));
        out.close();
        cout << "\n Citizen added successfully.\n";
    } else {
        cout << "Error: Could not open data file for writing.\n";
    }
}

void displayAll() {
    Citizen c;
    ifstream in(FILENAME, ios::binary);
    if (!in.is_open()) {
        cout << "\n No records found or file cannot be opened.\n";
        return;
    }

    
    cout << left << setw(15) << "\nAadhar" << setw(20) << "Name" << setw(15)
         << "Phone" << setw(10) << "Gender" << setw(15) << "DOB" << setw(30) << "Address" << endl;
    cout << string(100, '-') << endl; // Separator line

    bool foundAny = false;
    while (in.read(reinterpret_cast<char*>(&c), sizeof(Citizen))) {
        if (c.getIsActive()) { 
            cout << left << setw(15) << c.getAadhar() << setw(20) << c.getName() << setw(15)
                 << c.getPhone() << setw(10) << c.getGender() << setw(15) << c.getDOB() << setw(30) << c.getAddress() << endl;
            foundAny = true;
        }
    }
    in.close();

    if (!foundAny) {
        cout << "\n No active citizen records to display.\n";
    }
}

void searchCitizen() {
    char aad[13];
    cout << "\nEnter Aadhar Number to search: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.getline(aad, 13);

    Citizen c;
    bool found = false;

    ifstream in(FILENAME, ios::binary);
    if (!in.is_open()) {
        cout << "\n No records found or file cannot be opened.\n";
        return;
    }

    while (in.read(reinterpret_cast<char*>(&c), sizeof(Citizen))) {
        if (strcmp(c.getAadhar(), aad) == 0 && c.getIsActive()) { 
            cout << "\n Citizen Found:\n";
            cout << "Name: " << c.getName() << "\nAddress: " << c.getAddress() << "\nPhone: " << c.getPhone()
                 << "\nGender: " << c.getGender() << "\nDOB: " << c.getDOB() << endl;
            found = true;
            break; 
        }
    }
    in.close();
    if (!found) cout << "\n Citizen not found or is not active.\n";
}

void updateCitizen() {
    char aad[13];
    cout << "\nEnter Aadhar Number of citizen to update: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(aad, 13);

    Citizen c;
    bool found = false;

   
    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "\n No records found or file cannot be opened for update.\n";
        return;
    }

   
    while (file.read(reinterpret_cast<char*>(&c), sizeof(Citizen))) {
        if (strcmp(c.getAadhar(), aad) == 0 && c.getIsActive()) { 
            cout << "\n Current Details:\n";
            cout << "Name: " << c.getName() << "\nAddress: " << c.getAddress() << "\nPhone: " << c.getPhone()
                 << "\nGender: " << c.getGender() << "\nDOB: " << c.getDOB() << endl;

            cout << "\nEnter New Details:\n";
            char name[50], address[100], phone[15], gender[10], dob[12];

            bool isNameValid;
            do {
                isNameValid = true;
                cout << "Enter Name: ";
                cin.getline(name, 50);
                if (!isValidName(name)) {
                    cout << " Invalid name! Only alphabets and spaces allowed.\n";
                    isNameValid = false;
                }
            } while (!isNameValid);
            c.setName(name);

            do {
                cout << "Enter Address: ";
                cin.getline(address, 100);
                if (strlen(address) == 0)
                    cout << " Address cannot be empty.\n";
            } while (strlen(address) == 0);
            c.setAddress(address);

            bool isPhoneValid;
            do {
                isPhoneValid = true;
                cout << "Enter Phone (10 digits, starts with 6,7,8,9): ";
                cin.getline(phone, 15);
                int len = strlen(phone);
                if (len != 10) {
                    cout << " Phone number must be exactly 10 digits.\n";
                    isPhoneValid = false;
                } else if (phone[0] != '6' && phone[0] != '7' && phone[0] != '8' && phone[0] != '9') {
                    cout << " Phone number must start with 6, 7, 8, or 9.\n";
                    isPhoneValid = false;
                } else {
                    for (int i = 0; i < len; i++) {
                        if (!isdigit(phone[i])) {
                            cout << " Phone must contain only digits.\n";
                            isPhoneValid = false;
                            break;
                        }
                    }
                }
            } while (!isPhoneValid);
            c.setPhone(phone);

            do {
                cout << "Enter Gender (Male/Female/Other): ";
                cin.getline(gender, 10);
                string genderStr(gender);
                for (char& ch : genderStr) ch = tolower(ch);
                if (genderStr != "male" && genderStr != "female" && genderStr != "other") {
                    cout << " Invalid Gender! Choose Male, Female, or Other.\n";
                } else {
                    c.setGender(gender);
                    break;
                }
            } while (true);

            do {
                cout << "Enter Date of Birth (DD-MM-YYYY): ";
                cin.getline(dob, 12);
                if (!isValidDOB(dob)) {
                    cout << " Invalid DOB! Use DD-MM-YYYY and a valid date in the past.\n";
                } else {
                    c.setDOB(dob);
                    break;
                }
            } while (true);

           
            int pos = file.tellg();
            file.seekp(pos - static_cast<int>(sizeof(Citizen))); 
            file.write(reinterpret_cast<char*>(&c), sizeof(Citizen)); 
            found = true;
            cout << "\n Citizen record updated successfully.\n";
            break; 
        }
    }
    file.close();
    if (!found) cout << "\n Record not found or is not active.\n";
}

void deleteCitizen() {
    char aad[13];
    cout << "\nEnter Aadhar Number of citizen to delete: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cin.getline(aad, 13);

    Citizen c;
    bool found = false;

    fstream file(FILENAME, ios::binary | ios::in | ios::out);
    if (!file.is_open()) {
        cout << "\n No records found or file cannot be opened for deletion.\n";
        return;
    }

    while (file.read(reinterpret_cast<char*>(&c), sizeof(Citizen))) {
        if (strcmp(c.getAadhar(), aad) == 0 && c.getIsActive()) { 
            c.setIsActive(false); 
            int pos = file.tellg();
            file.seekp(pos - static_cast<int>(sizeof(Citizen)));
            file.write(reinterpret_cast<char*>(&c), sizeof(Citizen));
            found = true;
            cout << "\n Record deleted successfully (marked as inactive).\n";
            break; 
        }
    }
    file.close();
    if (!found) cout << "\n Record not found or is already inactive.\n";
}

const char ADMIN_USERNAME[] = "suraj";
const char ADMIN_PASSWORD[] = "suraj@77";

bool adminLogin() {
    char username[50];
    char password[50];
    int attempts = 3; 
    char ch; 

    while (attempts > 0) {
        cout << "\n =================\n";
        cout << "   Admin Login \n";
        cout << " =================\n";
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";

        int i = 0;
       
        while (true) {
            ch = _getch(); 
            if (ch == 13) break; 
            if (ch == 8) { 
                if (i > 0) {
                    i--;
                    cout << "\b \b"; 
                }
                continue;
            }
           
            if (i < 49) { 
                password[i] = ch;
                cout << '*';
                i++;
            }
        }
        password[i] = '\0'; 
        cout << endl;

       
        if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
            cout << "\nLogin successful!\n";
            return true; 
        } else {
            attempts--; 
            cout << "\nInvalid credentials! " << attempts << " attempts remaining.\n";
        }
    }
    cout << "\nToo many failed attempts. System locked.\n";
    return false; 
}

void generateReport() {
    Citizen c;
    ofstream report("citizen_report.txt");
    if (!report.is_open()) {
        cout << "\nError: Could not create report file.\n";
        return;
    }

   
    report << "\n==========================================\n";
    report << "        CITIZEN DATABASE REPORT          \n";
    report << "==========================================\n";
    report << "Generated on: " << __DATE__ << " at " << __TIME__ << "\n\n";

    ifstream in(FILENAME, ios::binary);
    if (!in.is_open()) {
        report << "No records found or database file cannot be opened.\n";
        report.close();
        cout << "\nError: Could not open database file.\n";
        return;
    }

    int totalRecords = 0;
    report << left << setw(15) << "Aadhar" << setw(20) << "Name" << setw(15)
           << "Phone" << setw(10) << "Gender" << setw(15) << "DOB" << setw(30) << "Address" << endl;
    report << string(100, '-') << endl;

    while (in.read(reinterpret_cast<char*>(&c), sizeof(Citizen))) {
        if (c.getIsActive()) {
            report << left << setw(15) << c.getAadhar() << setw(20) << c.getName() << setw(15)
                   << c.getPhone() << setw(10) << c.getGender() << setw(15) << c.getDOB() 
                   << setw(30) << c.getAddress() << endl;
            totalRecords++;
        }
    }
    in.close();

    report << "\n==========================================\n";
    report << "Total Active Records: " << totalRecords << "\n";
    report << "==========================================\n";
    report.close();

    cout << "\nReport generated successfully in 'citizen_report.txt'\n";
}

int main() {
    if (!adminLogin()) {
        return 1;
    }

    int choice;
    do {
        cout << "\n =================================\n";
        cout << "   Aadhar Card Management System \n";
        cout << " =================================\n";
        cout << "1. Add New Citizen\n";
        cout << "2. Display All Records\n";
        cout << "3. Search Citizen by Aadhar\n";
        cout << "4. Update Citizen Record\n";
        cout << "5. Delete Citizen Record\n";
        cout << "6. Generate Report\n";          
        cout << "7. Exit\n";                   

        while (true) {
            cout << "\nEnter your choice: ";
            if (cin >> choice) {
                break;
            } else {
                cout << "Invalid input! Please enter a number between 1 to 7.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {
            case 1: addCitizen(); break;
            case 2: displayAll(); break;
            case 3: searchCitizen(); break;
            case 4: updateCitizen(); break;
            case 5: deleteCitizen(); break;
            case 6: generateReport(); break;    
            case 7: cout << "\n Exiting system. Goodbye!\n"; break;
            default: cout << "\n Invalid choice. Please enter a number between 1 and 7.\n";
        }

    } while (choice != 7);                     

    return 0;
}
