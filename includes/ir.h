//ir.h
#ifndef IR_H
#define IR_H

#include "enums.h"

class ir 
{
private:
    class Node {
        private:
            Node *prev, *next;
            int line, opcode;
            struct
            {
                int sr, vr, pr, nu;
            } args[3];

        public:
            Node();
            Node(int line, int opcode, int arg1, int arg2, int arg3, Node* prev, Node* next);
            ~Node() = default;
        

            void print() const;

            friend ir;
    };

    Node head, tail;
    int _size;

public:
    ir();
    ~ir();
    int size() {return _size;}
    void clear();
    void print();
    void emplace_back(int line, int opcode, int arg1, int arg2, int arg3);
};



#endif