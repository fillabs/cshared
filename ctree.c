#include "ctree.h"
#include "cmem.h"

static inline int __is_right( cnode_t * x){
    return x->parent->childs[1] == x;
}

static void __left_rotate(cnode_t * x){
    cnode_t * y = x->childs[1];
    x->childs[1] = y->childs[0];
    if(y->childs[0]){
        y->childs[0]->parent = x;
    }
    y->parent = x->parent;
    if (x->parent){
        x->parent->childs[__is_right(x)] = y;
    }
	y->childs[0] = x;
	x->parent = y;
}

static void __right_rotate(cnode_t * x){
    cnode_t * y = x->childs[0];
    x->childs[0] = y->childs[1];
    if(y->childs[1]){
		y->childs[1]->parent = x;
    }
    y->parent = x->parent;
    if (x->parent){
        x->parent->childs[__is_right(x)] = y;
    }
	y->childs[1] = x;
	x->parent = y;
}

static void __splay(cnode_t * x){
    while(x->parent != NULL){
        if(x->parent->parent == NULL){
            if (x == x->parent->childs[0]){
                // zig rotation
                __right_rotate(x->parent);
            }else{
                // zag rotation
                __left_rotate(x->parent);
            }
        }else{
            int p_is_right = __is_right(x->parent);
            if(__is_right(x)){
                if(p_is_right){
    				// zag-zag rotation
	    			__left_rotate(x->parent->parent);
		    		__left_rotate(x->parent);
                }else{
    				// zig-zag rotation
    				__left_rotate(x->parent);
		    		__right_rotate(x->parent);
                }
            }else{
                if(p_is_right){
    				// zag-zig rotation
				    __right_rotate(x->parent);
				    __left_rotate(x->parent);
                }else{
    				// zig-zig rotation
	    			__right_rotate(x->parent->parent);
		    		__right_rotate(x->parent);
                }
            }
        }
    }
}

cnode_t * _ctree_splay_add (cnode_t ** proot, ctree_compare_fn* comparator, cnode_t * n, int replace)
{
    // insert node
    cnode_t * x = *proot;
    cnode_t * y = NULL;
    cnode_t * ret = n;
	int l;
    
    while (x != NULL) {
        if (x == n)
            return n;
		y = x;
        l = comparator(n, y);
        if (l == 0) { // equal. return previous value
            if (replace) {
                __splay(x);
                *n = *x;              
                if(x->childs[1]){
                    x->childs[1]->parent = n;
                }
		        if (x->childs[0]){
                    x->childs[0]->parent = n;
                }
                n->parent = NULL;
                *proot = n;
                return x;
            } else {
                return NULL;
            }
        } else {
            l = (l>0);
        }
        x = x->childs[l];
    }

    // y is parent of x
    n->parent = y;
    n->childs[0] = n->childs[1] = NULL;
    
    if(y){
        y->childs[l] = n;
        __splay(n);
    }
    *proot = n;
    return ret;
}

static cnode_t * __find(cnode_t * x, ctree_compare_fn* comparator, const cnode_t * w)
{
    while(x) {
        if(x == w)
            break;
        int l = comparator(w, x);
        if(l == 0)
            break;
        l = (l>0);
        x = x->childs[l];
    };
    return x;
}

cnode_t * _ctree_splay_find (cnode_t ** proot, ctree_compare_fn* comparator, const cnode_t * w)
{
    cnode_t * x;
    x = __find(*proot, comparator, w);
    if(x){
        __splay(x);
        *proot = x;
    }
    return x;
}

static inline cnode_t * __lastElement(cnode_t * x, int is_right)
{
    while(x->childs[is_right]){
        x = x->childs[is_right];
    }
    return x;
}
#define __min(x) __lastElement(x, 0)
#define __max(x) __lastElement(x, 1)

static cnode_t * __join(cnode_t * s, cnode_t * t)
{
    if (s == NULL) return t;
    if (t == NULL) return s;

	s = __max(s);
	__splay(s);
	s->childs[1] = t;
	t->parent = s;
	return s;
}

cnode_t *  _ctree_splay_del (cnode_t ** proot, ctree_compare_fn* comparator, const cnode_t * w)
{
    cnode_t * x = __find(*proot, comparator, w);
    if(x) {
		__splay(x);

        if(x->childs[1]){
            x->childs[1]->parent = NULL;
        }

		if (x->childs[0]){
            x->childs[0]->parent = NULL;
        }

        *proot = __join(x->childs[0], x->childs[1]);
        x->childs[0] = x->childs[1] = NULL;
    }
    return x;
}

static int __inorder(cnode_t  * x, int height, ctree_walk_fn * cb, void * const user)
{
    int rc = 0;
    if(x) {
        int rc = cb(x, height, user);
        if(rc >= 0){
            int n = __inorder(x->childs[0], height + 1, cb, user);
            if (n >=0) {
                rc += n;
                n = __inorder(x->childs[1], height + 1, cb, user);
                if (n >=0) {
                    return rc + n;
                }
            }
            rc = n;
        }
    }
    return rc;
}

static int __preorder(cnode_t  * x, int height, ctree_walk_fn * cb, void * const user)
{
    int rc = 0;
    if(x) {
        int rc = __preorder(x->childs[0], height + 1, cb, user);
        if(rc >= 0){
            int n = cb(x, height, user);
            if (n >=0) {
                rc += n;
                n = __preorder(x->childs[1], height + 1, cb, user);
                if (n >=0) {
                    return rc + n;
                }
            }
            rc = n;
        }
    }
    return rc;
}

int _ctree_splay_walk_preorder(cnode_t  * x, ctree_walk_fn * cb, void * const user)
{
    return __preorder(x, 0, cb, user);
}

int _ctree_splay_walk_inorder(cnode_t  * x, ctree_walk_fn * cb, void * const user)
{
    return __inorder(x, 0, cb, user);
}
