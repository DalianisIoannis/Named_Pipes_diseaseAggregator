#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct HeapNode{
    char    *occurence;
    int     total;
    int     id;
    struct HeapNode *right;
    struct HeapNode *left;
} HeapNode;

typedef struct HeapNode *HeapNodePtr;

typedef struct{
    HeapNodePtr root;
} MaxHeap;

typedef MaxHeap *MaxHeapPtr;


MaxHeapPtr initMaxHeap();
void emptyMaxHeap(MaxHeapPtr);
void emptyMaxHeapNode(HeapNodePtr);
void printMaxHeapNode(HeapNodePtr, int);
bool addMaxHeapNode(MaxHeapPtr, char*, int*);
bool searchAllexisting(HeapNodePtr*, char*);
void findParentOfInsertedNode(HeapNodePtr, int, HeapNodePtr*);
void insertNodetoMaxHeap(HeapNodePtr*, HeapNodePtr);
void reheapify(HeapNodePtr*);
void printKlargestItems(MaxHeapPtr, int, int*);
HeapNodePtr getNodeWithId(HeapNodePtr, int, HeapNodePtr*);