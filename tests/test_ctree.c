#include "../ctree.h"
#include "../cmem.h"
#include "../cstr.h"

#include <stdio.h>
#include <assert.h>

typedef struct {
    cnode_t a;
    int key;
}Node;

int NodeCompare(const Node * a, const Node * b){
    return a->key - b->key;
}

cnode_t * NodeNew(int k)
{
    Node * n = cnew(Node);
    n->key = k;
    return &n->a;
}

static char _spaces [256];
void _printHelper(Node * n, char * indent, int last) {
    // print the tree structure on the screen
    if ( n ) {
        printf("%s", indent);
        char *s;
        if (last) {
            printf("└────");
            s = cstrcat (indent, "     ");

        } else {
            printf("├────");
            s = cstrcat (indent, "|    ");
        }
        printf("%d\n", n->key);

        _printHelper((Node*)n->a.childs[0], indent, 0);
        *s = 0;
        _printHelper((Node*)n->a.childs[1], indent, 1);
        *s = 0;
    }
}

void printTree(Node * n)
{
    _spaces[0] = 0;
    if(n){
        printf("%d\n", n->key);
        _printHelper((Node*)n->a.childs[0], _spaces, 0);
        _spaces[0] = 0;
        _printHelper((Node*)n->a.childs[1], _spaces, 1);
    }
    printf("\n");
}

int print_node(cnode_t * n, int height, void * const user)
{
    while(height--)
        printf("   ");
    printf("%d\n", ((Node*)n)->key);
    return 0; 
}

int main(int argc, char ** argv){

    cnode_t * root = NULL;
    cnode_t *x;
    Node * r;

    ctree_splay_add(&root, NodeCompare, (x=NodeNew(1)), 0);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(33)),0);
	ctree_splay_add(&root, NodeCompare, (x=NodeNew(44)),0);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(67)),0);
    printTree((Node*)root);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(5)), 0);
    printTree((Node*)root);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(89)),0);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(41)),0);
    ctree_splay_add(&root, NodeCompare, (x=NodeNew(98)),0);
    printTree((Node*)root);
    r = (Node*)ctree_splay_add(&root, NodeCompare, (x=NodeNew(1)), 0);
    assert(r == 0); free(x);
	r = (Node*)ctree_splay_add(&root, NodeCompare, (x=NodeNew(1)), 1);
    assert(r && r != (Node*)x && 0 == NodeCompare((Node*)x, r)); free(r);
    printTree((Node*)root);

    Node n = {{}, 33};
    r = (Node*)ctree_splay_find(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n));
    printTree((Node*)root);

    n.key = 44;
    r = (Node*)ctree_splay_find(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n));
    printTree((Node*)root);

    n.key = 40;
    r = (Node*)ctree_splay_find(&root, NodeCompare, &n);
    assert( r  == 0 );
    printTree((Node*)root);
/*
    n.key = 89;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    printTree((Node*)root);

    n.key = 67;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    printTree((Node*)root);
    
    n.key = 41;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    
    n.key = 5;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    
    n.key = 40;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r == 0);
    
    n.key = 98;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    
    n.key = 1;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    
    n.key = 44;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
    
    n.key = 33;
    r = (Node*)ctree_splay_del(&root, NodeCompare, &n);
    assert( r && 0 == NodeCompare(r, &n)); free(r);
*/
    printf("in order:\n");
    ctree_splay_walk_inorder(root, print_node, NULL);

    printf("pre order:\n");
    ctree_splay_walk_preorder(root, print_node, NULL);

    return 0;
}
