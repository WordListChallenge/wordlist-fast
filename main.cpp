#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <codecvt>
#include <locale>

using namespace std;

const int LEN = 6;

vector<set<u16string>> buildVector(
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> *utf16conv,
        char *fileName) {
    vector<set<u16string>> vec(LEN);
    ifstream myfile(fileName);
    string utf8;
    if (myfile.is_open()) {
        while (getline(myfile, utf8)) {
            u16string line = utf16conv->from_bytes(utf8);
            unsigned long len = line.size();
            if (len == LEN) {
                // the bucket at zero contains all words of length LEN
                set<u16string> &bucket = vec.at(0);
                bucket.insert(line);
            } else if (len > 0 && len < LEN) {
                set<u16string> &bucket = vec.at(len);
                bucket.insert(line);
            }
        }
        myfile.close();
    } else cout << "Unable to open file";
    return vec;
}

void printPairs(
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> *utf16conv,
        ofstream *out,
        vector<set<u16string>> *vec,
        u16string s) {
    for (int i = 1; i < LEN; ++i) {
        set<u16string> &heads = vec->at(i);
        set<u16string> &tails = vec->at(LEN - i);
        u16string head = s.substr(0, i);
        u16string tail = s.substr(i);
        bool headFound = heads.find(head) != heads.end();
        bool tailFound = tails.find(tail) != tails.end();
        if (headFound && tailFound) {
            *out << utf16conv->to_bytes(head) << "_" << utf16conv->to_bytes(tail) << "\n";
        }
    }
}

void run(char *infile, char *outfile) {
    // the UTF-8 / UTF-16 standard conversion facet
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
    vector<set<u16string>> vec = buildVector(&utf16conv, infile);
    set<u16string> &words = vec.at(0);
    ofstream out;
    out.open(outfile);
    for (auto it = words.rbegin(); it != words.rend(); ++it) {
        printPairs(&utf16conv, &out, &vec, *it);
    }
    out.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Expecting two arguments: input_file output_file";
        return 1;
    }
    char *infile = argv[1];
    char *outfile = argv[2];
    run(infile, outfile);
    return 0;
}