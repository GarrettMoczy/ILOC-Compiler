#include "ir.h"
#include "iostream"

const char* getOpcodeName(int opcode) {
    switch (opcode) {
        case OP_ADD:    return "add";
        case OP_SUB:    return "sub";
        case OP_MULT:   return "mult";
        case OP_LSHIFT: return "lshift";
        case OP_RSHIFT: return "shift";
        case OP_LOAD:   return "load";
        case OP_STORE:  return "store";
        case OP_LOADI:  return "loadI";
        case OP_OUTPUT: return "output";
        case OP_NOP:    return "nop";
        default:        return "UNKNOWN";
    }
}

ir::Node::Node() : prev(nullptr), next(nullptr), line(0), opcode(0) {
    for (int i = 0; i < 3; ++i) {
        args[i] = {0, 0, 0, 0};
    }
}

ir::Node::Node(int _line, int _opcode, int _arg1, int _arg2, int _arg3, Node* _prev, Node* _next) 
: prev(_prev), next(_next), line(_line), opcode(_opcode) 
{
    args[0] = {_arg1, -1, -1, -1};
    args[1] = {_arg2, -1, -1, -1};
    args[2] = {_arg3, -1, -1, -1};
}

void ir::Node::print() const {
    printf("[Line %d | %s", line, getOpcodeName(opcode));

    // Function to print an individual argument if it's not -1
    auto printArg = [](const char* label, int value) {
        if (value != -1) {
            printf(" | %s: %d", label, value);
        }
    };

    for (int i = 0; i < 3; i++) { 
        printArg("sr", args[i].sr);
        printArg("vr", args[i].vr);
        printArg("pr", args[i].pr);
        printArg("nu", args[i].nu);
    }
    
    printf(" ]\n");
}


ir::ir() {
    tail.prev = &head; 
    head.next = &tail;
    _size = 0;
}

ir::~ir() {
    clear();
}

void ir::clear() {
    Node * curr = head.next;
    while(curr != &tail) {
        Node * temp = curr;
        curr = curr->next;
        delete temp;
    }
    head.next = &tail;
    tail.prev = &head;
    _size = 0;
}

void ir::print() {
    Node * curr = head.next;
    while(curr != &tail) {
        curr->print();
        curr = curr->next;
    }
}

void ir::emplace_back(int line, int opcode, int arg1, int arg2, int arg3) {
    Node * node = new Node (line, opcode, arg1, arg2, arg3, tail.prev, &tail);
    tail.prev->next = node;
    tail.prev = node;
    _size++; 
}

