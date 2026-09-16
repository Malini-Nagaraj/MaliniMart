#include <iostream>
#include <string>

using namespace std;

int main()
{
    string email;
    string password;

    cout << "====================================\n";
    cout << "          MALINIMART LOGIN\n";
    cout << "====================================\n";

    cout << "Enter Email    : ";
    cin >> email;

    cout << "Enter Password : ";
    cin >> password;

    if (email == "malini@gmail.com" && password == "test123")
    {
        cout << "\nLogin successful!\n";
        cout << "Welcome Malini!\n";
    }
    else
    {
        cout << "\nInvalid email or password!\n";
    }

    return 0;
}