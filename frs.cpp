#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cctype>
using namespace std;

const int TOTAL_SEATS = 100;

class Flight {
private:
    struct Passenger {
        string fname, lname, ID, phone_number, food_menu, booking_time;
        int seat_number, reservation_number;
        Passenger* next;
    };

    Passenger* head;
    int seat_status[TOTAL_SEATS] = {0};
    int current_reservation = 1000;

    void display_available_seats() const {
        cout << "\n========== Available Seats ==========\n";
        for (int i = 0; i < TOTAL_SEATS; ++i) {
            if (seat_status[i] == 0)
                cout << "|" << setw(2) << i + 1 << "|";
            else
                cout << "| X|";

            if ((i + 1) % 10 == 0) cout << endl;
        }
        cout << "=====================================\n";
    }

    string get_current_time() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[30];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", ltm);
        return string(buffer);
    }

    bool is_valid_phone(const string& phone) {
        if (phone.length() != 10) return false;
        for (char ch : phone)
            if (!isdigit(ch)) return false;
        return true;
    }

    bool is_valid_name(const string& name) {
        for (char ch : name)
            if (!isalpha(ch)) return false;
        return true;
    }

public:
    Flight() : head(nullptr) {}
    ~Flight();

    void book_ticket();
    void cancel_ticket();
    void change_reservation();
    void passenger_details();
    void get_booking_details();  
    void get_my_bookings();      
};

Flight::~Flight() {
    while (head) {
        Passenger* temp = head;
        head = head->next;
        delete temp;
    }
}

void Flight::book_ticket() {
    Passenger* new_passenger = new Passenger;

    cout << "\n--- Booking Ticket ---\n";

    do {
        cout << "Enter First Name        : ";
        cin >> new_passenger->fname;
        if (!is_valid_name(new_passenger->fname))
            cout << "Invalid name.\n";
    } while (!is_valid_name(new_passenger->fname));

    do {
        cout << "Enter Last Name         : ";
        cin >> new_passenger->lname;
        if (!is_valid_name(new_passenger->lname))
            cout << "Invalid name. Only letters allowed.\n";
    } while (!is_valid_name(new_passenger->lname));

    cout << "Enter ID                : ";
    cin >> new_passenger->ID;

    do {
        cout << "Enter Phone Number (10 digits): ";
        cin >> new_passenger->phone_number;
        if (!is_valid_phone(new_passenger->phone_number))
            cout << "Invalid phone number. Try again.\n";
    } while (!is_valid_phone(new_passenger->phone_number));

    display_available_seats();
    int seat;
    do {
        cout << "Choose seat number (1-" << TOTAL_SEATS << "): ";
        cin >> seat;
    } while (seat < 1 || seat > TOTAL_SEATS || seat_status[seat - 1]);

    seat_status[seat - 1] = 1;
    new_passenger->seat_number = seat;

    int food_choice;
    cout << "\nFood Options:\n1. Veg\n2. Non-Veg\n3. No Food\nChoice: ";
    cin >> food_choice;
    new_passenger->food_menu = (food_choice == 1) ? "Veg" : (food_choice == 2) ? "Non-Veg" : "No Food";

    new_passenger->reservation_number = ++current_reservation;
    new_passenger->booking_time = get_current_time();
    new_passenger->next = nullptr;

    cout << "\nReservation Confirmed!\n";
    cout << "Reservation Number: " << new_passenger->reservation_number << "\n";
    cout << "Booking Time      : " << new_passenger->booking_time << "\n";

    if (!head) head = new_passenger;
    else {
        Passenger* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = new_passenger;
    }
}

void Flight::cancel_ticket() {
    int res_no;
    cout << "\n--- Cancel Ticket ---\n";
    cout << "Enter Reservation Number: ";
    cin >> res_no;

    Passenger* temp = head, *prev = nullptr;
    while (temp && temp->reservation_number != res_no) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        cout << "Reservation not found.\n";
        return;
    }

    seat_status[temp->seat_number - 1] = 0;
    if (!prev) head = temp->next;
    else prev->next = temp->next;

    delete temp;
    cout << "Reservation cancelled successfully.\n";
}

void Flight::change_reservation() {
    int old_seat;
    cout << "\n--- Change Reservation ---\n";
    cout << "Enter your current seat number: ";
    cin >> old_seat;

    Passenger* temp = head;
    while (temp && temp->seat_number != old_seat)
        temp = temp->next;

    if (!temp) {
        cout << "Passenger not found.\n";
        return;
    }

    display_available_seats();
    int new_seat;
    do {
        cout << "Enter new seat number: ";
        cin >> new_seat;
    } while (new_seat < 1 || new_seat > TOTAL_SEATS || seat_status[new_seat - 1]);

    seat_status[old_seat - 1] = 0;
    seat_status[new_seat - 1] = 1;
    temp->seat_number = new_seat;

    cout << "Seat changed successfully to seat " << new_seat << ".\n";
}

void Flight::passenger_details() {
    int res_no;
    cout << "\n--- Passenger Details ---\n";
    cout << "Enter Reservation Number: ";
    cin >> res_no;

    Passenger* temp = head;
    while (temp) {
        if (temp->reservation_number == res_no) {
            cout << "\n===== Passenger Information =====\n";
            cout << "Name         : " << temp->fname << " " << temp->lname << "\n";
            cout << "ID           : " << temp->ID << "\n";
            cout << "Phone        : " << temp->phone_number << "\n";
            cout << "Seat Number  : " << temp->seat_number << "\n";
            cout << "Food Choice  : " << temp->food_menu << "\n";
            cout << "Booked At    : " << temp->booking_time << "\n";
            cout << "Reservation# : " << temp->reservation_number << "\n";
            cout << "=================================\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Passenger not found.\n";
}

void Flight::get_booking_details() {
    Passenger* temp = head;
    cout << "\n=========== All Bookings ===========\n";
    if (!temp) {
        cout << "No bookings available.\n";
        return;
    }

    cout << left << setw(10) << "Res#"
         << setw(15) << "Name"
         << setw(10) << "Seat"
         << setw(10) << "Food"
         << "Booked At\n";
    cout << "-------------------------------------------------------------\n";

    while (temp) {
        cout << left << setw(10) << temp->reservation_number
             << setw(15) << (temp->fname + " " + temp->lname)
             << setw(10) << temp->seat_number
             << setw(10) << temp->food_menu
             << temp->booking_time << endl;
        temp = temp->next;
    }
    cout << "=============================================================\n";
}

void Flight::get_my_bookings() {
    string phone;
    cout << "\n--- View My Bookings ---\n";
    cout << "Enter your phone number: ";
    cin >> phone;

    if (!is_valid_phone(phone)) {
        cout << "Invalid phone number.\n";
        return;
    }

    Passenger* temp = head;
    bool found = false;
    cout << "\nYour Bookings:\n";
    cout << left << setw(10) << "Res#"
         << setw(15) << "Name"
         << setw(10) << "Seat"
         << setw(10) << "Food"
         << "Booked At\n";
    cout << "-------------------------------------------------------------\n";

    while (temp) {
        if (temp->phone_number == phone) {
            found = true;
            cout << left << setw(10) << temp->reservation_number
                 << setw(15) << (temp->fname + " " + temp->lname)
                 << setw(10) << temp->seat_number
                 << setw(10) << temp->food_menu
                 << temp->booking_time << endl;
        }
        temp = temp->next;
    }

    if (!found)
        cout << "No bookings found for this number.\n";
    cout << "=============================================================\n";
}

void show_menu() {
    Flight flight;
    int choice;
    do {
        cout << "\n=========== Flight Reservation Menu ===========\n";
        cout << "1. Book Ticket\n";
        cout << "2. Cancel Ticket\n";
        cout << "3. Change Reservation\n";
        cout << "4. Passenger Details\n";
        cout << "5. View All Bookings\n";
        cout << "6. View My Bookings\n";
        cout << "0. Exit\n";
        cout << "===============================================\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: flight.book_ticket(); break;
            case 2: flight.cancel_ticket(); break;
            case 3: flight.change_reservation(); break;
            case 4: flight.passenger_details(); break;
            case 5: flight.get_booking_details(); break;
            case 6: flight.get_my_bookings(); break;
            case 0: cout << "\nExiting system. Thank you!\n"; break;
            default: cout << "Invalid option. Please try again.\n";
        }
    } while (choice != 0);
}

int main() {
    show_menu();
    return 0;
}
