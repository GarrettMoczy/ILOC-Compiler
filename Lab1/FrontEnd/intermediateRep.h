//intermediateRep.h
#ifndef INTERMEDIATEREP_H
#define INTERMEDIATEREP_H

class intermediateRep
{
private:
    class Node
    {
    private:
        Node * next;
        Node * prev;


    public:
        Node(/* args */);
        ~Node() = default;
    };
    

    Node head, tail;
public:
    intermediateRep(/* args */);
    ~intermediateRep();
};




#endif