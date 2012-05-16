/*
 * This file is part of Copiste.
 *
 * Copiste is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  Copiste is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Copiste.  If not, see <http://www.gnu.org/licenses/>.
 */

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


QuadTree::QuadTree() : mRoot(0), mCorpus(0)
{
    ;
}

QuadTree::~QuadTree()
{
    if(mRoot)
        delete mRoot;
}

//! Filters the points in the list which are on the given rectangle
std::list<int> QuadTree::separate(std::list<int> l, QuadTree::rect view)
{
    std::list<int> res;
    for(std::list<int>::iterator i = l.begin(); i != l.end(); i++)
    {
        if(*i < (int)mCorpus->size())
        {
            double px = mCorpus->elem(*i)[1];
            double py = mCorpus->elem(*i)[2];

            if(px >= view.x && px <= view.x + view.w &&
               py >= view.y && py <= view.y + view.h)
                res.push_front(*i);
        }
    }
    return res;
}

//! Generates a node for the quadtree
QuadNode* QuadTree::createNode(std::list<int> points, QuadTree::rect view)
{
    std::list<int> visible = separate(points, view);
    QuadNode *result;
    if(visible.empty()) // if the size is 0
        result = 0;
    else if(++visible.begin() == visible.end()) // if the size is 1
        result = new QuadNode(visible.front());
    else
    {
        QuadNode* n = new QuadNode;
        rect r;
        r.w = view.w / 2;
        r.h = view.h / 2;

        // High left
        r.x = view.x;
        r.y = view.y;
        n->hl = createNode(visible, r);

        // High right
        r.x += r.w;
        n->hr = createNode(visible, r);

        //! Low right
        r.y += r.h;
        n->lr = createNode(visible, r);

        //! Low left
        r.x -= r.w;
        n->ll = createNode(visible, r);

        result = n;
    }

    return result;
}

void QuadTree::create(Corpus *c, QuadTree::rect view)
{
    mCorpus = c;

    if(c != 0)
    {
        //! Create the initial list of all elements
        std::list<int> points;
        for(int i = c->size()-1; i >= 0; i--)
            points.push_front(i);

        //! Recurse
        mRoot = createNode(points, view);
    }
}

bool QuadTree::isInRect(double x, double y, rect r)
{
    return (x >= r.x && y >= r.y && x <= r.x + r.w && y <= r.y + r.h);
}

int QuadTree::nodeSearch(QuadNode *node, QuadTree::rect view, double x, double y)
{
    int result = -1;
    if(node != 0)
    {
        if(node->elem != -1 && isInRect(mCorpus->elem(node->elem)[1], mCorpus->elem(node->elem)[2], view))
            result = node->elem;
        else
        {
            rect r;
            r.w = view.w / 2;
            r.h = view.h / 2;

            // High left
            r.x = view.x;
            r.y = view.y;
            if(isInRect(x, y, r))
                result = nodeSearch(node->hl, r, x, y);

            // High right
            r.x += r.w;
            if(isInRect(x, y, r))
                result = nodeSearch(node->hr, r, x, y);

            //! Low right
            r.y += r.h;
            if(isInRect(x, y, r))
                result = nodeSearch(node->lr, r, x, y);

            //! Low left
            r.x -= r.w;
            if(isInRect(x, y, r))
                result = nodeSearch(node->ll, r, x, y);
        }
    }
    return result;   
}

int QuadTree::nearest(double x, double y)
{
    return nodeSearch(mRoot, mViewport, x, y);
}

