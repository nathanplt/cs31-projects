//
//  rental.cpp
//  Project 2
//
//  Created by Nathan Zhang on 10/14/24.
//

#include <iostream>
#include <string>
using namespace std;

int main(int argc, const char * argv[]) {
    // Handling all inputs and errors
    int start;
    cout << "Odometer at start: ";
    cin >> start;
    if (start < 0){
        cout << "---" << '\n';
        cout << "The starting odometer reading must not be negative." << '\n';
        return 1;
    }
    
    int end;
    cout << "Odometer at end: ";
    cin >> end;
    if (end < start){
        cout << "---" << '\n';
        cout << "The ending odometer reading must be at least as large as the starting reading." << '\n';
        return 1;
    }
    
    int days;
    cout << "Rental days: ";
    cin >> days;
    if (days <= 0){
        cout << "---" << '\n';
        cout << "The number of rental days must be positive." << '\n';
        return 1;
    }
    
    // Using cin.ignore to use getline after inputting ints
    cin.ignore(10000, '\n');
    
    string name;
    cout << "Customer name: ";
    getline(cin, name);
    if (name.length() == 0){
        cout << "---" << '\n';
        cout << "You must enter a customer name." << '\n';
        return 1;
    }
    
    string luxury;
    cout << "Luxury car? (y/n): ";
    getline(cin, luxury);
    if (luxury != "y" && luxury != "n"){
        cout << "---" << '\n';
        cout << "You must enter y or n." << '\n';
        return 1;
    }
    
    int month;
    cout << "Starting month (1=Jan, 2=Feb, etc.): ";
    cin >> month;
    if (month < 1 || month > 12){
        cout << "---" << '\n';
        cout << "The month number must be in the range 1 through 12." << '\n';
        return 1;
    }
    
    // Calculating the base rate based on the car's luxury status
    int base_rate = 0;
    if (luxury == "y"){
        base_rate = 75 * days;
    } else {
        base_rate = 45 * days;
    }
    
    // Calculating the additional cost of the miles driven
    // rental_charge holds the total cost
    double rental_charge = base_rate;
    int miles = end - start;
    
    // Adding the cost of the first 100 miles driven
    // The min function is utilized to deal with the case where the car has driven less than 100 miles
    int first_miles = min(miles, 100);
    rental_charge += 0.33 * first_miles;
    miles -= first_miles;
    
    // Adding the cost of the next 500 miles driven (if there are any additional miles)
    int second_miles = min(miles, 500);
    
    // This comparison is true when month = 1, 2, 3, or 12 which is exactly what we want (the Winter months)
    // To show this: 1 % 12 = 1, 2 % 12 = 2, 3 % 12 = 3, 12 % 12 = 0
    if (month % 12 <= 3){
        rental_charge += 0.33 * second_miles;
    } else {
        rental_charge += 0.27 * second_miles;
    }
    miles -= second_miles;
    
    // Adding the cost of any additional mile driven past the 600 mile mark
    rental_charge += 0.21 * miles;
    
    // Setting the precision of cout to two decimal points
    cout.setf(ios :: fixed);
    cout.precision(2);
    
    // Outputting the answer
    cout << "---" << '\n';
    cout << "The rental charge for " << name << " is $" << rental_charge << '\n';
}
