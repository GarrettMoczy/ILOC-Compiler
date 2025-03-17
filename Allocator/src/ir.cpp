#include "../includes/ir.h"
#include "iostream"
#include <vector>
#include <queue>

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
}

ir::Node::Node(int _line, int _opcode, int _arg1, int _arg2, int _arg3, Node* _prev, Node* _next) 
: prev(_prev), next(_next), line(_line), opcode(_opcode)
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
    _maxSR = -1;
    _maxLive = 0;
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
    _maxSR = 0;
    _maxLive = 0;
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
    if (reg > _maxSR)
        _maxSR = reg;
}

void ir::rename() {
    if (_maxSR == -1) {
        std::cerr << "No source registers found\n";
        return;
    }
    if (_size == 0) {
        std::cerr << "No IR exists to rename\n";
        return;
    }

    std::vector<int> SRtoVR(_maxSR+1, -1);
    std::vector<int> LU(_maxSR+1, INT32_MAX);

    int vrName = 0;
    int currLive = 0;
    int index = tail.prev->line;
    

    Node * curr = tail.prev;

    while(curr != &head) {
        if (curr->opcode == OPCODE::OP_OUTPUT) {
            curr->args[0].vr = curr->args[0].sr;
            curr = curr->prev;
            continue;
        }
        if (curr->args[2].sr != -1)  {
            if (SRtoVR[curr->args[2].sr] == -1) {
                SRtoVR[curr->args[2].sr] = vrName++;
                currLive++;
            }
            curr->args[2].vr = SRtoVR[curr->args[2].sr];
            curr->args[2].nu = LU[curr->args[2].sr];
            SRtoVR[curr->args[2].sr] = -1;
            currLive--;
            LU[curr->args[2].sr] = INT32_MAX;
        }
        if (currLive > _maxLive)
            _maxLive = currLive;
        if (curr->opcode == OPCODE::OP_LOADI) {
            curr->args[0].vr = curr->args[0].sr;
            curr = curr->prev;
            continue;
        }
        for(int i = 0; i < 2; i++) {
            if (curr->args[i].sr != -1) {
                if (SRtoVR[curr->args[i].sr] == -1) {
                    SRtoVR[curr->args[i].sr] = vrName++;
                    currLive++;
                }
                curr->args[i].vr = SRtoVR[curr->args[i].sr];
                curr->args[i].nu = LU[curr->args[i].sr];
                LU[curr->args[i].sr] = index;
            }
        }
        if (currLive > _maxLive)
            _maxLive = currLive;
        index--;
        curr = curr->prev;
    }
    _maxVR = vrName;
}

void ir::vrToCode() {
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
            printf("loadI %d => r%d\n", curr->args[0].vr, curr->args[2].vr);
        }
        else if (curr->opcode == OPCODE::OP_OUTPUT) {
            printf("output %d\n", curr->args[0].vr);
        }
    }
}

void ir::prToCode() {
    for(Node* curr = head.next; curr != &tail; curr = curr->next) {
        if (curr->opcode <= OPCODE::OP_RSHIFT) {
            printf("%s r%d, r%d => r%d \n", 
                opcodeToString(curr->opcode), curr->args[0].pr, curr->args[1].pr, curr->args[2].pr );
        }
        else if (curr->opcode == OPCODE::OP_LOAD) {
            printf("load r%d => r%d\n", curr->args[0].pr, curr->args[2].pr );
        }
        else if (curr->opcode == OPCODE::OP_STORE) {
            printf("store r%d => r%d\n", curr->args[0].pr, curr->args[1].pr );
        }
        else if (curr->opcode == OPCODE::OP_LOADI) {
            printf("loadI %d => r%d\n", curr->args[0].pr, curr->args[2].pr);
        }
        else if (curr->opcode == OPCODE::OP_OUTPUT) {
            printf("output %d\n", curr->args[0].pr);
        }
    }
}

void ir::spill(int spilledVR, int spilledPR, int reservedPR,  int location, Node * curr) {

    Node * loadI = new Node(curr->line, OPCODE::OP_LOADI, -1, -1, -1, curr->prev, nullptr);
    loadI->args[0].vr = location;
    loadI->args[0].pr = location;
    loadI->args[2].pr = reservedPR;
    Node * store = new Node(curr->line + 1, OPCODE::OP_STORE, -1, -1, -1, loadI, curr);
    loadI->next = store;
    store->args[0].vr = spilledVR;
    store->args[0].pr = spilledPR;
    store->args[1].pr = reservedPR;

    curr->prev->next = loadI;
    curr->prev = store;
};

void ir::restore(int restoredVR, int restoredPR, int reservedPR, int location, Node * curr) {

    Node * loadI = new Node(curr->line, OPCODE::OP_LOADI, -1, -1, -1, curr->prev, nullptr);
    loadI->args[0].vr = location;
    loadI->args[0].pr = location;
    loadI->args[2].pr = reservedPR;
    Node * load = new Node(curr->line + 1, OPCODE::OP_LOAD, -1, -1, -1, loadI, curr);
    loadI->next = load;
    load->args[0].pr = reservedPR;
    load->args[2].vr = restoredVR;
    load->args[2].pr = restoredPR;

    curr->prev->next = loadI;
    curr->prev = load;
};

void ir::allocate(int k) {
    std::vector<int> PRtoVR;
    std::vector<int> PRNU;
    int spillLoc = 65536;
    int reservedPR = -1;
    if (k < _maxLive) {
        reservedPR = --k;   
    }

    PRtoVR = std::vector<int>(k, -1);
    PRNU = std::vector<int>(k, -1);
    

    std::vector<int> VRtoPR(_maxVR + 1, -1);
    std::vector<int> VRtoSpillLoc(_maxVR + 1, -1);
    std::queue<int> freePR;


    for(int i = 0; i < k; i++) {
        freePR.push(i);
    }

    for(Node * curr = head.next; curr != &tail; curr = curr->next) {

        //special case that only deals with constants
        if (curr->opcode == OPCODE::OP_OUTPUT)
        {
            curr->args[0].pr = curr->args[0].sr;
            continue;
        }


        //code for uses
        if (curr->opcode != OPCODE::OP_LOADI) 
        {
            for(int i = 0; i < 2; i++) {
                if (curr->args[i].vr != -1) {
                    int VR = curr->args[i].vr;
                        int PR = VRtoPR[VR];
                        if (PR != - 1) {
                            curr->args[i].pr = PR;
                            PRNU[PR] = curr->args[i].nu;
                            }
                        else if (VRtoSpillLoc[VR] != -1) {
                            int PR;
                            if (!freePR.empty()) {
                                PR = freePR.front();
                                freePR.pop();
                            }
                            else {
                                int nextUse = -1;
                                for(int i = 0; i < k; i++) {
                                    if (PRNU[i] > nextUse && curr->args[0].pr != i) {
                                        PR = i;
                                        nextUse = PRNU[i];
                                    }
                                }
                                // std::cout << PR << ": " << PRNU[PR] << '\n';
                                spill(PRtoVR[PR], PR, reservedPR, spillLoc,curr);
                                VRtoSpillLoc[PRtoVR[PR]] = spillLoc;
                                spillLoc += 4;
                                VRtoPR[PRtoVR[PR]] = -1;
                                
                            }
                            restore(VR, PR, reservedPR, VRtoSpillLoc[VR], curr);
                            curr->args[i].pr = PR;
                            PRNU[PR] = curr->args[i].nu;
                            PRtoVR[PR] = VR;
                            VRtoPR[VR] = PR;
                            VRtoSpillLoc[VR] = -1;
                        }          
                    }
                }

            for(int i = 0; i < 2; i++) {
                if (curr->args[i].vr != -1) {
                    int VR = curr->args[i].vr;
                    int PR = VRtoPR[VR];
                    if (PR != - 1) {
                        if (curr->args[i].nu == INT32_MAX) {
                            PRNU[PR] = -1;
                            PRtoVR[PR] = -1;
                            VRtoPR[VR] = -1;
                            freePR.push(PR);
                        }
                    }
                }
            }            
        }
        else 
        {
            curr->args[0].pr = curr->args[0].sr;
        }

        if (curr->args[2].vr != -1)  
        {
            int VR = curr->args[2].vr;
            int PR;
            if (!freePR.empty()) {
                PR = freePR.front();
                freePR.pop();
            }
            else {
                int nextUse = -1;
                for(int i = 0; i < k; i++) {
                    if (PRNU[i] > nextUse) {
                        PR = i;
                        nextUse = PRNU[i];
                    }
                }
                // std::cout << PR << ": " << PRNU[PR] << '\n';
                spill(PRtoVR[PR], PR, reservedPR, spillLoc, curr);
                VRtoSpillLoc[PRtoVR[PR]] = spillLoc;
                spillLoc += 4;
                VRtoPR[PRtoVR[PR]] = -1;
                
            }
            curr->args[2].pr = PR;
            PRNU[PR] = curr->args[2].nu;
            PRtoVR[PR] = VR;
            VRtoPR[VR] = PR;
            if (curr->args[2].nu == INT32_MAX) {
                PRNU[PR] = -1;
                PRtoVR[PR] = -1;
                VRtoPR[VR] = -1;
                freePR.push(PR);
            }
        }
    }
}


