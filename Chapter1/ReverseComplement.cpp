#include <iostream>
#include <string>

std::string ReverseComplement(std::string Pattern){
    std::string rev_comp;
    rev_comp.resize(Pattern.length());
    for (int i = Pattern.length() - 1; i >= 0; i--) {
        rev_comp[Pattern.length() - i - 1] = Pattern[i];
    }

    for (std::string::iterator it = rev_comp.begin(); it != rev_comp.end(); it++) {
        if (*it == 'A') {
            *it = 'T';
        } else if (*it == 'T') {
            *it = 'A';
        } else if (*it == 'C') {
            *it = 'G';
        } else if (*it == 'G') {
            *it = 'C';
        }
    }
    return rev_comp;
}

int main() {
    std::string Pattern = "AAAACCCGGT";
    std::string rev_comp = ReverseComplement("AAAACCCGGT");
    std::cout << Pattern << "\n";
    std::cout << rev_comp;
    return 0;

}