
#include "elegantlist.hpp"

ElegantList el;

int main() {

    el.print("(");
    el.print("function");
    el.print("hello");
        el.print("(");
        el.print("set");
        el.print("int");
        el.print("var");
        el.print(")");
        el.print("(");
            el.print("(");
            el.print("set");
            el.print("int");
            el.print("x");
            el.print(")");
            el.print("(");
            el.print("return");
            el.print("0");
            el.print(")");
        el.print(")");
    el.print(")");

    return 0;
}
