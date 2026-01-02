/*
NOTES:
comments starting with '=>' are for explanation purposes of members. These comments must be removed before final submission of project.
This code is not structured yet, neither the UI is implemented.
For Now our main focus is just to implement the core logic.
Terms:
 'book': refers to 'Discrete Mathematics and it's Applications' by 'Kenneth H. Rosen'
*/

#include <iostream>
#include <cstdlib>

using namespace std;

typedef long long bigInt;

const string INDENTATION = "\t\t";


// ************ Utility Functions **************
void clearScreen();
int ToInt(string);
void PressEnterToContinue(string message = "Press Enter to continue");
int GetChoice(string text = "Enter choice: ");
int MenuBuilder(string[], int);
bool IsValidInt(string);
bool IsValidChoice(int, int, int);


// ************ UI Functions ******************
void showMainMenu();
void showLogo();


// ************ Main Logic Functions ******************
bigInt normalizeMod(bigInt, bigInt);
bigInt gcd(bigInt, bigInt);
bigInt modularExponentiation(bigInt, bigInt, bigInt);
bigInt extendedGcd(bigInt, bigInt, bigInt&, bigInt&);
bigInt modInverse(bigInt, bigInt);
bigInt linearCongruence(bigInt, bigInt, bigInt);
bigInt chineseRemainderTheorem(bigInt[][2], bigInt&, int);

int main() {
    int choice;
    do{
        showMainMenu();
    }while(choice);
}

// ************ Utility Functions **************

void clearScreen(){
    #if defined(WIN32_)
        system("cls");
    #else
        system("clear");
    #endif
}

// Our implementation of 'string-to-integer' function.
//
// Args:
//   str: The string to convert.
// Returns:
//   The converted integer.
int ToInt(string str) {
  int num = 0;
  bool isNegative = false;
  for (int i = 0; i < str.length(); i++) {
    if(i==0 && str[i]=='-') {
      isNegative = true;
      continue;
    }
    num = num * 10 + (str[i] - '0');
  }

  if(isNegative)
    return -num;
  return num;
}

void PressEnterToContinue(string message) {
  char temp;
  cout << endl << INDENTATION << message;
  cin.get(temp);
}

// Prompts the user for a numeric menu choice and returns it.
//
// Args:
//   text: The prompt text to display.
// Returns:
//   The chosen integer.
int GetChoice(string text) {
  int choice;
  string s_choice;
  do {
    cout << "\n" << INDENTATION << text;
    getline(cin, s_choice);
  } while (!IsValidInt(s_choice));
  choice = ToInt(s_choice);
  return choice;
}

// Validates whether a string represents a valid integer (optionally negative).
//
// Args:
//   str: The string to validate.
// Returns:
//   True if str is an integer, otherwise false.
bool IsValidInt(string str) {
  if (!str.length()) {
    cout << "\n" << INDENTATION << "Invalid integer input!";

    PressEnterToContinue("Press enter to try again.");
    return false;
  }
  for (int i = 0; i < str.length(); i++) {
    if (i == 0 && str[i] == '-') continue;
    if (str[i] < '0' || str[i] > '9') {
      cout << "\n" << INDENTATION << "Invalid integer input!";

      PressEnterToContinue("Press enter to try again.");
      return false;
    }
  }
  return true;
}

// Validates that a numeric choice is within [start, end].
//
// Args:
//   choice: The choice to validate.
//   start: Minimum valid value.
//   end: Maximum valid value.
// Returns:
//   True if choice is valid, otherwise false.
bool IsValidChoice(int choice, int start, int end) {
  if (choice < start || choice > end) {
    cout << "\n"
         << INDENTATION << "Invalid Choice!"
         << "\n"
         << INDENTATION << "Make a choice between '" << start << "' and '"
         << end << "'.";

    PressEnterToContinue("Press enter to try again.");
    return false;
  }
  return true;
}

// Displays a numbered menu and returns the user's choice.
//
// Args:
//   options: Array of option strings.
//   size: Number of options.
// Returns:
//   The selected option index (1-based).
int MenuBuilder(string options[], int size) {
  int choice;
  do {
    cout << "\n\n";
    for (int i = 0; i < size; i++) {
      cout << INDENTATION << i + 1 << ". " << options[i] << "\n";
    }

    choice = GetChoice();
  } while (!IsValidChoice(choice, 1, size));
  return choice;
}

// ************ UI Functions ******************

void showMainMenu(){
    int choice;
    string options[] = {"Modular exponentiation (b^n mod m)",
                        "Modular inverse (a^-1 mod m)",
                        "Solve linear congruence ax ≡ b (mod m)",
                        "Chinese Remainder Theorem (multiple congruences)",
                        "Applications of congruences",
                        "Run sample tests",
                        "Exit program"};
    clearScreen();
    cout<<endl;
    showLogo();
    choice = MenuBuilder(options,7);
    switch (choice)
    {
    case 7:
        exit(0);
        break;
    
    default:
        break;
    }
    
}

void showLogo(){
    cout<<INDENTATION<<"+++++++++++++++++++++++++++++++++"<<endl;
    cout<<INDENTATION<<"+ Modular Arithmetic Calculator +"<<endl;
    cout<<INDENTATION<<"+++++++++++++++++++++++++++++++++"<<endl;
}


/*
=> This function just returns the +ve equivalent of a mod m.
like if we take -3 mod 11, the result will be -3, but
by using this function, the result will be 8
*/
bigInt normalizeMod(bigInt a, bigInt m){
return (a % m + m) % m;
}

bigInt gcd(bigInt a, bigInt b) {
    if (a == 0)
        return b;
    return gcd(b % a, a);
}


// Computes (b^n) mod m using fast modular exponentiation
/*
=> The algorith from this function was given in the book's chapter number 4 'Number theroy and cryptography', section 4.2.4 (Page 268 of 8th edition).
This algorithm uses the concept of binary expansion
*/
bigInt modularExponentiation(bigInt b, bigInt n, bigInt m)
{
    bigInt x = 1;
    bigInt power = b % m;

    while (n > 0)
    {
        if (n % 2 == 1) // if current bit a_i = 1
            x = (x * power) % m;

        power = (power * power) % m; // square for next bit
        n /= 2;                      // move to next bit
    }

    return x; // equals b^n mod m
}

// extended gcd: returns gcd(a,b) and sets x,y such that a*x + b*y = gcd(a,b)
/*
=> The algorithm for this function is learned online (https://www.geeksforgeeks.org/dsa/euclidean-algorithms-basic-and-extended/).
*/
bigInt extendedGcd(bigInt a, bigInt b, bigInt &x, bigInt &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    if(a>=b){
        // cout<<a<<" = "<<b<<"*"<<a/b<<" + "<<a%b<<endl; // => Prints steps while calculating gcd
    }
    bigInt x1, y1;
    bigInt g = extendedGcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    // cout<<g<<" = "<<a<<"*"<<x<<" + "<<b<<"*"<<y<<endl; // => Print recursive steps
    return g;
}


// modular inverse of a modulo m using Extended Euclidean Algorithm
// returns inverse in range [0, m-1] if it exists, otherwise returns -1
/*
Just a simple function, using 'extended_gcd' to calculate inverse of a mod m.
*/
bigInt modInverse(bigInt a, bigInt m) {
    a = normalizeMod(a,m);               // normalize a
    bigInt x, y;
    bigInt g = extendedGcd(a, m, x, y);
    if (g != 1) return -1;             // inverse doesn't exist when gcd != 1
    return normalizeMod(x,m);            // ensure positive result
}

/*
=> This function solves linearCongruence of Form ax≡b (mod m). Just give the value of a, b and m and this function returns (if possible) the value of x, otherwise -1;
*/
bigInt linearCongruence(bigInt a, bigInt b, bigInt m){
    bigInt g = gcd(a,m);
   
        if(b%g){
            return -1;// => solution does not exist
        }
        if(g!=1){
            a = a / g;
            b = b / g;
            m = m / g;
        }
    

    bigInt inv = modInverse(a,m);
    return (b*inv)%m;
}


/*
=> Reference to learn: https://brilliant.org/wiki/chinese-remainder-theorem/
*/
bigInt chineseRemainderTheorem(bigInt equations[][2], bigInt &M, int k){
    int x = 0;
    M = 1;
    bigInt y, z;
    for (int i = 0; i < k; i++)
    {
        M *= equations[i][1];
        if(i<k-1 && gcd(equations[i][1],equations[i+1][1])!=1){ // => CRT could not apply as mods are not relatively co-prime.
            M = -1;
            return -1;
        }
    }
    for (int i = 0; i < k; i++)
    {
        y = M/equations[i][1];
        z = modInverse(y,equations[i][1]);
        x += (equations[i][0]*y*z);
    }
    x %= M;
    if (x < 0) x += M;
    return x;
}
