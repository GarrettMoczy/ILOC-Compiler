#include "../includes/ir.h"
#include "iostream"
#include <vector>

const char* opcodeToString(int opcode) {
    switch (opcode) {
        case OP_ADD:    return "add";
        case OP_SUB:    return "sub";
        case OP_MULT:   return "mult";
        case OP_LSHIFT: return "lshift";
        case OP_RSHIFT: return "rshift";
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
        args[i] = {-1, -1, -1, -1};
    }
    maxSR = -1;
}

ir::Node::Node(int _line, int _opcode, int _arg1, int _arg2, int _arg3, Node* _prev, Node* _next) 
: prev(_prev), next(_next), line(_line), opcode(_opcode), maxSR(-1)
{
    args[0] = {_arg1, -1, -1, -1};
    args[1] = {_arg2, -1, -1, -1};
    args[2] = {_arg3, -1, -1, -1};
}

void ir::Node::print() const {
    printf("[Line %d | %s", line, opcodeToString(opcode));

    // Function to print an individual argument if it's not -1
    auto printArg = [](const char* label, int value) {
        if (value != -1) 
            printf("%s: %d ", label, value);
    };

    for (int i = 0; i < 3; i++) { 
        printf(" | ");
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

void ir::updateSR(int reg) {
    if (reg > maxSR)
        maxSR = reg;
}

void ir::rename() {
    if (maxSR == -1) {
        std::cerr << "No source registers found\n";
        return;
    }
    if (_size == 0) {
        std::cerr << "No IR exists to rename\n";
        return;
    }

    std::vector<int> SRtoVR(maxSR+1, -1);
    std::vector<int> LU(maxSR+1, INT32_MAX);

    int vrName = 0;
    int index = tail.prev->line;

    Node * curr = tail.prev;

    while(curr != &head) {
        if (curr->opcode == OPCODE::OP_OUTPUT) {
            curr = curr->prev;
            continue;
        }
        if (curr->args[2].sr != -1)  {
            if (SRtoVR[curr->args[2].sr] == -1)
                SRtoVR[curr->args[2].sr] = vrName++;
            curr->args[2].vr = SRtoVR[curr->args[2].sr];
            curr->args[2].nu = LU[curr->args[2].sr];
            SRtoVR[curr->args[2].sr] = -1;
            LU[curr->args[2].sr] = INT32_MAX;
        }
        if (curr->opcode == OPCODE::OP_LOADI) {
            curr = curr->prev;
            continue;
        }
        for(int i = 0; i < 2; i++) {
            if (curr->args[i].sr != -1) {
                if (SRtoVR[curr->args[i].sr] == -1)
                    SRtoVR[curr->args[i].sr] = vrName++;
                curr->args[i].vr = SRtoVR[curr->args[i].sr];
                curr->args[i].nu = LU[curr->args[i].sr];
                LU[curr->args[i].sr] = index;
            }
        }
        index--;
        curr = curr->prev;
    }
}

void ir::irToCode() {
    for(Node* curr = head.next; curr != &tail; curr = curr->next) {
        if (curr->opcode <= OPCODE::OP_RSHIFT) {
            printf("%s r%d, r%d => r%d\n", 
                opcodeToString(curr->opcode), curr->args[0].vr, curr->args[1].vr, curr->args[2].vr );
        }
        else if (curr->opcode == OPCODE::OP_LOAD) {
            printf("load r%d => r%d\n", curr->args[0].vr, curr->args[2].vr );
        }
        else if (curr->opcode == OPCODE::OP_STORE) {
            printf("store r%d => r%d\n", curr->args[0].vr, curr->args[1].vr );
        }
        else if (curr->opcode == OPCODE::OP_LOADI) {
            printf("loadI %d => r%d\n", curr->args[0].sr, curr->args[2].vr);
        }
        else if (curr->opcode == OPCODE::OP_OUTPUT) {
            printf("output %d\n", curr->args[0].sr);
        }
    }
}
