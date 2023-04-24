#ifndef ctree_h
#define ctree_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct cnode_t cnode_t;

struct cnode_t{
    cnode_t * parent;
    cnode_t * childs[2];
};

typedef int (ctree_compare_fn) (const cnode_t * a, const cnode_t * b);
typedef int (ctree_walk_fn)(cnode_t * node, int height, void * const user);

cnode_t *  _ctree_splay_add (cnode_t ** root, ctree_compare_fn* comparator, cnode_t * n, int replace);
#define ctree_splay_add(root, comparator, n, replace) _ctree_splay_add (root, (ctree_compare_fn*) (comparator), (cnode_t*) (n), (replace)) 
cnode_t *  _ctree_splay_find(cnode_t ** root, ctree_compare_fn* comparator, const cnode_t * w);
#define ctree_splay_find(root, compare, w) _ctree_splay_find(root, (ctree_compare_fn*) compare, (const cnode_t *) w)
cnode_t *  _ctree_splay_del(cnode_t ** root, ctree_compare_fn* comparator, const cnode_t * w);
#define ctree_splay_del(root, compare, w) _ctree_splay_del(root, (ctree_compare_fn*) compare, (const cnode_t *) w)
int        _ctree_splay_walk_inorder(cnode_t  * root, ctree_walk_fn * cb, void * const user);
#define ctree_splay_walk_inorder(root, cb, user) _ctree_splay_walk_inorder(root, (ctree_walk_fn*) cb,  user)
int        _ctree_splay_walk_preorder(cnode_t  * root, ctree_walk_fn * cb, void * const user);
#define ctree_splay_walk_preorder(root, cb, user) _ctree_splay_walk_preorder(root, (ctree_walk_fn*) cb,  user)

#ifdef __cplusplus
}
#endif
#endif
