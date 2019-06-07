// This function is used for returning a list of all trees
// in the dataset whose name is the same as the one passed to it
// as the first argument.

list<Tree>  AvlTree::findallmatches (const Tree & x, AvlNode *t ) const
{
    if ( NULL == t )
        return list<Tree>();

    if ( islessname(t->tree_obj, x) ) 
        // x is greater than tree in current node
        return findallmatches(x, t->right );

    else if ( islessname(x, t->tree_obj) ) 
        // x is less than tree in current node
        return findallmatches(x, t->left );

    else {
        // x equals tree_obj in current node;
        list<Tree> leftlist, rightlist;
        leftlist = findallmatches(x, t->left );
        leftlist.push_back(t->tree_obj);
        rightlist = findallmatches(x, t->right );
        leftlist.splice(leftlist.end(), rightlist);
        return leftlist;
    }
}
