#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <codecvt>
#include <locale>
#include <unordered_set>
#include <forward_list>

using namespace std;

const unsigned int LEN = 6;

struct parsedList {

    forward_list<u16string> wordsToCheck;
    vector<unordered_set<u16string>> candidates;

    parsedList(
            forward_list<u16string> *wordToCheck,
            vector<unordered_set<u16string>> *candidates) :
            wordsToCheck(*wordToCheck),
            candidates(*candidates) {};
};

parsedList *parseWordList(
        forward_list<u16string> *wordsToCheck,
        vector<unordered_set<u16string>> *candidates,
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> *utf16conv,
        char *fileName) {
    ifstream myfile(fileName);
    string utf8;
    if (myfile.is_open()) {
        while (getline(myfile, utf8)) {
            u16string line = utf16conv->from_bytes(utf8);
            unsigned long len = line.size();
            if (len == LEN) {
                wordsToCheck->push_front(line);
            } else if (len > 0 && len < LEN) {
                unordered_set<u16string> &bucket = candidates->at(len - 1);
                bucket.insert(line);
            }
        }
        myfile.close();
    } else cout << "Unable to open file";
    return new parsedList(wordsToCheck, candidates);
}

/**
 * @param utf16conv String converter for printing.
 * @param out The output stream that we're writing to.
 * @param candidates A vector of length LEN - 1.
 *   Words of length 1 are at zero, words of length 2 are at one, and so on.
 * @param s The string that we're attempting to write as a concatenation of two candidates.
 */
void printPairs(
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> *utf16conv,
        ofstream *out,
        vector<unordered_set<u16string>> *candidates,
        u16string *s) {
    for (unsigned int len = LEN - 1; len != 0; --len) {
        u16string head = s->substr(0, len);
        u16string tail = s->substr(len);
        unordered_set<u16string> &heads = candidates->at(len - 1);
        unordered_set<u16string> &tails = candidates->at(LEN - 1 - len);
        bool headFound = heads.find(head) != heads.end();
        bool tailFound = tails.find(tail) != tails.end();
        if (headFound && tailFound) {
            *out << utf16conv->to_bytes(head) << "_" << utf16conv->to_bytes(tail) << "\n";
        }
    }
}

void run(char *infile, char *outfile) {
    forward_list<u16string> wordsToCheck;
    vector<unordered_set<u16string>> vec(LEN - 1);
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
    parsedList *parsedList = parseWordList(&wordsToCheck, &vec, &utf16conv, infile);
    forward_list<u16string> &words = parsedList->wordsToCheck;
    ofstream out;
    out.open(outfile);
    for (auto &word : words) {
        printPairs(&utf16conv, &out, &parsedList->candidates, &word);
    }
    out.close();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Expecting two arguments: input_file output_file\n";
        return 1;
    }
    char *infile = argv[1];
    char *outfile = argv[2];
    run(infile, outfile);
    return 0;
}
