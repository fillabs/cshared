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

void ctree_splay(cnode_t * n)
{
    __splay(n);
}


cnode_t * _ctree_splay_add (cnode_t ** proot, ctree_compare_fn* comparator, cnode_t * n, int replace)
{
    // insert node
    cnode_t * x = *proot;
    cnode_t * y = NULL;
	int l;
    
    while (x != NULL) {
        if (x == n)
            return n;
		y = x;
        l = comparator(y, n);
        if (l == 0) { // equal. 
            __splay(x);
            if (replace) {
                *n = *x;
                if(x->childs[1]){
                    x->childs[1]->parent = n;
                }
		        if (x->childs[0]){
                    x->childs[0]->parent = n;
                }
                n->parent = NULL;
                *proot = n;
            }else{
                *proot = x;
            }
            // return previous value
            return x;
        } else {
            l = (l<0) ? 1 : 0;
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
    return n;
}

static cnode_t * __find(cnode_t * x, ctree_compare_fn* comparator, const void * w)
{
    while(x) {
        if(x == w)
            break;
        int l = comparator(x, w);
        if(l == 0)
            break;
        l = (l<0) ? 1 : 0;
        x = x->childs[l];
    };
    return x;
}

cnode_t * _ctree_splay_find (cnode_t ** proot, ctree_compare_fn* comparator, const void * w)
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

/*
#define __cmin(x) __lastElement(x, 0)
#define __cmax(x) __lastElement(x, 1)
*/

static cnode_t * __join(cnode_t * s, cnode_t * t)
{
	s = __lastElement(s, 1);
	__splay(s);
	s->childs[1] = t;
	t->parent = s;
	return s;
}

static cnode_t * __join_childs(cnode_t * x)
{
    cnode_t * o;
    if(NULL == x->childs[1]){
        if(NULL == x->childs[0]) return NULL;
        o = x->childs[0];
        x->childs[0] = NULL;
    }else if(NULL == x->childs[0]){
        o = x->childs[1];
        x->childs[1] = NULL;
    }else{
        x->childs[0]->parent = NULL;
        x->childs[1]->parent = NULL;
        o = __join(x->childs[0], x->childs[1]);
        x->childs[0] = x->childs[1] = NULL;
    }
    o->parent = NULL;
    return o;
}

cnode_t *  _ctree_splay_del (cnode_t ** proot, ctree_compare_fn* comparator, const void * w)
{
    cnode_t * x = __find(*proot, comparator, w);
    if(x) {
		__splay(x);
        *proot = __join_childs(x);
        
    }
    return x;
}

cnode_t *  ctree_splay_del_node (cnode_t ** proot, cnode_t *x)
{
    cnode_t * p = x->parent;
    cnode_t * j = __join_childs(x);
    if(p == NULL){
        *proot = j;
    }else {
        if(j) j->parent = p;
        if(p->childs[0] == x) p->childs[0] = j;
        else                  p->childs[1] = j;
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
/*
inline static cnode_t * __swap( cnode_t ** p, cnode_t * n)
{
    cnode_t * ret = *p;
    *p = n;
    return ret;
}
*/
#define node_swap(P, N) cmem_swap(P, N)

void    _ctree_clean(cnode_t ** root, ctree_walk_fn * cb, void * const user)
{
    cnode_t * x = *root;
    int height = 0;
    while(x){
        if(x->childs[0]){
            height ++;
            x = node_swap(&x->childs[0], NULL);
        }else if(x->childs[1]){
            height ++;
            x = node_swap(&x->childs[1], NULL);
        }else{
            cnode_t * n = x;
            x = node_swap(&x->parent, NULL);
            cb(n, height, user);
            height --;
        }
    }
    *root = NULL;
}

void    _ctree_filter(cnode_t ** root, ctree_walk_fn * filter,  ctree_walk_fn * free, void * const user)
{
    cnode_t * x = *root;
    int height = 0;
    cnode_t *o = x;
    while(x){
        if(o == x){
            if(x->childs[0]){
                o = x = x->childs[0];
                height++;
                continue;
            }
            if(x->childs[1]){
                o = x = x->childs[1];
                height++;
                continue;
            }
        }else{
            if(o == x->childs[0]){
                // back from left
                if(x->childs[1]){
                    o = x = x->childs[1];
                    height++;
                    continue;
                }
            }
            o = x;
        }
        // call cb
        x = x->parent;
        if(0 == filter(o, height, user)){
            cnode_t * j = __join_childs(o);
            if(j) j->parent = x;
            if(x){
                if(x->childs[0] == o)
                    x->childs[0] = j;
                else 
                    x->childs[1] = j;
            }else{
                *root = j;
            }
            if(free){
                free(o, height, user);
            }
            o = NULL;
        }
        height--;
    }
}

const cnode_t * _ctree_next_node_up(const cnode_t * l, const cnode_t ** px)
{
    const cnode_t * x = *px;
    if (x == NULL){
        return NULL;
    }
    while (x) {
        if (l == x){
            if (x->childs[0]) {
                l = x = x->childs[0];
                continue;
            }
            if (x->childs[1]) {
                l = x = x->childs[1];
                continue;
            }
        }else{
            if (l == x->childs[0]){
                // back from left
                if (x->childs[1]){
                    l = x = x->childs[1];
                    continue;
                }
            }
            l = x;
        }
        *px = x->parent;
        break;
    }
    return l;
}

void _ctree_filter_down (cnode_t ** root, ctree_walk_fn * filter,  ctree_walk_fn * free, void * const user)
{
    cnode_t * x = *root;
    int height = 0;
    cnode_t *o = x;
    while(x){
        if(o == x){
            // check this node
            int rc = filter(x, height, user);
            if(rc > 0){
                // keep node
                // continue traverse
                if(x->childs[0]){
                    // go down left
                    o = x = x->childs[0];
                    height++;
                    continue;
                }
                if(x->childs[1]){
                    // go down right
                    o = x = x->childs[1];
                    height++;
                    continue;
                }
                // no more childs
                // go up but keep 'o' pointing to the node
            }else{
                //remove node or subtree
                cnode_t * j = NULL;
                if(rc == 0){
                    // remove node
                    j = __join_childs(o);
                }
                x = x->parent;
                if(x){
                    x->childs[__is_right(o)] = j;
                }else{
                    *root = j;
                }
                o->parent = NULL;
                if(free){
                    if(rc==0){
                        free(o, height, user);
                    }else{
                        _ctree_clean(&o, free, user);
                    }
                }
                o = NULL;
                continue;
            }
        }else{
            if(o == x->childs[0]){
                // back from left
                if(x->childs[1]){
                    o = x = x->childs[1];
                    height++;
                    continue;
                }
            }
            o = x;
        }
        x = x->parent;
        height--;
    }
}

const cnode_t * _ctree_next_node_down(const cnode_t * l, const cnode_t ** px)
{
    // TODO: implement it
    return _ctree_next_node_up(l, px);
}

const cnode_t * ctree_check_consistence(const cnode_t * root)
{
    const cnode_t * x = root;
    const cnode_t *o = x;
    while(x){
        if(o == x){
            // continue traverse
            if(x->childs[0]){
                // go down left
                o = x = x->childs[0];
                continue;
            }
            if(x->childs[1]){
                // go down right
                o = x = x->childs[1];
                continue;
            }
            // no more childs
            // go up but keep 'o' pointing to the node
        }else{
            if(o == x->childs[0]){
                // back from left
                if(x->childs[1]){
                    o = x = x->childs[1];
                    continue;
                }
            }else if(o != x->childs[1]){
                return o;
            }
            o = x;
        }
        x = x->parent;
    }    
    return NULL;
}
