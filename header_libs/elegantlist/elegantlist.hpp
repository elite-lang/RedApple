#ifndef ELEGANT_LIST_H
#define ELEGANT_LIST_H


#include <string>
#include <iostream>

using namespace std;

class ElegantList {
public:
    ElegantList() : os(std::cout) {}
    ElegantList(ostream& out_stream) : os(out_stream) {}

    void print(const string& data) {
        if (data == "(") {
            os << endl;
            ++level;
            printTab();
        }
        os << data << ' ';
        if (data == ")") {
            --level;
        }
    }
private:
    void printTab() {
        for (int i = 1; i < level; ++i)
            os << "    ";
    }
    ostream& os;
    int level;
};



#endif /* end of include guard: ELEGANT_LIST_H */
