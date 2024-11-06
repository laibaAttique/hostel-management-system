
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
using namespace std;
struct Resident {
    string name;
    string cnic;
    string phoneNo;
    string adress;
    int dues;
    int laundryPieces;
    Resident* prev;
    Resident* next;
};
class Node {
   public:
    int roomNo;
    Resident* resident;
    int capacity;
    int height;
    int availableSeats;
    Node* left;
    
    
    Node* right;
};
int height(Node* N) {
    if (N == NULL) {
        return 0;
    }
    return N->height;
}
int max(int a, int b) { return (a > b) ? a : b; }
Node* newNode(int roomNo) {
    Node* room = new Node();
    room->roomNo = roomNo;
    if (roomNo % 2 == 0) {
        room->capacity = 2;
    } else {
        room->capacity = 3;
    }
    room->availableSeats = room->capacity;
    room->resident = NULL;
    room->left = NULL;
    room->right = NULL;
    room->height = 1;
    return (room);
}
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(Node* N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}
Node* insert(Node* node, int roomNo) {
    /* 1. Perform the normal BST insertion */
    if (node == NULL) return (newNode(roomNo));

    if (roomNo < node->roomNo)
        node->left = insert(node->left, roomNo);
    else if (roomNo > node->roomNo)
        node->right = insert(node->right, roomNo);
    else  // Equal keys are not allowed in BST
        return node;

    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left), height(node->right));

    /* 3. Get the balance factor of this ancestor
            node to check whether this node became
            unbalanced */
    int balance = getBalance(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Left Left Case
    if (balance > 1 && roomNo < node->left->roomNo) return rightRotate(node);

    // Right Right Case
    if (balance < -1 && roomNo > node->right->roomNo) return leftRotate(node);

    // Left Right Case
    if (balance > 1 && roomNo > node->left->roomNo) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && roomNo < node->right->roomNo) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* return the (unchanged) node pointer */
    return node;
}
Node* searchNode(Node* root, int roomNo) {
    if (root == NULL) {
        cout << "gyk";
        return NULL;
    } else {
        if (root->roomNo == roomNo) {
            return root;
        } else if (roomNo > root->roomNo) {
            return searchNode(root->right, roomNo);
        } else if (roomNo < root->roomNo) {
            return searchNode(root->left, roomNo);
        }
    }
}
void AvailableSeats(Node* root) {
    if (root != NULL) {
        AvailableSeats(root->left);
        if (root->availableSeats > 0) {
            cout << root->roomNo << "	    |	   " << root->availableSeats
                 << endl;
        }
        AvailableSeats(root->right);
    }
}
void AddResident(Node* root, string name, string CNIC, string phoneNo,
                 string adress, int dues, int roomNo, int pieces) {
    if (root != NULL) {
        if (root->roomNo > roomNo) {
            AddResident(root->left, name, CNIC, phoneNo, adress, dues, roomNo,
                        pieces);
        } else if (root->roomNo < roomNo) {
            AddResident(root->right, name, CNIC, phoneNo, adress, dues, roomNo,
                        pieces);
        } else if (root->roomNo == roomNo) {
            if (root->availableSeats < 1) {
                cout << "No available seats in room number " << root->roomNo
                     << endl;
                return;
            } else {
                if (root->resident == NULL) {
                    root->resident = new Resident;
                    root->resident->name = name;
                    root->resident->cnic = CNIC;
                    root->resident->phoneNo = phoneNo;
                    root->resident->adress = adress;
                    root->resident->dues = dues;
                    root->resident->laundryPieces = pieces;
                    root->resident->prev = NULL;
                    root->resident->next = NULL;
                } else {
                    Resident* temp = root->resident;
                    while (temp->next != NULL) {
                        temp = temp->next;
                    }
                    temp->next = new Resident;
                    temp->next->name = name;
                    temp->next->cnic = CNIC;
                    temp->next->phoneNo = phoneNo;
                    temp->next->adress = adress;
                    temp->next->dues = dues;
                    temp->next->laundryPieces = pieces;
                    temp->next->prev = temp;
                    temp->next->next = NULL;
                }
                root->availableSeats -= 1;
                // cout << "Resident successfully added"<<endl;
            }
        }
    }
}
void RemoveResident(Node* root, int roomNo, string name) {
    Node* room = searchNode(root, roomNo);

    if (room == NULL || room->resident == NULL) {
        cout << "Room not found or no residents in room " << roomNo << endl;
        return;
    }

    Resident* current = room->resident;
    Resident* prev = NULL;
    while (current != NULL) {
        if (current->name == name) {
            if (prev == NULL) {
                room->resident = current->next;
            } else {
                prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = prev;
                }
            }

            delete current;
            room->availableSeats += 1;
            cout << "Resident successfully removed from room " << roomNo
                 << endl;
            return;
        }

        prev = current;
        current = current->next;
    }

    cout << "Resident not found in room " << roomNo << endl;
}

void searchResident(Node* root, int roomNo, string name) {
    Node* room = searchNode(root, roomNo);
    Resident* resident = room->resident;
    while (resident != NULL) {
        if (resident->name == name) {
            cout << "---------------------------" << endl;
            cout << "	   RESIDENT DATA" << endl;
            cout << "name:" << resident->name << endl;
            cout << "CNIC number: " << resident->cnic << endl;
            cout << "Phone Number: " << resident->phoneNo << endl;
            cout << "Adress: " << resident->adress << endl;
            cout << "Remaining dues: " << resident->dues << endl;
            cout << "---------------------------" << endl;
            return;
        }
        resident = resident->next;
    }
    cout << "not found" << endl;
    return;
}

void writeTreeToCSVHelper(Node* node, ofstream& file) {
    if (node != NULL) {
        // Write data of the current node
        Resident* resident = node->resident;
        while (resident != NULL) {
            file << node->roomNo << "," << resident->name << ","
                 << resident->cnic << "," << resident->phoneNo << ","
                 << resident->adress << "," << resident->dues << ","
                 << resident->laundryPieces << "\n";
            resident = resident->next;
        }

        // Recursively traverse left and right subtrees
        writeTreeToCSVHelper(node->left, file);
        writeTreeToCSVHelper(node->right, file);
    }
}

// Function to write data to a CSV file
void writeDataToCSV(Node* root, string filename) {
    ofstream file(
        filename
            .c_str());  // Note: Need to use c_str() for ofstream with string

    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    file << "roomNo,name,cnic,phone,adress,dues,LaundryPieces\n";  // CSV header

    // Call the helper function with the root of the AVL tree
    writeTreeToCSVHelper(root, file);

    file.close();
}

void readDataFromCSV(Node* root, string filename) {
    ifstream file(
        filename
            .c_str());  // Note: Need to use c_str() for ifstream with string

    if (!file.is_open()) {
        cerr << "New Error opening file: " << filename << endl;
        return;
    }

    string line;
    getline(file, line);  // Skip the header

    while (getline(file, line)) {
        istringstream iss(line);
        string roomNo, name, cnic, phone, adress, dues, LaundryPieces;

        getline(iss, roomNo, ',');
        getline(iss, name, ',');
        getline(iss, cnic, ',');
        getline(iss, phone, ',');
        getline(iss, adress, ',');
        getline(iss, dues, ',');
        getline(iss, LaundryPieces, ',');

        // Convert roomNo from string to integer
    try{
        int roomNumber;
        istringstream(roomNo) >> roomNumber;

        int Dues;
        istringstream(dues) >> Dues;

        int Laundry;
        istringstream(LaundryPieces) >> Laundry;

         
        AddResident(root, name, cnic, phone, adress, Dues, roomNumber, Laundry);
        }catch (const std::exception& e) {
     ::cerr << "An error occurred: " << e.what() << "\n";
}
    }

    file.close();
}
bool checkAvaialability(Node* root, int roomNo) {
    Node* room = searchNode(root, roomNo);
    if (room->availableSeats < 1) {
        return false;
    }
    return true;
}

void submitFee(Node* root, int roomNo, string name, int fee) {
    Node* room = searchNode(root, roomNo);
    Resident* resident = room->resident;
    while (resident != NULL) {
        if (resident->name == name) {
            if (resident->dues == 0) {
                cout << "Fee cannot be submitted, No remaining dues" << endl;
                return;
            }
            if (resident->dues < fee) {
                cout << "Fee cannot be submitted. Because the submission amount "
                        "is greater than your current dues";
                return;
            }
            cout << "----------------------------------------" << endl;
            cout << "			  FEE INVOICE		" << endl;
            cout << "Name: " << resident->name << endl;
            cout << "CNIC number: " << resident->cnic << endl;
            cout << "Room No: " << roomNo << endl;
            cout << "fee submitted: " << fee << endl;
            cout << "Remaining dues: " << resident->dues - fee << endl;
            cout << "----------------------------------------" << endl;
            resident->dues = resident->dues - fee;
            return;
        }
        resident = resident->next;
    }
    cout << "No resident by this name was found" << endl;
}
void updateResidentsFee(Node* node) {
    if (node != NULL) {
        Resident* resident = node->resident;
        while (resident != NULL) {
            resident->dues += 18000;
            resident = resident->next;
        }
        updateResidentsFee(node->left);
        updateResidentsFee(node->right);
    }
}
void getLaundryTicket(Node* root, string name, int roomNo, int pieces) {
    Node* room = searchNode(root, roomNo);
    Resident* res = room->resident;
    while (res != NULL) {
        if (res->name == name) {
            if (res->laundryPieces < pieces) {
                cout << "You donot have sufficient pieces left for laundry"
                     << endl;
                return;
            }
            cout << "---------LAUNRY TICKET----------" << endl;
            cout << "Name: " << name << endl;
            cout << "Room number: " << roomNo << endl;
            cout << "Pieces: " << pieces << endl;
            cout << "Remaining pieces: " << res->laundryPieces - pieces << endl;
            cout << "---------LAUNRY TICKET----------" << endl;
            res->laundryPieces = res->laundryPieces - pieces;
            return;
        }
        res = res->next;
    }
    cout << "Resident not found " << endl;
    return;
}

void updateLaundry(Node* node) {
    if (node != NULL) {
        Resident* resident = node->resident;
        while (resident != NULL) {
            resident->laundryPieces = 30;
            resident = resident->next;
        }
        updateLaundry(node->left);
        updateLaundry(node->right);
    }
}
struct StaffNode {
    string name;
    int age;
    string occupation;
    int id;
    bool busy;
    string cnic;
    StaffNode* next;

    StaffNode(string name, int age, string occupation, int id, string cnic) {
        this->name = name;
        this->age = age;
        this->occupation = occupation;
        this->id = id;
        this->cnic = cnic;
        this->busy = false;
        this->next = NULL;
    }
};
class JanitorRequest {
   public:
    string requesterName;
    int roomNumber;

    JanitorRequest(string name, int room)
        : requesterName(name), roomNumber(room) {}
};

class StaffManager {
   private:
    StaffNode* janitorHead;
    StaffNode* cookHead;
    StaffNode* guardHead;
    StaffNode* electricianHead;
    StaffNode* plumberHead;

    queue<StaffNode*> janitorWaitingList;
    queue<StaffNode*> cookWaitingList;
    queue<StaffNode*> guardWaitingList;
    queue<StaffNode*> electricianWaitingList;
    queue<StaffNode*> plumberWaitingList;
    queue<string> requestingPlumberWaitingList;
    queue<string> requestingElectricianWaitingList;
    queue<JanitorRequest> janitorRequestWaitingList;

    int janitorCount;
    int cookCount;
    int guardCount;
    int electricianCount;
    int plumberCount;

   public:
    StaffManager()
        : janitorHead(NULL),
          cookHead(NULL),
          guardHead(NULL),
          electricianHead(NULL),
          plumberHead(NULL),
          janitorCount(0),
          cookCount(0),
          guardCount(0),
          electricianCount(0),
          plumberCount(0) {}

    void addStaff(string name, int age, string occupation, int id,
                  string cnic) {
        StaffNode* newStaff = new StaffNode(name, age, occupation, id, cnic);

        if (canAddToOccupation(occupation)) {
            addToOccupationList(occupation, newStaff);
        } else {
            addToWaitingList(occupation, newStaff);
        }
    }
    void requestJanitor(string name, int roomNumber) {
        StaffNode*& occupationHead = getOccupationHead("janitor");

        // Check if a janitor is available or in the waiting list
        if (occupationHead || !janitorRequestWaitingList.empty()) {
            // If someone is waiting, assign the next person in the queue
            assignNextJanitor(name, roomNumber);
        } else {
            cout << "No janitor available for request. Adding to waiting list."
                 << endl;
            janitorRequestWaitingList.push(JanitorRequest(name, roomNumber));
        }
    }
    void makeJanitorAvailable() {
        StaffNode*& occupationHead = getOccupationHead("janitor");

        if (!occupationHead) {
            cout << "Janitor is now available" << endl;
            return;
        }

        StaffNode* currentWorker = occupationHead;

        // Find the first busy worker in the list
        while (currentWorker && !currentWorker->busy) {
            currentWorker = currentWorker->next;
        }

        if (currentWorker) {
            currentWorker->busy = false;
            cout << "Janitor is now available." << endl;

            // If someone is waiting, assign the next person in the queue
            assignNextJanitor("", 0);  // Pass empty values as a placeholder,
                                       // you can modify this accordingly
        } else {
            cout << "No busy janitor workers found." << endl;
        }
    }
    void fireStaff(string occupation) {
        StaffNode*& occupationHead = getOccupationHead(occupation);

        if (occupationHead) {
            cout << "Enter the ID of the " << occupation << " to be fired: ";
            int idToFire;
            cin >> idToFire;

            if (removeFromLinkedList(occupationHead, idToFire)) {
                cout << "Employee with ID " << idToFire
                     << " successfully fired." << endl;

                // If there is someone in the waiting list for the same
                // occupation, replace the fired employee
                replaceFiredEmployee(occupation);
            } else {
                cout << "No employee found with ID " << idToFire << "." << endl;
            }
        } else {
            cout << "No " << occupation << " employees to fire." << endl;
        }
    }

    void displayAllStaffMembers() {
        displayStaffMembers(janitorHead, "Janitor");
        displayStaffMembers(cookHead, "Cook");
        displayStaffMembers(guardHead, "Guard");
        displayStaffMembers(electricianHead, "Electrician");
        displayStaffMembers(plumberHead, "Plumber");
    }

    void displayWaitingLists() {
        cout << "Waiting Lists:" << endl;
        displayStaffMembers(janitorWaitingList, "Janitor Waiting List");
        displayStaffMembers(cookWaitingList, "Cook Waiting List");
        displayStaffMembers(guardWaitingList, "Guard Waiting List");
        displayStaffMembers(electricianWaitingList, "Electrician Waiting List");
        displayStaffMembers(plumberWaitingList, "Plumber Waiting List");
    }
    void requestWorker(string name, string occupation, int roomNumber) {
        StaffNode*& occupationHead = getOccupationHead(occupation);

        // Check if the occupation is available in the employment list or in the
        // waiting list
        if (occupationHead || !getWaitingList(occupation).empty()) {
            // If someone is waiting, assign the next person in the queue
            assignNextFromQueue(occupation);

            cout << "Worker assigned to " << name << " in room " << roomNumber
                 << " for " << occupation << " work." << endl;
        } else {
            cout << "No " << occupation << " available for request." << endl;
        }
    }

    void makePlumberAvailable() { makeAvailable("plumber"); }

    // Function to make an electrician available
    void makeElectricianAvailable() { makeAvailable("electrician"); }

    void assignNextFromQueue(string occupation) {
        StaffNode*& occupationHead = getOccupationHead(occupation);

        if (!occupationHead) {
            cout << "No " << occupation << " available for assignment." << endl;
            return;
        }

        StaffNode* currentWorker = occupationHead;

        // Find the first available worker in the list
        while (currentWorker && currentWorker->busy) {
            currentWorker = currentWorker->next;
        }

        if (currentWorker) {
            currentWorker->busy = true;
            cout << "Next available " << occupation << " assigned to work."
                 << endl;
        } else {
            cout << "No available " << occupation << " workers found." << endl;
        }
    }

    void displayWaitingList(string occupation) {
        queue<StaffNode*>& staffWaitingList = getWaitingList(occupation);
        queue<JanitorRequest>& janitorRequestList = janitorRequestWaitingList;

        cout << "Waiting List for " << occupation << ":" << endl;
        int id = 1;

        while (!staffWaitingList.empty()) {
            StaffNode* currentStaff = staffWaitingList.front();
            staffWaitingList.pop();
            cout << "Name: " << currentStaff->name
                 << ", Age: " << currentStaff->age << ", ID: " << id++
                 << ", Cnic: " << currentStaff->cnic << endl;
        }

        // Display the waiting list for janitor requests
        if (occupation == "janitor") {
            int requestID = 1;
            while (!janitorRequestList.empty()) {
                JanitorRequest currentRequest = janitorRequestList.front();
                janitorRequestList.pop();
                cout << "Requester: " << currentRequest.requesterName
                     << ", Room: " << currentRequest.roomNumber
                     << ", Request ID: " << requestID++ << endl;
            }
        }

        cout << "---------------------" << endl;
    }
    void makeAvailable(string occupation) {
        StaffNode*& occupationHead = getOccupationHead(occupation);

        if (!occupationHead) {
            cout << "No " << occupation << " workers available." << endl;
            return;
        }

        StaffNode* currentWorker = occupationHead;

        // Find the first busy worker in the list
        while (currentWorker && !currentWorker->busy) {
            currentWorker = currentWorker->next;
        }

        if (currentWorker) {
            currentWorker->busy = false;
            cout << "Worker for " << occupation << " is now available." << endl;

            // If someone is waiting, assign the next person in the queue
            assignNextFromQueue(occupation);
        } else {
            cout << "No busy " << occupation << " workers found." << endl;
        }
    }

   private:
    bool canAddToOccupation(string occupation) {
        if (occupation == "janitor" && janitorCount < 2) {
            return true;
        } else if (occupation == "cook" && cookCount < 1) {
            return true;
        } else if (occupation == "guard" && guardCount < 1) {
            return true;
        } else if (occupation == "electrician" && electricianCount < 1) {
            return true;
        } else if (occupation == "plumber" && plumberCount < 1) {
            return true;
        }

        return false;
    }

    void addToOccupationList(string occupation, StaffNode* newStaff) {
        int& occupationCount = getOccupationCount(occupation);
        StaffNode*& occupationHead = getOccupationHead(occupation);

        newStaff->id = ++occupationCount;

        addNodeToLinkedList(occupationHead, newStaff);
    }

    void addToWaitingList(string occupation, StaffNode* newStaff) {
        queue<StaffNode*>& waitingListQueue = getWaitingList(occupation);
        waitingListQueue.push(newStaff);

        cout << "Limit exceeded for '" << occupation
             << "'. You've been added to the waiting list for your type."
             << endl;
    }

    StaffNode*& getOccupationHead(string occupation) {
        if (occupation == "janitor") {
            return janitorHead;
        } else if (occupation == "cook") {
            return cookHead;
        } else if (occupation == "guard") {
            return guardHead;
        } else if (occupation == "electrician") {
            return electricianHead;
        } else if (occupation == "plumber") {
            return plumberHead;
        } else {
            cout<<"Invalid occupation specified";
        }
    }

    int& getOccupationCount(string occupation) {
        if (occupation == "janitor") {
            return janitorCount;
        } else if (occupation == "cook") {
            return cookCount;
        } else if (occupation == "guard") {
            return guardCount;
        } else if (occupation == "electrician") {
            return electricianCount;
        } else if (occupation == "plumber") {
            return plumberCount;
        } else {
            cout<<"Invalid occupation specified";
        }
    }

    queue<StaffNode*>& getWaitingList(string occupation) {
        if (occupation == "janitor") {
            return janitorWaitingList;
        } else if (occupation == "cook") {
            return cookWaitingList;
        } else if (occupation == "guard") {
            return guardWaitingList;
        } else if (occupation == "electrician") {
            return electricianWaitingList;
        } else if (occupation == "plumber") {
            return plumberWaitingList;
        } else {
            cout<<"Invalid occupation specified";
        }
    }

    void addNodeToLinkedList(StaffNode*& head, StaffNode* newStaff) {
        if (!head) {
            head = newStaff;
        } else {
            StaffNode* curr = head;
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newStaff;
        }
    }

    void displayStaffMembers(StaffNode* head, string occupation) {
        cout << occupation << "s:" << endl;
        int id = 1;

        while (head) {
            cout << "Name: " << head->name << ", Age: " << head->age
                 << ", ID: " << id++ << ", Cnic: " << head->cnic << endl;
            head = head->next;
        }

        cout << "---------------------" << endl;
    }

    void displayStaffMembers(queue<StaffNode*>& waitingList,
                             string occupation) {
        cout << occupation << ":" << endl;
        int id = 1;

        while (!waitingList.empty()) {
            StaffNode* currentStaff = waitingList.front();
            waitingList.pop();
            cout << "Name: " << currentStaff->name
                 << ", Age: " << currentStaff->age << ", ID: " << id++
                 << ", Cnic: " << currentStaff->cnic << endl;
        }

        cout << "---------------------" << endl;
    }
    bool removeFromLinkedList(StaffNode*& head, int idToRemove) {
        StaffNode* temp = head;
        StaffNode* prev = NULL;

        // Traverse the linked list to find the node with the specified ID
        while (temp != NULL && temp->id != idToRemove) {
            prev = temp;
            temp = temp->next;
        }

        // If the node is found, remove it from the linked list
        if (temp != NULL) {
            if (prev == NULL) {
                head = temp->next;
            } else {
                prev->next = temp->next;
            }

            delete temp;
            return true;
        }

        return false;
    }

    void replaceFiredEmployee(string occupation) {
        queue<StaffNode*>& waitingList = getWaitingList(occupation);
        StaffNode*& occupationHead = getOccupationHead(occupation);

        if (!waitingList.empty()) {
            StaffNode* replacement = waitingList.front();
            waitingList.pop();

            cout << "New employee from the waiting list assigned to "
                 << occupation << "." << endl;

            // Assign a new ID to the replacement employee
            replacement->id = getOccupationCount(occupation);

            // Add the replacement employee to the linked list
            addNodeToLinkedList(occupationHead, replacement);
        }
    }
    queue<string>& getRequestWaitingList(string occupation) {
        if (occupation == "plumber") {
            return requestingPlumberWaitingList;
        } else if (occupation == "electrician") {
            return requestingElectricianWaitingList;
        } else {
            cout<<"Invalid occupation specified for request";
        }
    }
    void assignNextJanitor(string requesterName, int roomNumber) {
        StaffNode*& occupationHead = getOccupationHead("janitor");

        if (!janitorRequestWaitingList.empty()) {
            // If there are requests in the waiting list, assign the janitor to
            // the next request
            JanitorRequest request = janitorRequestWaitingList.front();
            janitorRequestWaitingList.pop();
            cout << "Janitor assigned to " << request.requesterName
                 << " in room " << request.roomNumber << "." << endl;
        } else if (occupationHead) {
            // If there is no waiting request, assign the janitor to the
            // requester
            assignJanitor(requesterName, roomNumber);
        }
    }
    void assignJanitor(string requesterName, int roomNumber) {
        StaffNode*& occupationHead = getOccupationHead("janitor");

        if (occupationHead) {
            StaffNode* janitor = occupationHead;
            occupationHead = occupationHead->next;

            cout << "Janitor assigned to " << requesterName << " in room "
                 << roomNumber << "." << endl;
        }
    }
};

void displayMainMenu() {
    cout << "Main Menu:" << endl;
    cout << "1. Hire Employees" << endl;
    cout << "2. Fire Employees" << endl;
    cout << "3. Display All Staff Members" << endl;
    cout << "4. Display Waiting Lists" << endl;
    cout << "5. Request Worker" << endl;
    cout << "6. Exit" << endl;
    ;
    cout << "Enter your choice (1-8): ";
}
void displayRequestMenu() {
    cout << "Request Menu:" << endl;
    cout << "1. Request Plumber" << endl;
    cout << "2. Request Electrician" << endl;
    cout << "3. Make Plumber Available" << endl;
    cout << "4. Make Electrician Available" << endl;
    cout << "5. Display Plumber Waiting List" << endl;
    cout << "6. Display Electrician Waiting List" << endl;
    cout << "7. Exit" << endl;
    cout << "Enter your choice (1-7): ";
}
void processRequest(StaffManager& staff, string occupation) {
    string name;
    int roomNumber;

    cout << "Enter your name: ";
    getline(cin, name);

    cout << "Enter your room number: ";
    cin >> roomNumber;

    staff.requestWorker(name, occupation, roomNumber);
}

void staffManagement() {
    StaffManager staff;

    char mainChoice;

    do {
        cout << "Main Menu:" << endl;
        cout << "1. Hire Employees" << endl;
        cout << "2. Fire Employees" << endl;
        cout << "3. Display All Staff Members" << endl;
        cout << "4. Display Waiting Lists" << endl;
        cout << "5. Request Menu" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice (1-6): ";
        cin >> mainChoice;

        // Clear input buffer
       // cin.ignore

        switch (mainChoice) {
            case '1': {
                char hireChoice;
                do {
                    cout << "Enter your name: ";
                    string name;
                    cin.ignore();
                    getline(cin, name);

                    cout << "Enter your age: ";
                    int age;
                    cin.ignore();
                    cin >> age;

                    cout << "Enter your cnic: ";
                    string cnic;
                    cin.ignore();
                    cin >> cnic;

                    cout << "Enter your occupation (janitor, cook, guard, "
                            "plumber, electrician): ";
                    string occupation;
                    cin.ignore();
                    cin >> occupation;

                    staff.addStaff(name, age, occupation, 0, cnic);

                    cout << "Do you want to hire more staff? (y/n): ";
                    cin >> hireChoice;

                    
                } while (hireChoice == 'y' || hireChoice == 'Y');

                cout << "Hiring process completed." << endl;
                break;
            }

            case '2': {
                char fireChoice;
                do {
                    cout << "Enter the occupation of the employee to fire "
                            "(janitor, cook, guard, plumber, electrician): ";
                    string occupation;
                    cin >> occupation;
                    
                    staff.fireStaff(occupation);

                    cout << "Do you want to fire another employee? (y/n): ";
                    cin >> fireChoice;

                    
                } while (fireChoice == 'y' || fireChoice == 'Y');
                break;
            }

            case '3':
                staff.displayAllStaffMembers();
                break;

            case '4':
                staff.displayWaitingLists();
                break;

            case '5': {
                char requestChoice;
                do {
                    cout << "Request Menu:" << endl;
                    cout << "0. Request Plumber" << endl;
                    cout << "1. Request Electrician" << endl;
                    cout << "2. Request Janitor" << endl;
                    cout << "3. Make Plumber Available" << endl;
                    cout << "4. Make Electrician Available" << endl;
                    cout << "5. Make Janitor Available" << endl;
                    cout << "6. Display Plumber Waiting List" << endl;
                    cout << "7. Display Electrician Waiting List" << endl;
                    cout << "8. Display Janitor Waiting List" << endl;
                    cout << "9. Exit" << endl;
                    cout << "Enter your choice (0-9): ";

                    cin >> requestChoice;

                    
                    switch (requestChoice) {
                        case '0':
                            processRequest(staff, "plumber");
                            break;

                        case '1':
                            processRequest(staff, "electrician");
                            break;

                        case '2': {
                            string name;
                            int roomNumber;

                            cout << "Enter your name: ";
                            cin.ignore();
                            getline(cin, name);

                            cout << "Enter your room number: ";
                            
                            cin >> roomNumber;

                            staff.requestJanitor(name, roomNumber);
                            break;
                        }

                        case '3':
                            staff.makePlumberAvailable();
                            cout << "Plumber is now available." << endl;
                            break;

                        case '4':
                            staff.makeElectricianAvailable();
                            cout << "Electrician is now available." << endl;
                            break;

                        case '5':
                            int janitorCount;
                            cout << "Enter the count of janitors to make "
                                    "available: ";
                            cin.ignore();
                            cin >> janitorCount;                        
                            staff.makeJanitorAvailable();
                            break;

                        case '6':
                            staff.displayWaitingList("plumber");
                            break;

                        case '7':
                            staff.displayWaitingList("electrician");
                            break;

                        case '8':
                            // Display Janitor Waiting List
                            cout << "Janitor Waiting List:" << endl;
                            staff.displayWaitingList("janitor");
                            cout << "---------------------" << endl;
                            break;

                        case '9':
                            cout << "Exiting the request system." << endl;
                            break;

                        default:
                            cout << "Invalid choice. Please enter a number "
                                    "between 0 and 9."
                                 << endl;
                            break;
                    }
                } while (requestChoice != '9');
                break;
            }

            case '6':
                break;

            default:
                cout << "Invalid choice. Please enter a number between 1 and 6."
                     << endl;
                break;
        }

    } while (mainChoice != '6');
}
void residentManagement() {
	cout << "----------------------- SKWK1---------------" << endl;
    Node* root = NULL;
    for (int i = 1; i <= 16; i++) {
        root = insert(root, i);
    }
    
    readDataFromCSV(root, "ResidentsData.csv");

    char userInput;
    cout << "      ---------------------------------------" << endl;
    cout << "       WELCOME TO RESIDENT MANAGEMENT SYSTEM" << endl;
    cout << "      ---------------------------------------" << endl;
    do {
        cout << "Enter an option to continue" << endl;
        cout << "1. See all available seats" << endl;
        cout << "2. Add a new resident" << endl;
        cout << "3. Remove a resident" << endl;
        cout << "4. Search for a resident to see their data" << endl;
        cout << "5. Submit Fee" << endl;
        cout << "6. Update residents dues(add monthly fee)" << endl;
        cout << "7. Get laundry tickets" << endl;
        cout << "8. Update number of Laundry pieces" << endl;
        cout << "0. Exit" << endl;

        cin >> userInput;

        switch (userInput) {
            case '1':
                cout << "RoomNo      | AvailableSeats" << endl;
                AvailableSeats(root);
                break;
            case '2': {
                string name, cnic, phone, address;
                int roomNo;

                cout << "Enter room number: ";
                cin >> roomNo;

                if (!checkAvaialability(root, roomNo)) {
                    cout << "No available seats in this room." << endl;
                    continue;
                }

                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter CNIC number: ";
                getline(cin, cnic);

                cout << "Enter phone number: ";
                cin >> phone;

                cout << "Enter address: ";
                cin.ignore();
                getline(cin, address);
                AddResident(root, name, cnic, phone, address, 18000, roomNo,
                            30);
                cout << "process completed" << endl;
                break;
            }
            case '3': {
                string name;
                int roomNo;

                cout << "Enter the name of the resident: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter the room number: ";
                cin >> roomNo;

                RemoveResident(root, roomNo, name);
                break;
            }

            case '4': {
                string name;
                int roomNo;

                cout << "Enter the name of the resident: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter the room number: ";
                cin >> roomNo;

                searchResident(root, roomNo, name);
                break;
            }

            case '5': {
                string name;
                int roomNo, fee;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter room number: ";
                cin >> roomNo;

                cout << "Enter amount of fees to submit: ";
                cin >> fee;

                submitFee(root, roomNo, name, fee);
                break;
            }
            case '6': {
                updateResidentsFee(root);
                cout << "all residents fee updated successfully" << endl;
                break;
            }
            case '7': {
                string name;
                int roomNo, pieces;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter room number: ";
                cin >> roomNo;

                cout << "Enter number of pieces to give for laundry: ";
                cin >> pieces;
                getLaundryTicket(root, name, roomNo, pieces);
                break;
            }
            case '8': {
                updateLaundry(root);
                cout << "Number of pieces for laundry updated for all residents"
                     << endl;
                break;
            }
            case '0':
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    } while (userInput != '0');

    writeDataToCSV(root, "ResidentsData.csv");
}
int main() {
    char choice;
    cout << "      ---------------------------------------" << endl;
    cout << "       WELCOME TO HOSTEL MANAGEMENT SYSTEM" << endl;
    cout << "      ---------------------------------------" << endl;
    do {
        cout << "Enter an option to continue" << endl;
        cout << "1. Resident management" << endl;
        cout << "2. Staff management" << endl;
        cout << "0. Exit" << endl;

        cin >> choice;

        switch (choice) {
            case '1': {
                residentManagement();
                break;
            }
            case '2': {
                staffManagement();
                break;
            }
            case '0': {
                cout << "Exiting the program. Goodbye!" << endl;
                break;
            }
            default:
                cout << "Incorrect input try again" << endl;
                break;
        }
    } while (choice != '0');

    return 0;
}
