//
//  main.cpp
//  Project 4
//
//  Created by Nathan Zhang on 10/30/24.
//

#include <iostream>
#include <cassert>
#include <string>
using namespace std;

int appendToAll(string a[], int n, string value) {
    // If n is negative, return -1
    if (n < 0) {
        return -1;
    }
    
    // Iterate through the array and add "value" to every element
    for (int i=0; i<n; i++) {
        a[i] += value;
    }
    return n;
}

int lookup(const string a[], int n, string target) {
    // Iterate through the array, and if "target" is found, return the index
    // If n is negative, this code still works as the loop will simply not run
    for (int i=0; i<n; i++) {
        if (a[i] == target) {
            return i;
        }
    }
    
    // If "target" is not found, return -1
    return -1;
}

int positionOfMax(const string a[], int n) {
    // Set "max_pos" to -1 for the case that "n" is negative and the loop does not run
    int max_pos = -1;
    
    // Set "max_string" to the empty string, which is the lexicographically smallest string
    string max_string = "";
    for (int i=0; i<n; i++) {
        // Use > instead of >= to capture the earliest position of the array's max string
        if (a[i] > max_string) {
            max_string = a[i];
            max_pos = i;
        }
    }
    return max_pos;
}

int rotateLeft(string a[], int n, int pos) {
    // If n is negative or "pos" is not one of the elements of interest, return -1
    if (n < 0 || pos < 0 || pos >= n) {
        return -1;
    }
    
    // Starting from "pos", swap each element with the next element, moving the element at "pos" to the back
    // Stop before n - 1 to avoid moving the element at "pos" past the elements of interest
    for (int i=pos; i<n-1; i++) {
        string temp = a[i];
        a[i] = a[i+1];
        a[i+1] = temp;
    }
    return pos;
}

int countRuns(const string a[], int n) {
    // If n is negative, return -1
    if (n < 0) {
        return -1;
    }
    
    int count = 0;
    for (int i=0; i<n; i++) {
        // New run, so increment count
        count++;
        
        // Increment i until we reach the next run or until i = n
        string curr = a[i];
        while (i < n && a[i] == curr) {
            i++;
        }
        
        // The while loop puts i at the position of the next run's first element
        // Decrement i to cancel out the for loop's increment
        i--;
    }
    return count;
}

int flip(string a[], int n) {
    // If n is negative, return -1
    if (n < 0) {
        return -1;
    }
    
    // Swap each element with the element at the mirrored position from the end
    // The for loop only needs to go up to (n / 2) - 1 because of the symmetric nature of the swaps
    for (int i=0; i<n/2; i++) {
        string temp = a[i];
        a[i] = a[n-i-1];
        a[n-i-1] = temp;
    }
    return n;
}

int differ(const string a1[], int n1, const string a2[], int n2) {
    // If n1 or n2 is negative, return -1
    if (n1 < 0 || n2 < 0) {
        return -1;
    }
    
    // Stop when i is either equal to n1 or equal to n2 because this would mean at least one of the elements is not of interest
    for (int i=0; i<n1 && i<n2; i++) {
        // When inequal elements are found, immediately return i to ensure that the smsallest position of such a case is returned
        if (a1[i] != a2[i]) {
            return i;
        }
    }
    
    // If this code is reached, it means that no inequal elements were found, and thus the smaller of n1 or n2 is returned, or n1 if they are equal
    if (n1 <= n2) {
        return n1;
    }
    return n2;
}

int subsequence(const string a1[], int n1, const string a2[], int n2) {
    // If n1 or n2 is negative, return -1
    if (n1 < 0 || n2 < 0) {
        return -1;
    }
    
    // If the size of the subsequence is zero, then the first position that works is zero
    // This is hardcoded to avoid accessing an element that is not within n1
    if (n2 == 0) {
        return 0;
    }
    
    // i goes up to n1 - n2, because that is the last possible starting position for a subsequence of size n2
    for (int i=0; i<=n1-n2; i++) {
        // Assume the subsequence works until an error is found
        bool works = true;
        int pos = 0;
        
        for (int j=i; j<i+n2; j++) {
            // If the subsequence doesn't fit, set "works" to false
            if (a1[j] != a2[pos]) {
                works = false;
            }
            pos++;
        }
        
        // Immediately return i if "works" is true to ensure that the smallest position of a valid answer is returned
        if (works) {
            return i;
        }
    }
    return -1;
}

int lookupAny(const string a1[], int n1, const string a2[], int n2) {
    // If n1 or n2 is negative, return -1
    if (n1 < 0 || n2 < 0) {
        return -1;
    }
    
    for (int i=0; i<n1; i++) {
        for (int j=0; j<n2; j++) {
            // If a position i is found where a1[i] is equal to any of a2's elements of interest, immediately return i
            // With this, the smallest valid position is returned
            if (a1[i] == a2[j]) {
                return i;
            }
        }
    }
    
    // If no position is found, return -1
    return -1;
}

int separate(string a[], int n, string separator) {
    // If n is negative, return -1
    if (n < 0) {
        return -1;
    }
    
    // This is the same thing as simply sorting "a"
    // Sort "a" using bubble sort
    for (int i=0; i<n-1; i++) {
        // Find the position of the "curr_min", the smallest string with an index of i or greater
        // By default, assume "curr_min" is at i
        string curr_min = a[i];
        int pos = i;
        for (int j=i+1; j<n; j++) {
            if (a[j] < curr_min) {
                curr_min = a[j];
                pos = j;
            }
        }
        
        // Swap the current string with the "curr_min"
        string temp = a[i];
        a[i] = a[pos];
        a[pos] = temp;
    }

    // Find the first element of interest that is not less than "separator"
    for (int i=0; i<n; i++) {
        if (a[i] >= separator) {
            return i;
        }
    }
    
    // If all elements of interest are less than "separator", return n
    return n;
}


int main(int argc, const char * argv[]) {
    string h[7] = { "melania", "kamala", "donald", "tim", "", "doug", "jd" };
    assert(lookup(h, 7, "doug") == 5);
    assert(lookup(h, 7, "donald") == 2);
    assert(lookup(h, 2, "donald") == -1);
    assert(positionOfMax(h, 7) == 3);

    string g[4] = { "melania", "kamala", "jd", "usha" };
    assert(differ(h, 4, g, 4) == 2);
    assert(appendToAll(g, 4, "?") == 4 && g[0] == "melania?" && g[3] == "usha?");
    assert(rotateLeft(g, 4, 1) == 1 && g[1] == "jd?" && g[3] == "kamala?");

    string e[4] = { "donald", "tim", "", "doug" };
    assert(subsequence(h, 7, e, 4) == 2);

    string d[5] = { "kamala", "kamala", "kamala", "tim", "tim" };
    assert(countRuns(d, 5) == 2);

    string f[3] = { "jd", "donald", "gwen" };
    assert(lookupAny(h, 7, f, 3) == 2);
    assert(flip(f, 3) == 3 && f[0] == "gwen" && f[2] == "jd");

    assert(separate(h, 7, "jd") == 3);

    cout << "All tests succeeded on final version of code!!!!" << endl;
    
    /*
    string a[10] = {"a", "b", "c"};
    string b[10] = {"a", "b", "c", "c"};
    string c[10] = {};
    string d[10] = {"d", "d", "c", "b", "a"};
    string e[10] = {"a", "b"};
    string f[10] = {"c"};
    
    appendToAll(a, -100, "!"); // n is negative, so it should return -1
    appendToAll(a, 2, "!!"); // should only add "!!" to the first 2 elements, not to "c"
    appendToAll(a, 0, "!!"); // n is zero, so it should not apply "!!" to any elements and return n
    
    lookup(a, -2, "b"); // n is negative, so it should return -1
    lookup(a, 1, "b"); // although "b" is in the array, it is not in the first n elements, so it should return -1
    lookup(a, 3, "c"); // check if the function works and returns 2
    lookup(a, 3, "d"); // check if the function works and returns -1
    lookup(b, 4, "c"); // there is two "c" strings, but it should return the position of the first one (2)
    lookup(c, 0, "c"); // empty array, so return -1
    
    positionOfMax(a, -5); // n is negative, so it should return -1
    positionOfMax(a, 2); // check if it works and returns 1
    positionOfMax(b, 4); // there is two "c" strings (both max values of the array), but it should return the position of the first one (2)
    positionOfMax(b, 0); // n is zero, so it should return -1
    positionOfMax(c, 0); // empty array, so return -1
     
    rotateLeft(a, -5, 1); // n is negative, so it should reutrn -1
    rotateLeft(a, 3, 4); // pos is not within the first n elements, so it should return -1
    rotateLeft(a, 3, -1); // pos is negative, so it should return -1
    rotateLeft(b, 4, 1); // check if it works by making the array {"a", "c", "c", "b"} and returning 1
    rotateLeft(b, 3, 1); // check if it works by making the array ("a", "c", "b", "c") and returning 1
    rotateLeft(b, 0, 0); // n and pos are both zero, so it should return -1
    rotateLeft(c, 0, 0); // empty array, return -1

    countRuns(a, -1); // n is negative, so it should return -1
    countRuns(a, 3); // check if it works with single-element runs and returns 3
    countRuns(b, 4); // check if it works with multiple-element runs and returns 3
    countRuns(b, 0); // n is zero, so it should return 0
    countRuns(c, 0); // empty array, return 0

    flip(a, -7); // n is negative, so it should return -1
    flip(a, 0); // n is zero, so it should leave a unchanged and return 0
    flip(a, 2); // should only flip the first 2 elements and return 2, a should be {"b", "a", "c"}
    flip(b, 4); // should flip all elements and return 4, b should be {"c", "c", "b", "a"}
    flip(c, 0); // empty array, return 0

    differ(a, 0, b, 3); // return 0
    differ(a, -1, b, 0); // n1 is negative, return -1
    differ(a, 0, b, -1); // n2 is negative, return -1
    differ(a, 3, b, 4); // all elements are the same, return 3
    differ(a, 3, d, 5); // first element is different, return 0
    
    subsequence(a, 3, c, 0); // n2 is zero, return 0
    subsequence(a, -1, c, 0); // n1 is negative, return -1
    subsequence(a, 0, c, -1); // n2 is negative, return -1
    subsequence(a, 3, e, 2); // check if it works and returns 0
    
    lookupAny(a, -1, b, 2); // n1 is negative, return -1
    lookupAny(a, 0, b, -1); // n2 is negative, return -1
    lookupAny(a, 2, f, 1); // see if it only checks the first n1 elements in a, should return -1
    lookupAny(a, 3, f, 1); // make sure it works, should return 2
    lookupAny(b, 4, f, 1); // make sure it returns the first position, should return 2
    
    separate(a, -1, "d"); // n is negative, return -1
    separate(a, 3, "d"); // should keep a as it is as "d" is bigger than all elements, return 3
    separate(d, 5, "b"); // make sure function works, should return 1 and make d into {"a", "b", "c", "d", "d"}
    */

    return 0;
}
