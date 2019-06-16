#pragma once


  // Node and forward declaration because g++ does
  // not understand nested classes.
template <class Comparable>
class AvlTree;

template <class Comparable>
class AvlNode
{
    Comparable element;
    AvlNode   *left;
    AvlNode   *right;
    int        height;

    AvlNode( const Comparable & theElement, AvlNode *lt, AvlNode *rt, int h = 0 )
      : element( theElement ), left( lt ), right( rt ), height( h ) { }
    friend class AvlTree<Comparable>;
};

#include <iostream>       // For NULL
#include <functional>
#include <list>

// AvlTree class
//
// CONSTRUCTION: with ITEM_NOT_FOUND object used to signal failed finds
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// void remove( x )       --> Remove x (unimplemented)
// Comparable find( x )   --> Return item that matches x
// Comparable findMin( )  --> Return smallest item
// Comparable findMax( )  --> Return largest item
// list<Comparable&> findAllIf (p,q)--> returns a list of all items that statisfy p; goes through tree with q ordering (eg numeric order)
// q returns 1,0, -1. a 1 means go down right subtree, -1 means go left, 0 means dont go anywhere
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order


using namespace std;

template <class Comparable>
class AvlTree
{
  public:
    explicit AvlTree( const Comparable & notFound );
    AvlTree( const AvlTree & rhs );
    ~AvlTree( );

    const Comparable & findMin( ) const;
    const Comparable & findMax( ) const;
    const Comparable & find( const Comparable & x ) const;
    std::list<std::reference_wrapper<Comparable> > findAllIf( std::function<bool (Comparable)> p, std::function<int (Comparable)> q) const;

    int countIf( std::function<bool(Comparable)> p ) const;
    std::list<Comparable> collectIntoListIf( std::function<bool (Comparable)> p ) const;
    bool isEmpty( ) const;
    void printTree( ) const;
    std::ostream& printTreeToStream( std::ostream& os ) const;
    

    void makeEmpty( );
    int insert( const Comparable & x );
    void remove( const Comparable & x );

    const AvlTree & operator=( const AvlTree & rhs );
    
  private:
    AvlNode<Comparable> *root;

    const Comparable ITEM_NOT_FOUND;

    const Comparable & elementAt( AvlNode<Comparable> *t ) const;

    int insert( const Comparable & x, AvlNode<Comparable> * & t ) const;
    AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * find( const Comparable & x, AvlNode<Comparable> *t ) const;
    std::list<std::reference_wrapper<Comparable> > findAllIf( std::function<bool (Comparable)> p, std::function<int (Comparable)> q, AvlNode<Comparable> *t ) const;
    void makeEmpty( AvlNode<Comparable> * & t ) const;
    void printTree( AvlNode<Comparable> *t ) const;
    std::ostream& printTreeToStream( std::ostream& os, AvlNode<Comparable> *t ) const;
    AvlNode<Comparable> * clone( AvlNode<Comparable> *t ) const;
    int countIf( std::function<bool(Comparable)> p, AvlNode<Comparable> *t) const;
    std::list<Comparable> collectIntoListIf( std::function<bool (Comparable)> p, AvlNode<Comparable> *t ) const;

        // Avl manipulations
    int height( AvlNode<Comparable> *t ) const;
    int max( int lhs, int rhs ) const;
    void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
    void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
    void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
    void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;
};

// #include "AvlTree.cpp"

/**
 * Implements an unbalanced Avl search tree.
 * Note that all "matching" is based on the compares method.
 * @author Mark Allen Weiss
 * modified by: Ajani Stewart
 */
/**
 * Construct the tree.
 */
template <class Comparable>
AvlTree<Comparable>::AvlTree( const Comparable & notFound ) :
  root( NULL ), ITEM_NOT_FOUND( notFound )
{
}

/**
 * Copy constructor.
 */
template <class Comparable>
AvlTree<Comparable>::AvlTree( const AvlTree<Comparable> & rhs ) :
  ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ), root( NULL )
{
    *this = rhs;
}

/**
 * Destructor for the tree.
 */
template <class Comparable>
AvlTree<Comparable>::~AvlTree( )
{
    makeEmpty( );
}

/**
 * Insert x into the tree; duplicates are ignored.
 */
template <class Comparable>
int AvlTree<Comparable>::insert( const Comparable & x )
{
    return insert( x, root );
}

/**
 * Remove x from the tree. Nothing is done if x is not found.
 */
template <class Comparable>
void AvlTree<Comparable>::remove( const Comparable & x )
{
    cout << "Sorry, remove unimplemented; " << x <<
          " still present" << endl;
}

/**
 * Find the smallest item in the tree.
 * Return smallest item or ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin( ) const
{
    return elementAt( findMin( root ) );
}

/**
 * Find the largest item in the tree.
 * Return the largest item of ITEM_NOT_FOUND if empty.
 */
template <class Comparable>
const Comparable & AvlTree<Comparable>::findMax( ) const
{
    return elementAt( findMax( root ) );
}

/**
 * Find item x in the tree.
 * Return the matching item or ITEM_NOT_FOUND if not found.
 */
template <class Comparable>
const Comparable & AvlTree<Comparable>::
                          find( const Comparable & x ) const
{
    return elementAt( find( x, root ) );
}

/**
 * Make the tree logically empty.
 */
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( )
{
    makeEmpty( root );
}

/**
 * Test if the tree is logically empty.
 * Return true if empty, false otherwise.
 */
template <class Comparable>
bool AvlTree<Comparable>::isEmpty( ) const
{
    return root == NULL;
}

/**
 * Print the tree contents in sorted order.
 */
template <class Comparable>
void AvlTree<Comparable>::printTree( ) const
{
    if( isEmpty( ) )
        cout << "Empty tree" << endl;
    else
        printTree( root );
}

/**
 * Deep copy.
 */
template <class Comparable>
const AvlTree<Comparable> &
AvlTree<Comparable>::
operator=( const AvlTree<Comparable> & rhs )
{
    if( this != &rhs )
    {
        makeEmpty( );
        root = clone( rhs.root );
    }
    return *this;
}

/**
 * Internal method to get element field in node t.
 * Return the element field or ITEM_NOT_FOUND if t is NULL.
 */
template <class Comparable>
const Comparable & AvlTree<Comparable>::elementAt( AvlNode<Comparable> *t ) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

/**
 * Internal method to insert into a subtree.
 * x is the item to insert.
 * t is the node that roots the tree.
 */
template <class Comparable>
int AvlTree<Comparable>::insert( const Comparable & x, AvlNode<Comparable> * & t ) const
{
    int result = 0;
    if( t == NULL ) {
        t = new AvlNode<Comparable>( x, NULL, NULL );
        result = 1;
    }
    else if( x < t->element )
    {
        result = insert( x, t->left );
        if( height( t->left ) - height( t->right ) == 2 ) {
            if( x < t->left->element ) {
              rotateWithLeftChild( t );
            } else {
              doubleWithLeftChild( t );
            }
        }
    }
    else if( t->element < x )
    {
        result = insert( x, t->right );
        if( height( t->right ) - height( t->left ) == 2 ) {
            if( t->right->element < x ) {
              rotateWithRightChild( t );
            } else {
              doubleWithRightChild( t );
            }
        }
    }
    else
        ;  // Duplicate; do nothing
    t->height = max( height( t->left ), height( t->right ) ) + 1;
    return result;
}

/**
 * Internal method to find the smallest item in a subtree t.
 * Return node containing the smallest item.
 */
template <class Comparable>
AvlNode<Comparable> *
AvlTree<Comparable>::findMin( AvlNode<Comparable> *t ) const
{
    if( t == NULL)
        return t;

    while( t->left != NULL )
        t = t->left;
    return t;
}

/**
 * Internal method to find the largest item in a subtree t.
 * Return node containing the largest item.
 */
template <class Comparable>
AvlNode<Comparable> *
AvlTree<Comparable>::findMax( AvlNode<Comparable> *t ) const
{
    if( t == NULL )
        return t;

    while( t->right != NULL )
        t = t->right;
    return t;
}

/**
 * Internal method to find an item in a subtree.
 * x is item to search for.
 * t is the node that roots the tree.
 * Return node containing the matched item.
 */
template <class Comparable>
AvlNode<Comparable> *
AvlTree<Comparable>::find( const Comparable & x, AvlNode<Comparable> *t ) const
{
    while( t != NULL )
        if( x < t->element )
            t = t->left;
        else if( t->element < x )
            t = t->right;
        else
            return t;    // Match

    return NULL;   // No match
}

/**
 * Internal method to make subtree empty.
 */
template <class Comparable>
void AvlTree<Comparable>::makeEmpty( AvlNode<Comparable> * & t ) const
{
    if( t != NULL )
    {
        makeEmpty( t->left );
        makeEmpty( t->right );
        delete t;
    }
    t = NULL;
}

/**
 * Internal method to clone subtree.
 */
template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::clone( AvlNode<Comparable> * t ) const
{
    if( t == NULL )
        return NULL;
    else
        return new AvlNode<Comparable>( t->element, clone( t->left ),
                                      clone( t->right ), t->height );
}

/**
 * Return the height of node t, or -1, if NULL.
 */
template <class Comparable>
int AvlTree<Comparable>::height( AvlNode<Comparable> *t ) const
{
    return t == NULL ? -1 : t->height;
}

/**
 * Return maximum of lhs and rhs.
 */
template <class Comparable>
int AvlTree<Comparable>::max( int lhs, int rhs ) const
{
    return lhs > rhs ? lhs : rhs;
}

/**
 * Rotate binary tree node with left child.
 * For AVL trees, this is a single rotation for case 1.
 * Update heights, then set new root.
 */
template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const
{
    AvlNode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

/**
 * Rotate binary tree node with right child.
 * For AVL trees, this is a single rotation for case 4.
 * Update heights, then set new root.
 */
template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    AvlNode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max( height( k1->left ), height( k1->right ) ) + 1;
    k2->height = max( height( k2->right ), k1->height ) + 1;
    k1 = k2;
}

/**
 * Double rotate binary tree node: first left child.
 * with its right child; then node k3 with new left child.
 * For AVL trees, this is a double rotation for case 2.
 * Update heights, then set new root.
 */
template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const
{
    rotateWithRightChild( k3->left );
    rotateWithLeftChild( k3 );
}

/**
 * Double rotate binary tree node: first right child.
 * with its left child; then node k1 with new right child.
 * For AVL trees, this is a double rotation for case 3.
 * Update heights, then set new root.
 */
template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild( AvlNode<Comparable> * & k1 ) const
{
    rotateWithLeftChild( k1->right );
    rotateWithRightChild( k1 );
}

/**
 * Internal method to print a subtree in sorted order.
 * t points to the node that roots the tree.
 */
template <class Comparable>
void AvlTree<Comparable>::printTree( AvlNode<Comparable> *t ) const
{
    if( t != NULL )
    {
        printTree( t->left );
        cout << t->element << endl;
        printTree( t->right );
    }
}

template <class Comparable>
int AvlTree<Comparable>::countIf( std::function<bool(Comparable)> p ) const {
  return countIf(p, root);
}

template <class Comparable>
int AvlTree<Comparable>::countIf( std::function<bool(Comparable)> p, AvlNode<Comparable> *t ) const {
  if ( NULL == t ) {
    return 0;
  }
  return static_cast<int>(p(t->element)) + countIf(p, t->left) + countIf(p, t->right);
}

template <class Comparable>
std::ostream& AvlTree<Comparable>::printTreeToStream( std::ostream& os ) const {
  return printTreeToStream( os, root );
}

template <class Comparable>
std::ostream& 
AvlTree<Comparable>::printTreeToStream( std::ostream& os, AvlNode<Comparable> *t ) const {
  if ( NULL == t ) {
    return os;
  }
  printTreeToStream(os, t->left);
  os << t->element << "\n";
  printTreeToStream(os,t->right);
  return os;
}

template <class Comparable>
std::list<Comparable> 
AvlTree<Comparable>::collectIntoListIf( std::function<bool (Comparable)> p ) const {
  return collectIntoListIf(p, root);
}

template <class Comparable>
std::list<Comparable> 
AvlTree<Comparable>::collectIntoListIf( std::function<bool (Comparable)> p, AvlNode<Comparable> *t ) const {
  if (NULL == t) {
    std::list<Comparable> k;
    return k;
  } else {
    if (p(t->element)) {
      std::list<Comparable> l = collectIntoListIf(p,t->left);
      l.splice(l.begin(), std::list<Comparable>(1,t->element));
      l.splice(l.begin(), collectIntoListIf(p,t->right));
      return l;
    } else {
      std::list<Comparable> l = collectIntoListIf(p,t->left);
      l.splice(l.begin(), collectIntoListIf(p,t->right));
      return l;
    }
  }
}

template <class Comparable>
std::list<std::reference_wrapper<Comparable> > 
AvlTree<Comparable>::findAllIf( std::function<bool (Comparable)> p, std::function<int (Comparable)> q) const {
  return findAllIf(p,q,root);
}

template <class Comparable>
std::list<std::reference_wrapper<Comparable> > 
AvlTree<Comparable>::findAllIf( std::function<bool (Comparable)> p, std::function<int (Comparable)> q, AvlNode<Comparable> *t ) const {
  if ( NULL == t ) {
    return std::list<std::reference_wrapper<Comparable> >();
  } else {
    int result = q(t->element);
    if (p(t->element)) {
      std::list<std::reference_wrapper<Comparable> > l(1,t->element);
      switch (result) {
      case 1:
        l.splice(l.begin(), findAllIf(p,q,t->right));
        break;
      
      case -1:
        l.splice(l.begin(), findAllIf(p,q,t->left));
        break;

      case 0:
        return l;
      }
      return l;
    } else {
      switch (result) {
        case 1:
          return findAllIf(p,q,t->right);
        
        case -1:
          return findAllIf(p,q,t->left);

        default:
          return std::list<std::reference_wrapper<Comparable> >();;
      }
    }
  }
}
