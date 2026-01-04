/*
Terms:
 'book': refers to 'Discrete Mathematics and it's Applications' by 'Kenneth H. Rosen'
*/

#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <climits>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

typedef long long bigInt;

// UI Constants - ANSI Colors
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";
const string BOLD = "\033[1m";
const string MAGENTA = "\033[35m";

bool showSteps = false;

struct TestCase
{
    bigInt b, n, m, expected, result;
};

// ************ Utility Functions **************
void setupConsole();
void clearScreen();
void drawLine(int width = 60);
void printCentered(string text, string color = RESET, bool newLine = true);
void sleep(int);
bigInt toNumber(string);
void pressEnterToContinue(string message = "Press Enter to continue");
int getChoice(string text = "Enter choice: ");
int menuBuilder(string[], int, string = "Menu");
bool isValidNumber(string, bool isMod = false);
bool isValidChoice(int, int, int);
bigInt getNumberInput(string text = "Enter input", bool isMod = false);
bool isValidBinaryString(string);
bool isValidISBN(string);
void typeWrite(string text, int speed = 20);
void printRightAligned(string, string);

// ************ UI Functions ******************
void showLoadingScreen();
void showLogo();
void showHeader(string);
void showScreenIntro(string, string);
void showMainMenu();
void showApplicationsMenu();
void showModularExpScreen();
void showModInverseScreen();
void showLinearCongruenceScreen();
void showCRTScreen();
void showParityBitScreen();
void showUPCScreen();
void showISBNScreen();

// ************ Main Logic Functions ******************
bigInt gcd(bigInt, bigInt);
bigInt modularExponentiation(bigInt, bigInt, bigInt);
bigInt extendedGcd(bigInt, bigInt, bigInt &, bigInt &, bool showSteps);
bigInt modInverse(bigInt, bigInt);
bigInt linearCongruence(bigInt, bigInt, bigInt);
bigInt chineseRemainderTheorem(bigInt **, bigInt &, int);
void runSampleTests();
bool verifyCongruence(bigInt, bigInt, bigInt);

int main()
{
    setupConsole();
    srand(time(0));
    showLoadingScreen();
    do
    {
        showMainMenu();
    } while (true);
}

// ************ Utility Functions **************

void setupConsole()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
}

void clearScreen()
{
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void sleep(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
#endif
}

// Our implementation of 'string-to-integer' function.
//
// Args:
//   str: The string to convert.
// Returns:
//   The converted integer.
bigInt toNumber(string str)
{
    bigInt num = 0;
    bool isNegative = false;
    for (int i = 0; i < str.length(); i++)
    {
        if (i == 0 && str[i] == '-')
        {
            isNegative = true;
            continue;
        }
        num = num * 10 + (str[i] - '0');
    }

    if (isNegative)
        return -num;
    return num;
}

void pressEnterToContinue(string message)
{
    char temp;
    cout << "\n";
    printCentered(message, CYAN, false);
    cin.get(temp);
}

// Prompts the user for a numeric menu choice and returns it.
//
// Args:
//   text: The prompt text to display.
// Returns:
//   The chosen integer.
int getChoice(string text)
{
    int choice;
    string s_choice;
    do
    {
        cout << "\n";
        printCentered(text, CYAN);
        cout << string(43, ' ') << ">> ";
        getline(cin, s_choice);
    } while (!isValidNumber(s_choice));
    choice = toNumber(s_choice);
    return choice;
}

// Validates whether a string represents a valid integer (optionally negative).
//
// Args:
//   str: The string to validate.
// Returns:
//   True if str is an integer, otherwise false.
bool isValidNumber(string str, bool isMod)
{
    bool allZeros = true;
    if (!str.length())
    {
        printCentered("Invalid number input!", RED);

        pressEnterToContinue("Press enter to try again.");
        return false;
    }
    for (int i = 0; i < str.length(); i++)
    {
        if (i == 0 && str[i] == '-')
        {
            if (isMod)
            {
                printCentered("'mod' can't be negative", RED);
                pressEnterToContinue("Press enter to try again.");
                return false;
            }

            continue;
        }
        if (str[i] < '0' || str[i] > '9')
        {
            printCentered("Invalid number input", RED);

            pressEnterToContinue("Press enter to try again.");
            return false;
        }
        if (str[i] != '0')
            allZeros = false;
    }
    if (isMod && allZeros)
    {
        printCentered("'mod' can't be zero", RED);

        pressEnterToContinue("Press enter to try again.");
        return false;
    }
    return true;
}

bool isValidBinaryString(string str)
{
    if (!str.length())
    {
        printCentered("Invalid input", RED);

        pressEnterToContinue("Press enter to try again.");
        return false;
    }
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] != '0' && str[i] != '1')
        {
            printCentered("Invalid binary string", RED);

            pressEnterToContinue("Press enter to try again.");
            return false;
        }
    }
    return true;
}

bool isValidISBN(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == 'X')
        {
            continue;
        }
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    return true;
}

// This function prints text to the right of the console
void printRightAligned(string text, string color)
{
    int padding = 25; // fixed padding for right alignment
    cout << string(padding, ' ') << color << text << RESET << "\n";
}

// Validates that a numeric choice is within [start, end].
//
// Args:
//   choice: The choice to validate.
//   start: Minimum valid value.
//   end: Maximum valid value.
// Returns:
//   True if choice is valid, otherwise false.
bool isValidChoice(int choice, int start, int end)
{
    if (choice < start || choice > end)
    {
        printCentered("Invalid Choice!", RED);
        printCentered("Make a choice between '" + to_string(start) + "' and '" + to_string(end) + "'.", RED);

        pressEnterToContinue("Press enter to try again.");
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
int menuBuilder(string options[], int size, string title)
{
    int choice;
    do
    {
        clearScreen();
        showLogo();
        showHeader(title);
        cout << "\n\n";
        for (int i = 0; i < size; i++)
        {
            string optionLine = "[ " + to_string(i + 1) + " ]  " + options[i];
            printRightAligned(optionLine, RESET);
            sleep(50);
        }

        choice = getChoice();
    } while (!isValidChoice(choice, 1, size));
    return choice;
}

bigInt getNumberInput(string text, bool isMod)
{
    string input;
    bigInt n;
    do
    {
        printCentered(text + ": ", YELLOW, false);
        getline(cin, input);
    } while (!isValidNumber(input, isMod));
    return toNumber(input);
}

// This function validates whether a string is a valid ISBN number.
void drawLine(int width)
{
    int padding = (100 - width) / 2;
    if (padding < 0)
        padding = 0;
    cout << string(padding, ' ') << CYAN << string(width, '=') << RESET << "\n";
}

// This function prints centered text in the console.
void printCentered(string text, string color, bool newLine)
{
    int width = 100;
    int padding = (width - text.length()) / 2;
    if (padding < 0)
        padding = 0;
    cout << string(padding, ' ') << color << text << RESET;
    if (newLine)
    {
        cout << "\n";
    }
}

// This function simulates typing effect
void typeWrite(string text, int speed)
{
    int padding = 25;

    cout << string(padding, ' ');

    for (int i = 0; i < text.length();)
    {
        
        if (i + 2 < text.length())
        { // This is a naive workaround to print '≡' properly
            unsigned char b1 = text[i];
            unsigned char b2 = text[i + 1];
            unsigned char b3 = text[i + 2];

            if (b1 == 0xE2 && b2 == 0x89 && b3 == 0xA1)
            {
                cout << YELLOW << "≡" << RESET << flush;
                i += 3; 
                sleep(speed);
                continue;
            }
        }

        cout << YELLOW << text[i] << RESET << flush;

        if (text[i] == '\n')
            cout << string(padding, ' ');

        i += 1;
        sleep(speed);
    }

    cout << "\n";
}

// ************ UI Functions ******************

void showLoadingScreen()
{
    clearScreen();
    cout << "\n\n\n\n\n";
    showLogo();
    cout << "\n\n";

    string title = "Initializing Modular System...";
    printCentered(title, CYAN);
    cout << "\n";

    int barWidth = 40;
    int pad = (80 - barWidth) / 2;

    for (int i = 0; i <= 100; i += 2)
    {
        cout << "\r" << string(pad, ' ');
        cout << GREEN << "[";

        int pos = (i * barWidth) / 100;
        for (int j = 0; j < barWidth; ++j)
        {
            if (j < pos)
                cout << "=";
            else if (j == pos)
                cout << ">";
            else
                cout << " ";
        }
        cout << "] " << i << "%" << RESET << flush;
        sleep(20);
    }
    sleep(500);
}

void showLogo()
{
    cout << CYAN << "\t" << "          ███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗██╗      █████╗ ██████╗ \n" << RESET;
    cout << CYAN << "\t" << "          ████╗ ████║██╔═══██╗██╔══██╗██║   ██║██║     ██╔══██╗██╔══██╗\n" << RESET;
    cout << CYAN << "\t" << "          ██╔████╔██║██║   ██║██║  ██║██║   ██║██║     ███████║██████╔╝\n" << RESET;
    cout << CYAN << "\t" << "          ██║╚██╔╝██║██║   ██║██║  ██║██║   ██║██║     ██╔══██║██╔══██╗\n" << RESET;
    cout << CYAN << "\t" << "          ██║ ╚═╝ ██║╚██████╔╝██████╔╝╚██████╔╝███████╗██║  ██║██║  ██║\n" << RESET;
    cout << CYAN << "\t" << "          ╚═╝     ╚═╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝\n\n" << RESET;

    cout << CYAN << "\t" << " ██████╗ █████╗ ██╗      ██████╗██╗   ██╗██╗      █████╗ ████████╗ ██████╗ ██████╗ \n" << RESET;
    cout << CYAN << "\t" << "██╔════╝██╔══██╗██║     ██╔════╝██║   ██║██║     ██╔══██╗╚══██╔══╝██╔═══██╗██╔══██╗\n" << RESET;
    cout << CYAN << "\t" << "██║     ███████║██║     ██║     ██║   ██║██║     ███████║   ██║   ██║   ██║██████╔╝\n" << RESET;
    cout << CYAN << "\t" << "██║     ██╔══██║██║     ██║     ██║   ██║██║     ██╔══██║   ██║   ██║   ██║██╔══██╗\n" << RESET;
    cout << CYAN << "\t" << "╚██████╗██║  ██║███████╗╚██████╗╚██████╔╝███████╗██║  ██║   ██║   ╚██████╔╝██║  ██║\n" << RESET;
    cout << CYAN << "\t" << " ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝\n\n" << RESET;
}

// This function shows the header.
void showHeader(string text)
{
    drawLine();
    printCentered(text, BOLD + YELLOW);
    drawLine();
}

// This function shows the description for each screen with a typing effect animations.
void showScreenIntro(string title, string desc)
{
    clearScreen();
    showLogo();
    showHeader(title);
    cout << "\n";
    if (!desc.empty())
        typeWrite(desc, 10);

    cout << "\n";
    printCentered("-----------------------------", CYAN);
}

void showMainMenu()
{
    int choice;
    string options[] = {"Modular exponentiation (b^n mod m)",
                        "Modular inverse (a^-1 mod m)",
                        "Solve linear congruence ax ≡ b (mod m)",
                        "Chinese Remainder Theorem",
                        "Applications of congruences",
                        "Run sample tests",
                        "Exit program"};
    do
    {
        choice = menuBuilder(options, 7, "Main Menu");
        switch (choice)
        {
        case 1:
            showModularExpScreen();
            break;
        case 2:
            showModInverseScreen();
            break;
        case 3:
            showLinearCongruenceScreen();
            break;
        case 4:
            showCRTScreen();
            break;
        case 5:
            showApplicationsMenu();
            break;
        case 6:
            runSampleTests();
            break;
        case 7:
            clearScreen();
            printCentered("Exiting... Goodbye!", GREEN);
            sleep(1000);
            exit(0);
            break;

        default:
            break;
        }
    } while (choice != 7);
}

void showApplicationsMenu()
{

    int choice;
    string options[] = {"Parity Bit",
                        "Universal Product Code (UPC)",
                        "International Standard Book Number (ISBN)",
                        "Back to Main Menu"};
    do
    {

        choice = menuBuilder(options, 4, "Applications Menu");
        switch (choice)
        {
        case 1:
            showParityBitScreen();
            break;
        case 2:
            showUPCScreen();
            break;
        case 3:
            showISBNScreen();
            break;
        case 4:
            return;
            break;
        default:
            break;
        }
    } while (choice != 4);
}

void showModularExpScreen()
{
    string choice;
    string desc =
        R"(Modular exponentiation refers to computing
expressions of the form b^n mod m without
first computing b^n directly. Direct computation
is inefficient and often impossible in practice
because b^n grows exponentially large. Modular
exponentiation insteadd performs the computation
while continually reducing mod m.)";
    showScreenIntro("Modular Exponentiation", desc);
    do
    {
        printCentered("Do you want to calculate any modular exponentiation equation? [y/n]: ", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "n" || choice == "N")
    {
        return;
    }

    bigInt b, n, m;
    bigInt res;

    b = getNumberInput("Enter the value of b (the base)");
    n = getNumberInput("Enter the value of n (the exponent)");
    m = getNumberInput("Enter the value of m (mod)", true);

    res = modularExponentiation(b, n, m);

    cout << "\n";
    printCentered("Calculating...", YELLOW);
    sleep(300);

    cout << "\n";
    printCentered("RESULT", GREEN);
    string resultStr = to_string(b) + "^" + to_string(n) + " mod " + to_string(m) + " = " + to_string(res);
    printCentered(resultStr, BOLD + CYAN);
    pressEnterToContinue();
}

void showModInverseScreen()
{

    string choice;
    string desc =
        R"(A modular inverse of an integer
'a' modulo 'm' is a number such
that ax ≡ 1 (mod m). Modular inverses are
central in solving linear congruences,
applying the Chinese Remainder Theorem.

The standard form of this equation is 'a^-1 mod m')";

    showScreenIntro("Modular Inverse", desc);

    do
    {
        printCentered("Do you want to calculate modular inverse of 'a' mod 'm'? [y/n]:", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "n" || choice == "N")
    {
        return;
    }

    bigInt a, m;
    bigInt res;

    cout << "\n";

    a = getNumberInput("Enter the value of a");
    m = getNumberInput("Enter the value of m (mod)", true);

    do
    {
        drawLine();
        printCentered("Do you want to see the detailed steps of Extended Euclidean Algorithm? [y/n]:", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "y" || choice == "Y")
        showSteps = true;
    else
        showSteps = false;

    res = modInverse(a, m);

    cout << "\n";
    if (res == LLONG_MIN)
    {
        printCentered("Inverse does not exist!", RED);
        printCentered("(a and m are not coprime)", RED);
    }
    else
    {
        string str = "Answer: " + to_string(a) + "^-1 mod " + to_string(m) + " = " + to_string(res);
        printCentered(str, GREEN);
    }
    pressEnterToContinue();
    showSteps = false;
}

void showLinearCongruenceScreen()
{
    string choice;
    string desc =
        R"(A linear congruence is an equation of the form
ax ≡ b (mod m), where 'a', 'b', and 'm'
are known integers, and 'x' is the unknown
integer to be solved for.

The standard form of this equation is 'ax ≡ b (mod m)')";
    showScreenIntro("Linear Congruence", desc);
    do
    {
        printCentered("Do you want to solve a linear congruence equation? [y/n]: ", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "n" || choice == "N")
    {
        return;
    }

    bigInt a, b, m;
    bigInt res;

    cout << "\n";

    a = getNumberInput("Enter the value of a");
    b = getNumberInput("Enter the value of b");
    m = getNumberInput("Enter the value of m (mod)", true);

    do
    {
        drawLine();
        printCentered("Do you want to see the detailed steps of Extended Euclidean Algorithm? [y/n]:", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "y" || choice == "Y")
        showSteps = true;
    else
        showSteps = false;

    res = linearCongruence(a, b, m);

    cout << "\n";
    if (res == LLONG_MIN)
    {
        printCentered("No solution exists for the given linear congruence.", RED);
    }
    else
    {
        string str = "Answer: The solution x ≡ " + to_string(res) + " (mod " + to_string(m) + ")";
        printCentered(str, GREEN);
    }
    pressEnterToContinue();
    showSteps = false;
}

void showCRTScreen()
{
    string choice;
    string desc =
        R"(The Chinese Remainder Theorem (CRT) is a
powerful tool in number theory that
provides a way to solve systems of
simultaneous linear congruences with
pairwise coprime moduli.

The standard form of this equation is a system of congruences like:
x ≡ a1 (mod m1)
x ≡ a2 (mod m2)
...
x ≡ ak (mod mk))";
    showScreenIntro("Chinese Remainder Theorem", desc);
    do
    {
        printCentered("Do you want to solve a system of linear congruences? [y/n]:", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "n" || choice == "N")
    {
        return;
    }

    bigInt res, M;
    int numEquations;

    numEquations = getNumberInput("Enter number of equations", true);

    if (numEquations < 2)
    {
        printCentered("At least 2 equations are required to apply the Chinese Remainder Theorem.", RED);
        pressEnterToContinue();
        return;
    }

    bigInt **equations = new bigInt *[numEquations];

    for (int i = 0; i < numEquations; i++)
        equations[i] = new bigInt[2];

    for (int i = 0; i < numEquations; i++)
    {
        cout << "\n";
        printCentered("--- Equation " + to_string(i + 1) + " ---", YELLOW);
        equations[i][0] = getNumberInput("Enter the value of a");
        equations[i][1] = getNumberInput("Enter the value of m (mod)", true);
    }

    do
    {
        drawLine();
        printCentered("Do you want to see the detailed steps of Extended Euclidean Algorithm? [y/n]:", RESET, false);
        getline(cin, choice);
    } while (!(choice == "y" || choice == "n" || choice == "Y" || choice == "N"));

    if (choice == "y" || choice == "Y")
        showSteps = true;
    else
        showSteps = false;

    res = chineseRemainderTheorem(equations, M, numEquations);

    for (int i = 0; i < numEquations; i++)
        delete[] equations[i];
    delete[] equations;

    cout << "\n";
    if (res == LLONG_MIN)
    {
        printCentered("No solution exists for the given system of linear", RED);
        printCentered("congruences as mods are not relatively prime", RED);
    }
    else
    {
        string str = "Answer: The solution x ≡ " + to_string(res) + " (mod " + to_string(M) + ")";
        printCentered(str, GREEN);
    }
    pressEnterToContinue();
    showSteps = false;
}

void showParityBitScreen()
{
    string options[] = {
        "Calculate Parity Bit",
        "Verify Data with Parity Bit",
        "Back to Applications Menu"};
    string desc = R"(Digital information is represented by bit string, split
into blocks of a specified size. Before each block is stored
or transmitted, an extra bit, called a parity check bit, can be
appended to each block.)";
    int choice;
    do
    {
        showScreenIntro("Parity Bit Check", desc);
        choice = menuBuilder(options, 3, "Parity Bit");
        switch (choice)
        {
        case 1:
        {
            string data;
            int digitSum = 0;
            do
            {
                drawLine();
                printCentered("Enter the data bits (sequence of 0s and 1s) to generate parity bit: ");
                getline(cin, data);
            } while (!isValidBinaryString(data));

            for (int i = 0; i < data.length(); i++)
            {
                digitSum += (data[i] - '0');
            }
            printCentered("Parity bit: " + to_string(digitSum % 2), GREEN);
            pressEnterToContinue();
        }
        break;
        case 2:
        {
            string data;
            int digitSum = 0;
            do
            {
                drawLine();
                printCentered("Enter the data bits (sequence of 0s and 1s) to check validity: ");
                getline(cin, data);
            } while (!isValidBinaryString(data));
            for (int i = 0; i < data.length(); i++)
            {
                digitSum += (data[i] - '0');
            }
            if (digitSum % 2 == 0)
            {
                printCentered("Data is valid", GREEN);
            }
            else
            {
                printCentered("Data is not valid", RED);
            }
            pressEnterToContinue();
        }

        break;
        case 3:
            return;
            break;
        default:
            break;
        }

    } while (choice != 3);
}

void showUPCScreen()
{
    string options[] = {
        "Calculate UPC Check Digit",
        "Verify UPC Code",
        "Back to Applications Menu"};
    string desc = R"(The Universal Product Code (UPC) is a widely used
barcode symbology that encodes a 12-digit numeric
code. The last digit of the UPC is a check digit, which
is used to verify the accuracy of the code during scanning
and data entry.)";
    int choice;
    do
    {
        showScreenIntro("Universal Product Code (UPC)", desc);

        choice = menuBuilder(options, 3, "UPC Tools");
        switch (choice)
        {
        case 1:
        {
            string upc;
            int sum = 0, checkDigit;
            do
            {
                drawLine();
                printCentered("Enter the first 11 digits of the UPC code: ");
                getline(cin, upc);
            } while (!isValidNumber(upc, true) || upc.length() != 11);

            for (int i = 0; i < 11; i++)
            {
                if (i % 2 == 0)
                    sum += (upc[i] - '0');
                else
                    sum += 3 * (upc[i] - '0');
            }

            checkDigit = (10 - (sum % 10)) % 10;

            printCentered("Check digit: " + to_string(checkDigit), GREEN);
            pressEnterToContinue();
        }
        break;
        case 2:
        {
            string upc;
            int sum = 0, checkDigit;
            do
            {
                drawLine();
                printCentered("Enter the UPC code: ");
                getline(cin, upc);
            } while (!isValidNumber(upc, true) || upc.length() != 12);

            for (int i = 0; i < 12; i++)
            {
                if (i % 2 == 0)
                    sum += (upc[i] - '0');
                else
                    sum += 3 * (upc[i] - '0');
            }

            cout << "\n";
            if (verifyCongruence(sum, 0, 10))
            {
                printCentered("This code is correct", GREEN);
            }
            else
            {
                printCentered("This code is not correct", RED);
            }
            pressEnterToContinue();
        }
        break;
        case 3:
            return;
            break;
        default:
            break;
        }

    } while (choice != 3);
}

void showISBNScreen()
{
    string options[] = {
        "Calculate ISBN-10 Check Digit",
        "Verify ISBN-10 Code",
        "Back to Applications Menu"};
    string desc =
        R"(The International Standard Book Number (ISBN) is a unique numeric
identifier assigned to each edition and variation of a book.
The last digit of the ISBN is a check digit, which is used to verify
the integrity of book.)";
    int choice;
    do
    {
        showScreenIntro("International Standard Book Number (ISBN)", desc);

        choice = menuBuilder(options, 3);
        switch (choice)
        {
        case 1:
        {
            string isbn;
            int sum = 0, checkDigit;
            do
            {
                drawLine();
                printCentered("Enter the first 9 digits of the ISBN-10 code: ");
                getline(cin, isbn);
            } while (!isValidISBN(isbn) || isbn.length() != 9);

            for (int i = 0; i < 9; i++)
            {
                sum += (i + 1) * (isbn[i] - '0');
            }

            checkDigit = sum % 11;

            cout << "\n";
            if (checkDigit == 10)
            {
                printCentered("Check Digit: X", GREEN);
            }
            else
            {
                printCentered("Check Digit: " + to_string(checkDigit), GREEN);
            }
            pressEnterToContinue();
        }
        break;
        case 2:
        {
            string isbn;
            int res = 0, checkDigit;
            do
            {
                drawLine();
                printCentered("Enter the ISBN code: ");
                getline(cin, isbn);
            } while (!isValidISBN(isbn) || isbn.length() != 10);

            for (int i = 0; i < 10; i++)
            {
                if (isbn[i] == 'X')
                {
                    res += (i + 1) * 10;
                }
                else
                {
                    res += (i + 1) * (isbn[i] - '0');
                }
            }

            cout << "\n";
            if (verifyCongruence(res, 0, 11))
            {
                printCentered("This code is correct", GREEN);
            }
            else
            {
                printCentered("This code is not correct", RED);
            }
            pressEnterToContinue();
        }
        break;
        case 3:
            return;
            break;
        default:
            break;
        }

    } while (choice != 3);
}

// ************ Main Logic Functions ******************
bigInt gcd(bigInt a, bigInt b)
{
    if (a == 0)
        return b;
    return gcd(b % a, a);
}

/*
This function takes congruence (of form ax≡b(mod m)) and returns whether it is solvable or not
The logic of this function is described in the 'book' in proof of Theorem 4 from chapter 4(Page 255 of 8th edition).
*/
bool verifyCongruence(bigInt a, bigInt b, bigInt m)
{
    return (a - b) % m == 0;
}

/*
Computes (b^n) mod m using fast modular exponentiation
The algorith from this function was given in the book's chapter number 4, section 4.2.4 (Page 268 of 8th edition).
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
bigInt extendedGcd(bigInt a, bigInt b, bigInt &s, bigInt &t, bool showSteps)
{
    if (b == 0)
    {
        s = 1;
        t = 0;
        return a;
    }
    if (a >= b && showSteps)
    {
        string step = to_string(a) + " = " + to_string(b) + "*" + to_string(a / b) + " + " + to_string(a % b);
        printCentered(step, YELLOW);
        sleep(100);
    }
    bigInt s1, t1;
    bigInt g = extendedGcd(b, a % b, s1, t1, showSteps);
    s = t1;
    t = s1 - (a / b) * t1;
    return g;
}

/*
modular inverse of a modulo m using Extended Euclidean Algorithm
returns inverse in range [0, m-1] if it exists, otherwise returns LLONG_MIN
Concept of this function is taken from 'book', chapter 4, section 4.4.2 Theorem 1 (Page 291 of 8th edition).
*/
bigInt modInverse(bigInt a, bigInt m)
{
    bigInt s, t;
    if (showSteps)
    {
        string str;
        printCentered("Calculating GCD", YELLOW);

        cout << "\n";
        if (a > m)
        {
            str = to_string(a) + " = " + to_string(m) + "*" + to_string(a / m) + " + " + to_string(a % m);
            printCentered(str, YELLOW);
        }
        else
        {
            str = to_string(m) + " = " + to_string(a) + "*" + to_string(m / a) + " + " + to_string(m % a);
            printCentered(str, YELLOW);
        }
    }

    a = (a % m + m) % m; // make sure a >= 0 and a < m
    bigInt g = extendedGcd(a, m, s, t, showSteps);

    if (g != 1)
        return LLONG_MIN;   // inverse doesn't exist when gcd != 1
    return (s % m + m) % m; // returning +ve result
}

/*
This function solves linearCongruence of Form ax≡b (mod m). Just give the value of a, b and m and this function returns (if possible) the value of x, otherwise LLONG_MIN
Concept of this function is taken from 'book', chapter 4, section 4.4.2 Theorem 1 (Page 291 of 8th edition).
*/
bigInt linearCongruence(bigInt a, bigInt b, bigInt m)
{
    bigInt g = gcd(a, m);
    bigInt x;

    if (b % g)
    {
        return LLONG_MIN; // => solution does not exist
    }
    if (g != 1)
    {
        a = a / g;
        b = b / g;
        m = m / g;
    }

    bigInt inv = modInverse(a, m);

    if (inv == LLONG_MIN)
    {
        return LLONG_MIN;
    }
    x = (b * inv) % m;
    return x;
}

/*
This function solves system of linear congruences using Chinese Remainder Theorem.
Steps to implement this function is taken from 'https://brilliant.org/wiki/chinese-remainder-theorem/'
*/
bigInt chineseRemainderTheorem(bigInt **equations, bigInt &M, int k)
{
    bigInt x = 0;
    M = 1;
    bigInt y, z;
    for (int i = 0; i < k-1; i++)
    {
        for (int j = i + 1; j < k; j++)
        {
            if(gcd(equations[i][1], equations[j][1]) != 1){
                // => CRT could not apply as mods are not relatively co-prime.
                M = LLONG_MIN;
                return LLONG_MIN;
            }
        }
        
    }
    
    for (int i = 0; i < k; i++)
    {
        M *= equations[i][1];
    }
    for (int i = 0; i < k; i++)
    {
        y = M / equations[i][1];
        z = modInverse(y, equations[i][1]);
        x += (equations[i][0] * y * z);
    }
    x %= M;
    if (x < 0)
        x += M;
    return x;
}

void runSampleTests()
{
    string desc =
 R"(Running sample tests for modular exponentiation,
    modular inverse, linear congruence solver, and Chinese
    Remainder Theorem.)";
    clearScreen();
    showLogo();
    showHeader("Loading Sample Tests");
    showLoadingScreen();

    clearScreen();
    showLogo();
    showScreenIntro("Running Sample Tests", desc);

    // Sample Test 1: Modular Exponentiation
    TestCase t1;
    t1.b = 5;
    t1.n = 3;
    t1.m = 13;
    t1.expected = 8;
    t1.result = modularExponentiation(t1.b, t1.n, t1.m);

    printCentered("Modular Exponenctitaion Test: ", YELLOW);
    printCentered("Input: b = " + to_string(t1.b) + ", n = " + to_string(t1.n) + ", m = " + to_string(t1.m), YELLOW);
    printCentered("Expected Output: " + to_string(t1.expected), YELLOW);
    printCentered("Actual Output: " + to_string(t1.result), YELLOW);
    cout << endl;

    // Sample Test 2: Modular Inverse
    TestCase t2;
    t2.b = 3;
    t2.m = 11;
    t2.expected = 4;
    t2.result = modInverse(t2.b, t2.m);

    printCentered("Modular Inverse Test: ", YELLOW);
    printCentered("Input: a = " + to_string(t2.b) + ", m = " + to_string(t2.m), YELLOW);
    printCentered("Expected Output: " + to_string(t2.expected), YELLOW);
    printCentered("Actual Output: " + to_string(t2.result), YELLOW);
    cout << endl;

    // Sample Test 3: Linear Congruence Solver
    TestCase t3;
    t3.b = 4;
    t3.n = 8;
    t3.m = 12;
    t3.expected = 2;
    t3.result = linearCongruence(t3.b, t3.n, t3.m);

    printCentered("Linear Congruence Solver Test: ", YELLOW);
    printCentered("Input: a = " + to_string(t3.b) + ", b = " + to_string(t3.n) + ", m = " + to_string(t3.m), YELLOW);
    printCentered("Expected Output: " + to_string(t3.expected), YELLOW);
    printCentered("Actual Output: " + to_string(t3.result), YELLOW);
    cout << endl;

    // Sample Test 4: Chinese Remainder Theorem
    bigInt *equations[2];
    equations[0] = new bigInt[2]{2, 3};
    equations[1] = new bigInt[2]{3, 5};
    bigInt M4;
    bigInt result4 = chineseRemainderTheorem(equations, M4, 2);

    printCentered("Chinese Remainder Theorem Test: ", YELLOW);
    printCentered("Input: x ≡ 2 (mod 3), x ≡ 3 (mod 5)", YELLOW);
    printCentered("Expected Output: 8", YELLOW);
    printCentered("Actual Output: " + to_string(result4), YELLOW);
    cout << endl;

    pressEnterToContinue("Press enter to continue.");
}