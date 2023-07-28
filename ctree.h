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
/**
 * @brief The function to be called to compare existing tree node with user provoded data.
 * @param node The existing node 
 * @param value The value provided by user
 * @return  zero     - if _node_ is equal to the _value_
 *          positive - if _node_ is bigger than the _value_
 *          negative - if _node_ is less than the _value_
 */
typedef int (ctree_compare_fn) (const cnode_t * node, const void * value);

/**
 * @brief The call-back function type
 * @param node   The existing tree node
 * @param height The distance from the node to the head of the tree.
 * @param user   The user-defined value
 * @return The meaning of the return value depends of the callee.
 */
typedef int (ctree_walk_fn)(cnode_t * node, int height, void * const user);

/** @brief Add new node to the tree.
 *        The node will be put on the top of the tree.
 * 
 *  @param root Pointer to the tree head.
 *  @param comparator Comparing function. @see ctree_compare_fn
 *  @param n New node to be added. Comparing function will be called with this pointer in the second argument.
 *  @param replace If true, replace the existing equal node by the new one, otherwise keep the existing one.
 *  @return The function returns _n_ when the equal node was not found in the tree, or the exisitng equal node.
 *  @defgroup ctree_splay_add
 */
#define ctree_splay_add(root, comparator, n, replace) _ctree_splay_add (root, (ctree_compare_fn*) (comparator), n, (replace)) 
cnode_t *  _ctree_splay_add (cnode_t ** root, ctree_compare_fn* comparator, cnode_t * n, int replace);

/** @brief Find the node in the tree
 * 
 *  @param root Pointer to the tree head.
 *  @param comparator Comparing function. Function is called with 2 arguments: node in the tree and the _w_ argument. @see ctree_compare_fn
 *  @param w The data to be looked in the tree. 
 *  @return Returns found node or NULL otherwise.
 *  @defgroup ctree_splay_find
 */
/**@{*/
#define ctree_splay_find(root, compare, w) _ctree_splay_find(root, (ctree_compare_fn*) compare,  w)
cnode_t *  _ctree_splay_find(cnode_t ** root, ctree_compare_fn* comparator, const void * w);
/**@}*/

/** @brief Find the node and delete it from the tree 
 *
 *  The function calls the _comparator_ function for every node in the tree with the _w_ value as a second argument.
 *  If _comparator_ returns 0, the node is deleted from the tree.
 * 
 *  @param root Pointer to the tree head.
 *  @param comparator Comparing function. Function is called with 2 arguments: node in the tree and the _w_ argument. @see ctree_compare_fn
 *  @param w The data to be deleted from the tree.
 *  @return Deleted node or NULL otherwise.
 *  @defgroup ctree_splay_del
 */
/**@{*/
#define ctree_splay_del(root, compare, w) _ctree_splay_del(root, (ctree_compare_fn*) compare,  w)
cnode_t *  _ctree_splay_del(cnode_t ** root, ctree_compare_fn* comparator, const void * w);
/**@}*/

/** @brief Delete the node from the tree
 * 
 *  @param root Pointer to the tree head.
 *  @param n Node to be deleted from the tree. 
 *  @return Returns _n_ 
 */
cnode_t *  ctree_splay_del_node (cnode_t ** root, cnode_t *n);

/** @defgroup Tree traversal
 *  @brief "In-order" and 'pre-order' traversal of the tree.
 * 
 *  The function calls the _cb_ function for each node in the tree.
 *  The function stops the traversal if _cb_ returns negative value.
 *  Otherwise the function collect all _cb_ return values and returns its summ.
 * 
 *  Note: The function is implemented using recursion. Be carreful about stack overflow.
 *        Use @see ctree_filter instead.
 * 
 *  @param root Pointer to the tree head.
 *  @param cb Call-back function to be called for each node. @see ctree_walk_fn
 *  @param user Any user pointer to be passed to the _cb_ function as a _user_ argument.
 *  @return The summ of all non-negative return values
 */
/**@{*/
#define ctree_splay_walk_inorder(root, cb, user) _ctree_splay_walk_inorder(root, (ctree_walk_fn*) cb,  user)
int        _ctree_splay_walk_inorder(cnode_t  * root, ctree_walk_fn * cb, void * const user);
#define ctree_splay_walk_preorder(root, cb, user) _ctree_splay_walk_preorder(root, (ctree_walk_fn*) cb,  user)
int        _ctree_splay_walk_preorder(cnode_t  * root, ctree_walk_fn * cb, void * const user);
/**@}*/

/** @brief Cleanup the tree.
 * 
 *  The function will call the _cb_ for each node in the tree. 
 * 
 *  Note: The function is free of stack overflow risk.
 * 
 *  @param root Pointer to the tree head. The pointing value will be set to NULL after the function call.
 *  @param cb Call-back function to be called for each node. The _cb_ return value is ignored. @see ctree_walk_fn
 *  @param user Any user pointer to be passed to the _cb_ function as a _user_ argument.
 *  @defgroup ctree_clean
 */
/**@{*/
#define ctree_clean(root, cb, user) _ctree_clean(root, (ctree_walk_fn*) cb,  user)
void    _ctree_clean(cnode_t ** root, ctree_walk_fn * cb, void * const user);
/**@}*/

void ctree_splay(cnode_t * n);

/** @brief Filter tree using given _filter_ function.
 * 
 *  - _ctree_filter_up_ traverse the tree from the leafs to the root,
 *    while the _ctree_filter_down_ traverse it from the root to leafs.

 *  @param root Pointer to the tree head.
 *  @param filter Function to be called for each node.
 *         Function shall return positive value to keep the node in the tree, zero to delete it
 *         or negative value to delete a full sub-tree.
 *  @param cb Call-back function to be called for each deleted node.
 *         The _cb_ return value is ignored. Set it to NULL if not needed. 
 *  @param user Any user pointer to be passed to the _filter_ and _cb_ functions as a _user_ argument.
 *  @defgroup ctree_filter
 */
/**@{*/
#define ctree_filter_up(root, filter, cb, user) _ctree_filter(root, (ctree_walk_fn*) filter,  (ctree_walk_fn*) cb,  user)
#define ctree_filter(root, filter, cb, user) ctree_filter_up(root, filter, cb, user)
void    _ctree_filter(cnode_t ** root, ctree_walk_fn * filter, ctree_walk_fn * cb, void * const user);

#define ctree_filter_down(root, filter, cb, user) _ctree_filter_down(root, (ctree_walk_fn*) filter,  (ctree_walk_fn*) cb,  user)
void     _ctree_filter_down (cnode_t ** root, ctree_walk_fn * filter,  ctree_walk_fn * free, void * const user);
/**@}*/

/** @brief Traverse the tree in-place without call-backs.
 * 
 *  @param ROOT The tree head.
 *  @param n The name of the variable to be used to traverse the tree
 
 *  The example:
 *  @code{.c}
 *  extern cnode_t * root; // already initialized tree head
 *  ctree_foreach(root,n) {
 *      // here we have an access to the variable _n_ of type cnode_t*  
 *  }
 *  @endcode
 */
/**@{*/
#define ctree_foreach_up(ROOT,n) for(const cnode_t * __last_ ## n = ROOT, *n = _ctree_next_node_up(ROOT, &__last_ ## n); n; n = _ctree_next_node_up(n, &__last_ ## n))
#define ctree_foreach(ROOT,n) ctree_foreach_up(ROOT,n)

#define ctree_foreach_down(ROOT,n) for(const cnode_t * __last_ ## n = ROOT, *n = _ctree_next_node_down(ROOT, &__last_ ## n); n; n = _ctree_next_node_down(n, &__last_ ## n))
/**@}*/
/** @internal */
const cnode_t * _ctree_next_node_up(const cnode_t * p, const cnode_t ** px);
const cnode_t * _ctree_next_node_down(const cnode_t * p, const cnode_t ** px);
/** @endinternal */

#ifdef __cplusplus
}
#endif
#endif
