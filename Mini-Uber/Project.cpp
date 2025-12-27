#include<iostream>
#include<vector>
#include<queue>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<iomanip>

using namespace std;

// ==================== CLASS DEFINITIONS ====================

class Driver {
private:
    float total_Earning;
    float total_EarningPerRide;
    int totalRidesCompleted;

public:
    string Name;
    int ID;
    long long Contact;
    int Ratings;
    char Availability;

    Driver() {
        total_Earning = 0;
        total_EarningPerRide = 0;
        totalRidesCompleted = 0;
        Name = "";
        ID = 0;
        Contact = 0;
        Ratings = 0;
        Availability = 'F';
    }

    Driver(float t_earn, float t_earnPerRide, string n, int id, long long cont, int ratgs, char avail, int rides = 0) {
        total_Earning = t_earn;
        total_EarningPerRide = t_earnPerRide;
        totalRidesCompleted = rides;
        Name = n;
        ID = id;
        Contact = cont;
        Ratings = ratgs;
        Availability = avail;
    }

    float gettotal_Earning() { return total_Earning; }
    void settotal_Earning(float earn) { total_Earning = earn; }
    
    float gettotal_EarningPerRide() { return total_EarningPerRide; }
    void settotal_EarningPerRide(float earn) { total_EarningPerRide = earn; }
    
    int getTotalRides() { return totalRidesCompleted; }
    void incrementRides() { totalRidesCompleted++; }
};

class Rider {
private:
    long long Contact;

public:
    string Name;
    int ID;
    string Address;
    int totalBookings;

    Rider() {
        Contact = 0;
        Name = "";
        ID = 0;
        Address = "";
        totalBookings = 0;
    }

    Rider(long long cont, string n, int id, string add, int bookings = 0) {
        Contact = cont;
        Name = n;
        ID = id;
        Address = add;
        totalBookings = bookings;
    }

    long long getContact() { return Contact; }
    void setContact(long long cont) { Contact = cont; }
    void incrementBookings() { totalBookings++; }
};

class Ride {
private:
    float Payment;

public:
    string PickUp;
    float Distance;
    string Destination;
    string timeStamp;
    int driverID;
    int riderID;

    Ride() {
        Payment = 0;
        PickUp = "";
        Distance = 0;
        Destination = "";
        timeStamp = "";
        driverID = 0;
        riderID = 0;
    }

    Ride(float pay, string pick, float dist, string dest, string stamp, int dID = 0, int rID = 0) {
        Payment = pay;
        PickUp = pick;
        Distance = dist;
        Destination = dest;
        timeStamp = stamp;
        driverID = dID;
        riderID = rID;
    }

    float getPayment() { return Payment; }
    void setPayment(float pay) { Payment = pay; }
};

// ==================== GLOBAL DATA STRUCTURES ====================

vector<Driver> allDrivers;
vector<Rider> allRiders;
vector<Ride> allCompletedRides;
vector<Ride> allCancelledRides;
queue<Ride> pendingRides;

// ==================== FILE HANDLING FUNCTIONS ====================

void saveDriversToFile() {
    ofstream file("drivers.txt");
    if (!file.is_open()) return;

    for (size_t i = 0; i < allDrivers.size(); i++) {
        file << allDrivers[i].Name << ","
             << allDrivers[i].ID << ","
             << allDrivers[i].Contact << ","
             << allDrivers[i].Ratings << ","
             << allDrivers[i].Availability << ","
             << allDrivers[i].gettotal_Earning() << ","
             << allDrivers[i].getTotalRides() << endl;
    }
    file.close();
}

void saveRidersToFile() {
    ofstream file("riders.txt");
    if (!file.is_open()) return;

    for (size_t i = 0; i < allRiders.size(); i++) {
        file << allRiders[i].Name << ","
             << allRiders[i].ID << ","
             << allRiders[i].getContact() << ","
             << allRiders[i].Address << ","
             << allRiders[i].totalBookings << endl;
    }
    file.close();
}

void saveCompletedRidesToFile() {
    ofstream file("completed_rides.txt");
    if (!file.is_open()) return;

    for (size_t i = 0; i < allCompletedRides.size(); i++) {
        file << allCompletedRides[i].PickUp << ","
             << allCompletedRides[i].Destination << ","
             << allCompletedRides[i].Distance << ","
             << allCompletedRides[i].getPayment() << ","
             << allCompletedRides[i].timeStamp << ","
             << allCompletedRides[i].driverID << ","
             << allCompletedRides[i].riderID << endl;
    }
    file.close();
}

void saveCancelledRidesToFile() {
    ofstream file("cancelled_rides.txt");
    if (!file.is_open()) return;

    for (size_t i = 0; i < allCancelledRides.size(); i++) {
        file << allCancelledRides[i].PickUp << ","
             << allCancelledRides[i].Destination << ","
             << allCancelledRides[i].Distance << ","
             << allCancelledRides[i].getPayment() << ","
             << allCancelledRides[i].timeStamp << ","
             << allCancelledRides[i].driverID << ","
             << allCancelledRides[i].riderID << endl;
    }
    file.close();
}

void savePendingRidesToFile() {
    ofstream file("pending_rides.txt");
    if (!file.is_open()) return;

    queue<Ride> temp = pendingRides;
    while (!temp.empty()) {
        Ride r = temp.front();
        file << r.PickUp << ","
             << r.Destination << ","
             << r.Distance << ","
             << r.getPayment() << ","
             << r.timeStamp << ","
             << r.driverID << ","
             << r.riderID << endl;
        temp.pop();
    }
    file.close();
}

void loadDriversFromFile() {
    ifstream file("drivers.txt");
    if (!file.is_open()) return;

    allDrivers.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string name;
        int id, ratings, rides;
        long long contact;
        char availability;
        float earnings;

        getline(ss, name, ',');
        ss >> id; ss.ignore();
        ss >> contact; ss.ignore();
        ss >> ratings; ss.ignore();
        ss >> availability; ss.ignore();
        ss >> earnings; ss.ignore();
        ss >> rides;

        Driver d(earnings, 0.0, name, id, contact, ratings, availability, rides);
        allDrivers.push_back(d);
    }
    file.close();
}

void loadRidersFromFile() {
    ifstream file("riders.txt");
    if (!file.is_open()) return;

    allRiders.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string name, address;
        int id, bookings;
        long long contact;

        getline(ss, name, ',');
        ss >> id; ss.ignore();
        ss >> contact; ss.ignore();
        getline(ss, address, ',');
        ss >> bookings;

        Rider r(contact, name, id, address, bookings);
        allRiders.push_back(r);
    }
    file.close();
}

void loadCompletedRidesFromFile() {
    ifstream file("completed_rides.txt");
    if (!file.is_open()) return;

    allCompletedRides.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string pickup, destination, timestamp;
        float distance, payment;
        int driverID, riderID;

        getline(ss, pickup, ',');
        getline(ss, destination, ',');
        ss >> distance; ss.ignore();
        ss >> payment; ss.ignore();
        getline(ss, timestamp, ',');
        ss >> driverID; ss.ignore();
        ss >> riderID;

        Ride r(payment, pickup, distance, destination, timestamp, driverID, riderID);
        allCompletedRides.push_back(r);
    }
    file.close();
}

void loadCancelledRidesFromFile() {
    ifstream file("cancelled_rides.txt");
    if (!file.is_open()) return;

    allCancelledRides.clear();
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string pickup, destination, timestamp;
        float distance, payment;
        int driverID, riderID;

        getline(ss, pickup, ',');
        getline(ss, destination, ',');
        ss >> distance; ss.ignore();
        ss >> payment; ss.ignore();
        getline(ss, timestamp, ',');
        ss >> driverID; ss.ignore();
        ss >> riderID;

        Ride r(payment, pickup, distance, destination, timestamp, driverID, riderID);
        allCancelledRides.push_back(r);
    }
    file.close();
}

void loadPendingRidesFromFile() {
    ifstream file("pending_rides.txt");
    if (!file.is_open()) return;

    queue<Ride> empty;
    pendingRides = empty;
    string line;

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string pickup, destination, timestamp;
        float distance, payment;
        int driverID, riderID;

        getline(ss, pickup, ',');
        getline(ss, destination, ',');
        ss >> distance; ss.ignore();
        ss >> payment; ss.ignore();
        getline(ss, timestamp, ',');
        ss >> driverID; ss.ignore();
        ss >> riderID;

        Ride r(payment, pickup, distance, destination, timestamp, driverID, riderID);
        pendingRides.push(r);
    }
    file.close();
}

void loadAllData() {
    cout << "\n===== Loading Data from Files =====" << endl;
    loadDriversFromFile();
    loadRidersFromFile();
    loadCompletedRidesFromFile();
    loadCancelledRidesFromFile();
    loadPendingRidesFromFile();
    cout << "Loaded " << allDrivers.size() << " drivers" << endl;
    cout << "Loaded " << allRiders.size() << " riders" << endl;
    cout << "Loaded " << allCompletedRides.size() << " completed rides" << endl;
    cout << "===== Data Loading Complete! =====" << endl;
}

void saveAllData() {
    cout << "\n===== Saving All Data =====" << endl;
    saveDriversToFile();
    saveRidersToFile();
    saveCompletedRidesToFile();
    saveCancelledRidesToFile();
    savePendingRidesToFile();
    cout << "===== All Data Saved Successfully! =====" << endl;
}

// ==================== DRIVER MANAGEMENT ====================

void addDriver() {
    string name;
    int id, ratings;
    long long contact;
    char availability;

    cout << "\n===== Add New Driver =====" << endl;
    cout << "Enter Driver's Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter Driver's ID: ";
    cin >> id;

    cout << "Enter Driver's Contact: ";
    cin >> contact;

    cout << "Enter Driver's Initial Rating (1-5): ";
    cin >> ratings;

    cout << "Enter Driver's Availability (F=Free, B=Busy): ";
    cin >> availability;

    Driver newDriver(0.0, 0.0, name, id, contact, ratings, availability);
    allDrivers.push_back(newDriver);

    saveDriversToFile();
    cout << "\n Driver successfully added and saved!" << endl;
}

void viewAllDrivers() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers in the system!" << endl;
        return;
    }

    cout << "\n========== All Drivers ==========" << endl;
    cout << left << setw(20) << "Name" << setw(10) << "ID" << setw(15) << "Contact"
         << setw(10) << "Rating" << setw(12) << "Status" << setw(15) << "Earnings" << endl;
    cout << string(90, '-') << endl;

    for (size_t i = 0; i < allDrivers.size(); i++) {
        cout << left << setw(20) << allDrivers[i].Name
             << setw(10) << allDrivers[i].ID
             << setw(15) << allDrivers[i].Contact
             << setw(10) << allDrivers[i].Ratings
             << setw(12) << (allDrivers[i].Availability == 'F' ? "Free" : "Busy")
             << "Rs." << allDrivers[i].gettotal_Earning() << endl;
    }
    cout << string(90, '-') << endl;
}

void updateDriver() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers to update!" << endl;
        return;
    }

    int id;
    cout << "\n===== Update Driver =====" << endl;
    cout << "Enter Driver ID to update: ";
    cin >> id;

    int index = -1;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].ID == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << " Driver not found!" << endl;
        return;
    }

    cout << "\nCurrent Details:" << endl;
    cout << "Name: " << allDrivers[index].Name << endl;
    cout << "Contact: " << allDrivers[index].Contact << endl;

    string newName;
    long long newContact;

    cout << "\nEnter new name (or press Enter to keep current): ";
    cin.ignore();
    getline(cin, newName);
    if (!newName.empty()) {
        allDrivers[index].Name = newName;
    }

    cout << "Enter new contact (or 0 to keep current): ";
    cin >> newContact;
    if (newContact != 0) {
        allDrivers[index].Contact = newContact;
    }

    saveDriversToFile();
    cout << "\n Driver updated successfully!" << endl;
}

void searchDriverByID() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers in system!" << endl;
        return;
    }

    int searchID;
    cout << "\n===== Search Driver by ID =====" << endl;
    cout << "Enter Driver ID: ";
    cin >> searchID;

    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].ID == searchID) {
            cout << "\n Driver Found!" << endl;
            cout << "------------------------" << endl;
            cout << "Name: " << allDrivers[i].Name << endl;
            cout << "ID: " << allDrivers[i].ID << endl;
            cout << "Contact: " << allDrivers[i].Contact << endl;
            cout << "Rating: " << allDrivers[i].Ratings << "/5" << endl;
            cout << "Status: " << (allDrivers[i].Availability == 'F' ? "Free" : "Busy") << endl;
            cout << "Total Earnings: Rs." << allDrivers[i].gettotal_Earning() << endl;
            cout << "Total Rides: " << allDrivers[i].getTotalRides() << endl;
            cout << "------------------------" << endl;
            return;
        }
    }

    cout << "\n Driver with ID " << searchID << " not found!" << endl;
}

void searchDriverByName() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers in system!" << endl;
        return;
    }

    string searchName;
    cout << "\n===== Search Driver by Name =====" << endl;
    cout << "Enter Driver Name: ";
    cin.ignore();
    getline(cin, searchName);

    bool found = false;

    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].Name.find(searchName) != string::npos) {
            if (!found) {
                cout << "\n Driver(s) Found!" << endl;
                found = true;
            }
            cout << "------------------------" << endl;
            cout << "Name: " << allDrivers[i].Name << endl;
            cout << "ID: " << allDrivers[i].ID << endl;
            cout << "Contact: " << allDrivers[i].Contact << endl;
            cout << "Rating: " << allDrivers[i].Ratings << "/5" << endl;
            cout << "Status: " << (allDrivers[i].Availability == 'F' ? "Free" : "Busy") << endl;
            cout << "Total Earnings: Rs." << allDrivers[i].gettotal_Earning() << endl;
            cout << "------------------------" << endl;
        }
    }

    if (!found) {
        cout << "\n No driver found with name containing '" << searchName << "'" << endl;
    }
}

bool compareByName(const Driver &a, const Driver &b) {
    return a.Name < b.Name;
}

bool compareByRating(const Driver &a, const Driver &b) {
    return a.Ratings > b.Ratings;
}

void sortDriversByName() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers to sort!" << endl;
        return;
    }

    sort(allDrivers.begin(), allDrivers.end(), compareByName);
    cout << "\n Drivers sorted alphabetically by name!" << endl;
    viewAllDrivers();
}

void sortDriversByRating() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers to sort!" << endl;
        return;
    }

    sort(allDrivers.begin(), allDrivers.end(), compareByRating);
    cout << "\n✓ Drivers sorted by rating (highest first)!" << endl;
    viewAllDrivers();
}

// ==================== RIDER MANAGEMENT ====================

void addRider() {
    string name, address;
    int id;
    long long contact;

    cout << "\n===== Register as Rider =====" << endl;
    cout << "Enter Rider's Name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter Rider's ID: ";
    cin >> id;

    cout << "Enter Rider's Contact: ";
    cin >> contact;

    cout << "Enter Rider's Address: ";
    cin.ignore();
    getline(cin, address);

    Rider newRider(contact, name, id, address);
    allRiders.push_back(newRider);

    saveRidersToFile();
    cout << "\n Rider registered and saved successfully!" << endl;
}

void searchRiderByID() {
    if (allRiders.empty()) {
        cout << "\nNo riders in system!" << endl;
        return;
    }

    int searchID;
    cout << "\n===== Search Rider by ID =====" << endl;
    cout << "Enter Rider ID: ";
    cin >> searchID;

    for (size_t i = 0; i < allRiders.size(); i++) {
        if (allRiders[i].ID == searchID) {
            cout << "\n Rider Found!" << endl;
            cout << "------------------------" << endl;
            cout << "Name: " << allRiders[i].Name << endl;
            cout << "ID: " << allRiders[i].ID << endl;
            cout << "Contact: " << allRiders[i].getContact() << endl;
            cout << "Address: " << allRiders[i].Address << endl;
            cout << "Total Bookings: " << allRiders[i].totalBookings << endl;
            cout << "------------------------" << endl;
            return;
        }
    }

    cout << "\n Rider with ID " << searchID << " not found!" << endl;
}

void searchRiderByName() {
    if (allRiders.empty()) {
        cout << "\nNo riders in system!" << endl;
        return;
    }

    string searchName;
    cout << "\n===== Search Rider by Name =====" << endl;
    cout << "Enter Rider Name: ";
    cin.ignore();
    getline(cin, searchName);

    bool found = false;

    for (size_t i = 0; i < allRiders.size(); i++) {
        if (allRiders[i].Name.find(searchName) != string::npos) {
            if (!found) {
                cout << "\n✓ Rider(s) Found!" << endl;
                found = true;
            }
            cout << "------------------------" << endl;
            cout << "Name: " << allRiders[i].Name << endl;
            cout << "ID: " << allRiders[i].ID << endl;
            cout << "Contact: " << allRiders[i].getContact() << endl;
            cout << "Address: " << allRiders[i].Address << endl;
            cout << "Total Bookings: " << allRiders[i].totalBookings << endl;
            cout << "------------------------" << endl;
        }
    }

    if (!found) {
        cout << "\n No rider found with name containing '" << searchName << "'" << endl;
    }
}

// ==================== RIDE MANAGEMENT ====================

void bookRide() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers available. Please add drivers first!" << endl;
        return;
    }

    if (allRiders.empty()) {
        cout << "\nNo riders registered. Please register as a rider first!" << endl;
        return;
    }

    string pickup, destination, timestamp;
    float distance;
    int riderID;

    cout << "\n========== Book a Ride ==========" << endl;
    cout << "Enter Rider ID: ";
    cin >> riderID;

    int riderIndex = -1;
    for (size_t i = 0; i < allRiders.size(); i++) {
        if (allRiders[i].ID == riderID) {
            riderIndex = i;
            break;
        }
    }

    if (riderIndex == -1) {
        cout << " Rider not found! Please register first." << endl;
        return;
    }

    cout << "Enter Pickup Location: ";
    cin.ignore();
    getline(cin, pickup);

    cout << "Enter Destination: ";
    getline(cin, destination);

    cout << "Enter Distance (km): ";
    cin >> distance;

    cout << "Enter Timestamp (HH:MM): ";
    cin >> timestamp;

    float payment = distance * 100;
    cout << "\nCalculated Fare: Rs." << payment << endl;

    Ride newRide(payment, pickup, distance, destination, timestamp, 0, riderID);
    pendingRides.push(newRide);

    allRiders[riderIndex].incrementBookings();
    saveRidersToFile();
    savePendingRidesToFile();

    cout << " Ride request added to queue!" << endl;
    cout << "Your ride will be assigned to a driver shortly." << endl;
}

void assignDriver() {
    if (pendingRides.empty()) {
        cout << "\nNo pending rides to assign!" << endl;
        return;
    }

    Ride currentRide = pendingRides.front();

    cout << "\n===== Assigning Driver =====" << endl;
    cout << "Ride Details:" << endl;
    cout << "Pickup: " << currentRide.PickUp << endl;
    cout << "Destination: " << currentRide.Destination << endl;
    cout << "Distance: " << currentRide.Distance << " km" << endl;
    cout << "Fare: Rs." << currentRide.getPayment() << endl;

    int driverIndex = -1;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].Availability == 'F') {
            driverIndex = i;
            break;
        }
    }

    if (driverIndex == -1) {
        cout << "\n⚠ No available drivers! Ride remains in queue." << endl;
        return;
    }

    cout << "\n✓ Driver Assigned!" << endl;
    cout << "Driver Name: " << allDrivers[driverIndex].Name << endl;
    cout << "Driver Contact: " << allDrivers[driverIndex].Contact << endl;
    cout << "Driver Rating: " << allDrivers[driverIndex].Ratings << "/5" << endl;

    allDrivers[driverIndex].Availability = 'B';
    currentRide.driverID = allDrivers[driverIndex].ID;
    
    pendingRides.pop();
    pendingRides.push(currentRide);

    saveDriversToFile();
    savePendingRidesToFile();

    cout << "\n✓ Ride assignment complete! Driver is now BUSY." << endl;
}

void completeRide() {
    if (allDrivers.empty()) {
        cout << "\nNo drivers in system!" << endl;
        return;
    }

    int driverID;
    cout << "\n===== Complete Ride =====" << endl;
    cout << "Enter Driver ID who completed the ride: ";
    cin >> driverID;

    int driverIndex = -1;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].ID == driverID) {
            driverIndex = i;
            break;
        }
    }

    if (driverIndex == -1) {
        cout << " Driver not found!" << endl;
        return;
    }

    if (allDrivers[driverIndex].Availability == 'F') {
        cout << " This driver is FREE. No active ride to complete." << endl;
        return;
    }

    queue<Ride> tempQueue;
    Ride rideToComplete;
    bool found = false;

    while (!pendingRides.empty()) {
        Ride r = pendingRides.front();
        pendingRides.pop();
        
        if (r.driverID == driverID && !found) {
            rideToComplete = r;
            found = true;
        } else {
            tempQueue.push(r);
        }
    }

    pendingRides = tempQueue;

    if (!found) {
        cout << " No active ride found for this driver!" << endl;
        return;
    }

    int newRating;
    cout << "Rate the driver (1-5): ";
    cin >> newRating;
    if (newRating >= 1 && newRating <= 5) {
        int totalRides = allDrivers[driverIndex].getTotalRides();
        int currentRating = allDrivers[driverIndex].Ratings;
        int newAvgRating = ((currentRating * totalRides) + newRating) / (totalRides + 1);
        allDrivers[driverIndex].Ratings = newAvgRating;
    }

    allCompletedRides.push_back(rideToComplete);
    allDrivers[driverIndex].Availability = 'F';
    
    float currentEarnings = allDrivers[driverIndex].gettotal_Earning();
    allDrivers[driverIndex].settotal_Earning(currentEarnings + rideToComplete.getPayment());
    allDrivers[driverIndex].incrementRides();

    saveDriversToFile();
    saveCompletedRidesToFile();
    savePendingRidesToFile();

    cout << "\n Ride completed successfully!" << endl;
    cout << "Driver earnings: Rs." << allDrivers[driverIndex].gettotal_Earning() << endl;
    cout << "Driver is now FREE for next ride." << endl;
}

void cancelRide() {
    if (pendingRides.empty()) {
        cout << "\nNo pending rides to cancel!" << endl;
        return;
    }

    int riderID;
    cout << "\n===== Cancel Ride =====" << endl;
    cout << "Enter Rider ID: ";
    cin >> riderID;

    queue<Ride> tempQueue;
    bool found = false;
    Ride cancelledRide;

    while (!pendingRides.empty()) {
        Ride r = pendingRides.front();
        pendingRides.pop();

        if (r.riderID == riderID && !found) {
            cancelledRide = r;
            found = true;
            
            if (r.driverID != 0) {
                for (size_t i = 0; i < allDrivers.size(); i++) {
                    if (allDrivers[i].ID == r.driverID) {
                        allDrivers[i].Availability = 'F';
                        break;
                    }
                }
            }
        } else {
            tempQueue.push(r);
        }
    }

    pendingRides = tempQueue;

    if (found) {
        allCancelledRides.push_back(cancelledRide);
        saveDriversToFile();
        savePendingRidesToFile();
        saveCancelledRidesToFile();
        cout << "\n✓ Ride cancelled successfully!" << endl;
    } else {
        cout << "\n No pending ride found for Rider ID: " << riderID << endl;
    }
}

void viewPendingRides() {
    if (pendingRides.empty()) {
        cout << "\nNo pending rides!" << endl;
        return;
    }

    cout << "\n========== Pending Rides ==========" << endl;
    queue<Ride> temp = pendingRides;
    int count = 1;

    while (!temp.empty()) {
        Ride r = temp.front();
        cout << "\nRide #" << count++ << endl;
        cout << "Pickup: " << r.PickUp << endl;
        cout << "Destination: " << r.Destination << endl;
        cout << "Distance: " << r.Distance << " km" << endl;
        cout << "Fare: Rs." << r.getPayment() << endl;
        cout << "Rider ID: " << r.riderID << endl;
        cout << "Driver ID: " << (r.driverID == 0 ? "Not Assigned" : to_string(r.driverID)) << endl;
        cout << "Time: " << r.timeStamp << endl;
        cout << "------------------------" << endl;
        temp.pop();
    }
}

void viewCompletedRides() {
    if (allCompletedRides.empty()) {
        cout << "\nNo completed rides!" << endl;
        return;
    }

    cout << "\n========== Completed Rides ==========" << endl;
    for (size_t i = 0; i < allCompletedRides.size(); i++) {
        cout << "\nRide #" << (i + 1) << endl;
        cout << "Pickup: " << allCompletedRides[i].PickUp << endl;
        cout << "Destination: " << allCompletedRides[i].Destination << endl;
        cout << "Distance: " << allCompletedRides[i].Distance << " km" << endl;
        cout << "Fare: Rs." << allCompletedRides[i].getPayment() << endl;
        cout << "Driver ID: " << allCompletedRides[i].driverID << endl;
        cout << "Rider ID: " << allCompletedRides[i].riderID << endl;
        cout << "Time: " << allCompletedRides[i].timeStamp << endl;
        cout << "------------------------" << endl;
    }
}

void viewCancelledRides() {
    if (allCancelledRides.empty()) {
        cout << "\nNo cancelled rides!" << endl;
        return;
    }

    cout << "\n========== Cancelled Rides ==========" << endl;
    for (size_t i = 0; i < allCancelledRides.size(); i++) {
        cout << "\nRide #" << (i + 1) << endl;
        cout << "Pickup: " << allCancelledRides[i].PickUp << endl;
        cout << "Destination: " << allCancelledRides[i].Destination << endl;
        cout << "Distance: " << allCancelledRides[i].Distance << " km" << endl;
        cout << "Fare: Rs." << allCancelledRides[i].getPayment() << endl;
        cout << "Rider ID: " << allCancelledRides[i].riderID << endl;
        cout << "Time: " << allCancelledRides[i].timeStamp << endl;
        cout << "------------------------" << endl;
    }
}

// ==================== ANALYTICS AND REPORTING ====================

void generateReport() {
    cout << "\n========================================" << endl;
    cout << "       SYSTEM ANALYTICS REPORT" << endl;
    cout << "========================================" << endl;

    // Total rides
    cout << "\n--- Ride Statistics ---" << endl;
    cout << "Total Completed Rides: " << allCompletedRides.size() << endl;
    cout << "Total Cancelled Rides: " << allCancelledRides.size() << endl;
    
    queue<Ride> temp = pendingRides;
    int pendingCount = 0;
    while (!temp.empty()) {
        pendingCount++;
        temp.pop();
    }
    cout << "Total Pending Rides: " << pendingCount << endl;

    // Total earnings
    float totalEarnings = 0;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        totalEarnings += allDrivers[i].gettotal_Earning();
    }
    cout << "\nTotal System Earnings: Rs." << totalEarnings << endl;

    // Average distance
    if (!allCompletedRides.empty()) {
        float totalDistance = 0;
        for (size_t i = 0; i < allCompletedRides.size(); i++) {
            totalDistance += allCompletedRides[i].Distance;
        }
        cout << "Average Distance per Ride: " << (totalDistance / allCompletedRides.size()) << " km" << endl;
    }

    // Average driver rating
    if (!allDrivers.empty()) {
        float totalRating = 0;
        for (size_t i = 0; i < allDrivers.size(); i++) {
            totalRating += allDrivers[i].Ratings;
        }
        cout << "Average Driver Rating: " << (totalRating / allDrivers.size()) << "/5" << endl;
    }

    // Most active driver
    if (!allDrivers.empty()) {
        int maxRides = 0;
        int maxIndex = 0;
        for (size_t i = 0; i < allDrivers.size(); i++) {
            if (allDrivers[i].getTotalRides() > maxRides) {
                maxRides = allDrivers[i].getTotalRides();
                maxIndex = i;
            }
        }
        if (maxRides > 0) {
            cout << "\n--- Most Active Driver ---" << endl;
            cout << "Name: " << allDrivers[maxIndex].Name << endl;
            cout << "ID: " << allDrivers[maxIndex].ID << endl;
            cout << "Total Rides: " << allDrivers[maxIndex].getTotalRides() << endl;
            cout << "Total Earnings: Rs." << allDrivers[maxIndex].gettotal_Earning() << endl;
        }
    }

    // Highest earning driver
    if (!allDrivers.empty()) {
        float maxEarnings = 0;
        int maxIndex = 0;
        for (size_t i = 0; i < allDrivers.size(); i++) {
            if (allDrivers[i].gettotal_Earning() > maxEarnings) {
                maxEarnings = allDrivers[i].gettotal_Earning();
                maxIndex = i;
            }
        }
        if (maxEarnings > 0) {
            cout << "\n--- Highest Earning Driver ---" << endl;
            cout << "Name: " << allDrivers[maxIndex].Name << endl;
            cout << "ID: " << allDrivers[maxIndex].ID << endl;
            cout << "Total Earnings: Rs." << allDrivers[maxIndex].gettotal_Earning() << endl;
        }
    }

    // Rider with most bookings
    if (!allRiders.empty()) {
        int maxBookings = 0;
        int maxIndex = 0;
        for (size_t i = 0; i < allRiders.size(); i++) {
            if (allRiders[i].totalBookings > maxBookings) {
                maxBookings = allRiders[i].totalBookings;
                maxIndex = i;
            }
        }
        if (maxBookings > 0) {
            cout << "\n--- Most Active Rider ---" << endl;
            cout << "Name: " << allRiders[maxIndex].Name << endl;
            cout << "ID: " << allRiders[maxIndex].ID << endl;
            cout << "Total Bookings: " << allRiders[maxIndex].totalBookings << endl;
        }
    }

    cout << "\n========================================" << endl;
}

void exportReportToFile() {
    ofstream file("system_report.txt");
    if (!file.is_open()) {
        cout << "Error: Could not create report file!" << endl;
        return;
    }

    file << "========================================" << endl;
    file << "    UBER MANAGEMENT SYSTEM REPORT" << endl;
    file << "========================================" << endl;

    file << "\n--- Ride Statistics ---" << endl;
    file << "Total Completed Rides: " << allCompletedRides.size() << endl;
    file << "Total Cancelled Rides: " << allCancelledRides.size() << endl;

    queue<Ride> temp = pendingRides;
    int count = 0;
    while (!temp.empty()) {
        count++;
        temp.pop();
    }
    file << "Total Pending Rides: " << count << endl;

    float totalEarnings = 0;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        totalEarnings += allDrivers[i].gettotal_Earning();
    }
    file << "\nTotal System Earnings: Rs." << totalEarnings << endl;

    if (!allCompletedRides.empty()) {
        float totalDistance = 0;
        for (size_t i = 0; i < allCompletedRides.size(); i++) {
            totalDistance += allCompletedRides[i].Distance;
        }
        file << "Average Distance per Ride: " << (totalDistance / allCompletedRides.size()) << " km" << endl;
    }

    file << "\n--- All Drivers ---" << endl;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        file << "\nDriver " << (i + 1) << ":" << endl;
        file << "Name: " << allDrivers[i].Name << endl;
        file << "ID: " << allDrivers[i].ID << endl;
        file << "Rating: " << allDrivers[i].Ratings << "/5" << endl;
        file << "Total Rides: " << allDrivers[i].getTotalRides() << endl;
        file << "Total Earnings: Rs." << allDrivers[i].gettotal_Earning() << endl;
    }

    file.close();
    cout << "\n✓ Report exported to 'system_report.txt' successfully!" << endl;
}

// ==================== MENU SYSTEM ====================

void adminMenu();
void driverMenu();
void riderMenu();

void mainMenu() {
    int choice;

    while (true) {
        cout << "\n============================================" << endl;
        cout << "    UBER MANAGEMENT SYSTEM - LOGIN" << endl;
        cout << "============================================" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Driver Login" << endl;
        cout << "3. Rider Login" << endl;
        cout << "4. Exit System" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                adminMenu();
                break;
            case 2:
                driverMenu();
                break;
            case 3:
                riderMenu();
                break;
            case 4:
                cout << "\nSaving all data before exit..." << endl;
                saveAllData();
                cout << "Thank you for using Uber Management System!" << endl;
                return;
            default:
                cout << "\n Invalid choice! Please try again." << endl;
        }
    }
}

void adminMenu() {
    int choice;

    while (true) {
        cout << "\n============================================" << endl;
        cout << "          ADMIN DASHBOARD" << endl;
        cout << "============================================" << endl;
        cout << "1.  Add Driver" << endl;
        cout << "2.  Add Rider" << endl;
        cout << "3.  View All Drivers" << endl;
        cout << "4.  Update Driver" << endl;
        cout << "5.  Search Driver by ID" << endl;
        cout << "6.  Search Driver by Name" << endl;
        cout << "7.  Search Rider by ID" << endl;
        cout << "8.  Search Rider by Name" << endl;
        cout << "9.  Sort Drivers by Name" << endl;
        cout << "10. Sort Drivers by Rating" << endl;
        cout << "11. View Pending Rides" << endl;
        cout << "12. View Completed Rides" << endl;
        cout << "13. View Cancelled Rides" << endl;
        cout << "14. Generate Analytics Report" << endl;
        cout << "15. Export Report to File" << endl;
        cout << "16. Save All Data" << endl;
        cout << "17. Back to Main Menu" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: addDriver(); break;
            case 2: addRider(); break;
            case 3: viewAllDrivers(); break;
            case 4: updateDriver(); break;
            case 5: searchDriverByID(); break;
            case 6: searchDriverByName(); break;
            case 7: searchRiderByID(); break;
            case 8: searchRiderByName(); break;
            case 9: sortDriversByName(); break;
            case 10: sortDriversByRating(); break;
            case 11: viewPendingRides(); break;
            case 12: viewCompletedRides(); break;
            case 13: viewCancelledRides(); break;
            case 14: generateReport(); break;
            case 15: exportReportToFile(); break;
            case 16: saveAllData(); break;
            case 17: return;
            default: cout << "\n Invalid choice!" << endl;
        }
    }
}

void driverMenu() {
    int driverID;
    cout << "\nEnter your Driver ID: ";
    cin >> driverID;

    int driverIndex = -1;
    for (size_t i = 0; i < allDrivers.size(); i++) {
        if (allDrivers[i].ID == driverID) {
            driverIndex = i;
            break;
        }
    }

    if (driverIndex == -1) {
        cout << " Driver ID not found!" << endl;
        return;
    }

    int choice;
    while (true) {
        cout << "\n============================================" << endl;
        cout << "        DRIVER DASHBOARD" << endl;
        cout << "   Welcome, " << allDrivers[driverIndex].Name << "!" << endl;
        cout << "============================================" << endl;
        cout << "1. View My Profile" << endl;
        cout << "2. View Pending Rides" << endl;
        cout << "3. Complete My Ride" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n--- Your Profile ---" << endl;
                cout << "Name: " << allDrivers[driverIndex].Name << endl;
                cout << "ID: " << allDrivers[driverIndex].ID << endl;
                cout << "Contact: " << allDrivers[driverIndex].Contact << endl;
                cout << "Rating: " << allDrivers[driverIndex].Ratings << "/5" << endl;
                cout << "Status: " << (allDrivers[driverIndex].Availability == 'F' ? "Free" : "Busy") << endl;
                cout << "Total Earnings: Rs." << allDrivers[driverIndex].gettotal_Earning() << endl;
                cout << "Total Rides: " << allDrivers[driverIndex].getTotalRides() << endl;
                break;
            case 2:
                viewPendingRides();
                break;
            case 3:
                completeRide();
                break;
            case 4:
                return;
            default:
                cout << "\n Invalid choice!" << endl;
        }
    }
}

void riderMenu() {
    int riderID;
    cout << "\nEnter your Rider ID (or 0 to register): ";
    cin >> riderID;

    if (riderID == 0) {
        addRider();
        return;
    }

    int riderIndex = -1;
    for (size_t i = 0; i < allRiders.size(); i++) {
        if (allRiders[i].ID == riderID) {
            riderIndex = i;
            break;
        }
    }

    if (riderIndex == -1) {
        cout << " Rider ID not found! Please register first." << endl;
        return;
    }

    int choice;
    while (true) {
        cout << "\n============================================" << endl;
        cout << "         RIDER DASHBOARD" << endl;
        cout << "   Welcome, " << allRiders[riderIndex].Name << "!" << endl;
        cout << "============================================" << endl;
        cout << "1. Book a Ride" << endl;
        cout << "2. Cancel My Ride" << endl;
        cout << "3. View My Bookings" << endl;
        cout << "4. Assign Driver to My Ride" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bookRide();
                break;
            case 2:
                cancelRide();
                break;
            case 3:
                cout << "\nTotal Bookings: " << allRiders[riderIndex].totalBookings << endl;
                break;
            case 4:
                assignDriver();
                break;
            case 5:
                return;
            default:
                cout << "\n Invalid choice!" << endl;
        }
    }
}

// ==================== MAIN FUNCTION ====================

int main() {
    cout << "========================================" << endl;
    cout << "   UBER MANAGEMENT SYSTEM v1.0" << endl;
    cout << "========================================" << endl;
    cout << "\nInitializing system..." << endl;

    loadAllData();

    mainMenu();

    return 0;
}

