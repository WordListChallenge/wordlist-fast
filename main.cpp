#include <iostream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <unordered_set>
#include <forward_list>

using namespace std;

// we're trying to split all words of this length
const unsigned int LEN = 6;

void read_input_file(
        forward_list<u16string> &candidates,
        unordered_set<u16string> &fragments,
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> &conv,
        char *infile) {
    ifstream in(infile);
    string utf8;
    while (getline(in, utf8)) {
        u16string line = conv.from_bytes(utf8);
        unsigned long len = line.size();
        if (len == LEN) {
            candidates.push_front(line);
        } else if (len != 0 && len < LEN) {
            fragments.insert(line);
        }
    }
}

/**
 * @param conv String converter for printing.
 * @param out The output stream that we're writing to.
 * @param fragments An array of length LEN - 1.
 *   Words of length 1 are at zero, words of length 2 are at one, and so on.
 * @param candidate The string we're trying to write as a concatenation of two fragments.
 */
inline void print_pairs(
        wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> &conv,
        ofstream &out,
        unordered_set<u16string> &fragments,
        u16string &candidate) {
    for (unsigned int len = LEN - 1; len != 0; --len) {
        u16string head = candidate.substr(0, len);
        bool head_found = fragments.find(head) != fragments.end();
        if (head_found) {
            u16string tail = candidate.substr(len);
            if (fragments.find(tail) != fragments.end()) {
                out << conv.to_bytes(head) << " + "
                    << conv.to_bytes(tail) << " => "
                    << conv.to_bytes(candidate) << "\n";
            }
        }
    }
}

void run(char *infile, char *outfile) {
    forward_list<u16string> candidates;
    unordered_set<u16string> fragments(32768);
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> conv;
    read_input_file(candidates, fragments, conv, infile);
    ofstream out(outfile);
    for (u16string word : candidates) {
        print_pairs(conv, out, fragments, word);
    }
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
