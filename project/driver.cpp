#include <iostream>
#include "matchers.h"

using namespace std;

int main(int argc, char *argv[])
{

    string kafka = "you do not need to leave your room. aaacaaac remain sitting at your table and listen. do not even listen, simply wait, be quiet, still and solitary. the world will freely offer itself to you to be unmasked, it has no choice, it will roll in ecstasy at your feet.";

    string_matcher *m = new kmp(kafka);
    vector<int> matches;
    m->match("you", matches);
    cout << "KMP result" << endl;
    for (auto n : matches)
    {
        cout << n << " ";
    }
    cout << endl;

    string_matcher *ra = new rabin_karp(kafka);
    matches = vector<int>();
    ra->match("you", matches);
    cout << "rabin_karp result" << endl;
    for (auto n : matches)
    {
        cout << n << " ";
    }
    cout << endl;

    return 0;
}