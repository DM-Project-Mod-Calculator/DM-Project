/*
NOTES:
comments starting with '=>' are for explanation purposes of members. These comments must be removed before final submission of project.
This code is not structured yet, neither the UI is implemented.
For Now our main focus is just to implement the core logic.
Terms:
 'book': refers to 'Discrete Mathematics and it's Applications' by 'Kenneth H. Rosen'
*/

#include <iostream>

using namespace std;

/*
=> This function just returns the +ve equivalent of a mod m.
like if we take -3 mod 11, the result will be -3, but
by using this function, the result will be 8
*/
long long normalizeMod(long long a, long long m){
return (a % m + m) % m;
}

// Computes (b^n) mod m using fast modular exponentiation
/*
=> The algorith from this function was given in the book's chapter number 4 'Number theroy and cryptography', section 4.2.4 (Page 268 of 8th edition).
This algorithm uses the concept of binary expansion
*/
long long modularExponentiation(long long b, long long n, long long m)
{
    long long x = 1;
    long long power = b % m;

    while (n > 0)
    {
        if (n % 2 == 1) // if current bit a_i = 1
            x = (x * power) % m;

        power = (power * power) % m; // square for next bit
        n /= 2;                      // move to next bit
    }

    return x; // equals b^n mod m
}

long long gcd(long long a, long long b) {
    if (a == 0)
        return b;
    return gcd(b % a, a);
}


// extended gcd: returns gcd(a,b) and sets x,y such that a*x + b*y = gcd(a,b)
/*
=> The algorithm for this function is learned online (https://www.geeksforgeeks.org/dsa/euclidean-algorithms-basic-and-extended/).
*/
long long extendedGcd(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    if(a>=b){
        // cout<<a<<" = "<<b<<"*"<<a/b<<" + "<<a%b<<endl; // => Prints steps while calculating gcd
    }
    long long x1, y1;
    long long g = extendedGcd(b, a % b, x1, y1);
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
long long modInverse(long long a, long long m) {
    a = normalizeMod(a,m);               // normalize a
    long long x, y;
    long long g = extendedGcd(a, m, x, y);
    if (g != 1) return -1;             // inverse doesn't exist when gcd != 1
    return normalizeMod(x,m);            // ensure positive result
}

/*
=> This function solves linearCongruence of Form ax≡b (mod m). Just give the value of a, b and m and this function returns (if possible) the value of x, otherwise -1;
*/
long long linearCongruence(long long a, long long b, long long m){
    long long g = gcd(a,m);
   
        if(b%g){
            return -1;// => solution does not exist
        }
        if(g!=1){
            a = a / g;
            b = b / g;
            m = m / g;
        }
    

    long long inv = modInverse(a,m);
    return (b*inv)%m;
}



int main() {
    // testing will be done here
    return 0;
}