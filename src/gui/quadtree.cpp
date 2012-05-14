


#include "gui/quadtree.h"

QuadNode::QuadNode(int e) : elem(e), hl(0), hr(0), ll(0), lr(0)
{
    ;
}

QuadNode::QuadNode(QuadNode* a, QuadNode* b, QuadNode* c, QuadNode* d) :
    elem(-1), hl(a), hr(b), ll(c), lr(d) { ; }

QuadNode::~QuadNode()
{
    if(hl)
        delete hl;
    if(hr)
        delete hr;
    if(ll)
        delete ll;
    if(lr)
        delete lr;
}

bool QuadNode::isLeaf()
{
    return elem == -1;
}


QuadTree::QuadTree()
{
    ;
}

QuadTree::~QuadTree()
{
    if(mRoot)
        delete mRoot;
}

void QuadTree::create(Corpus *c, Viewport view)
{
    
}
