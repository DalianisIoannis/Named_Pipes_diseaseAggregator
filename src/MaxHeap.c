#include "../headers/MaxHeap.h"

MaxHeapPtr initMaxHeap(){
    MaxHeapPtr tree = malloc(sizeof(MaxHeap));
    if(tree==NULL){ return NULL; }
    tree->root = NULL;
    return tree;
}

void emptyMaxHeapNode(HeapNodePtr node){
    if(node==NULL){ return; }
    else{
        emptyMaxHeapNode(node->left);
        emptyMaxHeapNode(node->right);
        node->occurence = NULL;
        node->right = NULL;
        node->left  = NULL;
        free(node);
    }
}

void emptyMaxHeap(MaxHeapPtr tree){
    emptyMaxHeapNode(tree->root);
    free(tree);
}

void printMaxHeapNode(HeapNodePtr node, int space){
    int i;
    if(node==NULL){
        space += 10;
        for(i=10; i<space; i++) { printf(" "); }
        printf("NULL");
        return;
    }
    space += 10;
    printMaxHeapNode(node->right, space);
    printf("\n");
    for(i=10; i<space; i++){ printf(" "); }
    if( strcmp(node->occurence, "$$$$$$$$$$$")!=0 ){
        printf("%s, %d\n", node->occurence, node->total);
    }
    else{
        printf("NULL");
    }
    printMaxHeapNode(node->left, space);
}

// incresases total of occurences 
bool searchAllexisting(HeapNodePtr *father, char *item){
    if( (*father)==NULL ){
        return false;
    }
    else{
        if( strcmp( (*father)->occurence, item )==0 ){
            (*father)->total++;
            return true;
        }
        bool tmp = searchAllexisting( &(*father)->left, item );
        if(tmp==true){ return true; }
        return searchAllexisting( &(*father)->right, item );
    }
    return false;
}

void findParentOfInsertedNode(HeapNodePtr node, int insId, HeapNodePtr *father){
    if( node->id==(insId/2) ){
        (*father) = node;   // under this father we insert the node
    }
    if( node->left!=NULL ){
        findParentOfInsertedNode( node->left, insId, father );
    }
    if( node->right!=NULL ){
        findParentOfInsertedNode( node->right, insId, father );
    }
}

void insertNodetoMaxHeap(HeapNodePtr *root, HeapNodePtr new_node){
    HeapNodePtr parent;
    if( new_node->id==1 ){  // put in root
        (*root) = new_node;
    }
    else{
        findParentOfInsertedNode( (*root), new_node->id, &parent );
        if( (new_node->id)%2==1 ){
            parent->right = new_node;
        }
        else{
            parent->left = new_node;
        }
    }
}

void compareFatherWithChild(HeapNodePtr *father, HeapNodePtr *child){
    // printf("Compare father with virus %s and total occurences %d with child with virus %s and total occurences %d.\n", (*father)->occurence, (*father)->total, (*child)->occurence, (*child)->total);
    if( (*father)->total< (*child)->total ){
        int tmpInt;
        char *tmpStr;

        tmpInt = (*father)->total;
        (*father)->total = (*child)->total;
        (*child)->total = tmpInt;

        tmpStr = (*father)->occurence;
        (*father)->occurence = (*child)->occurence;
        (*child)->occurence = tmpStr;
    }
}

void reheapify(HeapNodePtr *node){
    if( (*node)==NULL ){ return; }
    if( (*node)->left!=NULL ){
        reheapify( &((*node)->left) );
    }
    if( (*node)->right!=NULL ){
        reheapify( &((*node)->right) );
    }
    if( (*node)->right!=NULL ){
        if( (*node)->left!=NULL ){
            if( ((*node)->left)->total > ((*node)->right)->total ){
                compareFatherWithChild( node, &((*node)->left) );   // compare current with left child
            }
            else{
                compareFatherWithChild( node, &((*node)->right) );  // compare current with right child
            }
        }
        else{
            compareFatherWithChild( node, &((*node)->right) );  // compare current with right child
        }
    }
    else{
        if( (*node)->left!=NULL ){
            compareFatherWithChild( node, &((*node)->left) );   // compare current with left child
        }
    }
}

HeapNodePtr getNodeWithId(HeapNodePtr treenode, int id, HeapNodePtr *father){
    HeapNodePtr tmp = NULL;
    if( treenode==NULL ){
        return NULL;
    }
    else{
        if( treenode->id == id ){
            return treenode;
        }
        else{
            *father = treenode;
            tmp =  getNodeWithId(treenode->right, id, father);
            if( tmp == NULL ){
                *father = treenode;
                tmp = getNodeWithId(treenode->left, id, father);
            }
            return tmp;
        }
    }
    return NULL;
}

// replace root with last inserted
void printKlargestItems(MaxHeapPtr tree, int k, int *id){

    // printf("I am going to print the %d largest items of Heap:\n", k);
    // printMaxHeapNode(tree->root, 0); printf("\n\n\n");

    // // int began = k;
    // while(k>0 && tree->root!=NULL){
    //     if( strcmp(tree->root->occurence, "$$$$$$$$$$$")!=0 ){ printf("%s %d\n", tree->root->occurence, tree->root->total); }
    //     tree->root->occurence = "$$$$$$$$$$$";
    //     tree->root->total = 0;
    //     reheapify( &(tree->root) );
    //     k--;
    // }

    while( k>0 && tree->root!=NULL ){
        k--;
        HeapNodePtr father = NULL;
        HeapNodePtr tmp = getNodeWithId(tree->root, (*id)--, &father);

        printf("%s %d\n", tree->root->occurence, tree->root->total);

        if( tmp==NULL || strcmp(tmp->occurence, tree->root->occurence)==0 ){
            emptyMaxHeapNode(tree->root);
            tree->root = NULL;
        }
        else{
            if( father!=NULL ){
                if( father->right!=NULL ){
                    if( father->right->id==(*id)+1 ){
                        father->right = NULL;
                    }
                }
                else{
                    father->left = NULL;
                }
            }

            HeapNodePtr old_last = tmp;
            HeapNodePtr right_child_of_old_root = tree->root->right;
            HeapNodePtr left_child_of_old_root  = tree->root->left;
            
            tree->root->left = NULL;
            tree->root->right = NULL;
            HeapNodePtr tmp_root = tree->root;
            emptyMaxHeapNode(tmp_root);

            tree->root = old_last;
            tree->root->right = right_child_of_old_root;
            tree->root->left = left_child_of_old_root;
        }
        reheapify( &(tree->root) );
    }
}

bool addMaxHeapNode(MaxHeapPtr tree, char *item, int *id){

    if( searchAllexisting( &(tree->root), item )==true ){
        reheapify( &(tree->root) );  // reheapify CAN BE AFTER EVERY NODE OR IN THE END
    }
    else{
        HeapNodePtr node = malloc(sizeof(HeapNode));
        if(node==NULL){
            printf("Couldn't allocate Heap Node.\n");
            return false;
        }
        node->occurence = item;
        node->total     = 1;
        node->id        = (*id);
        node->left      = NULL;
        node->right     = NULL;
        insertNodetoMaxHeap( &(tree->root), node );
        (*id)++;
    }
    return true;
}