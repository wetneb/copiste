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

#ifndef INCLUDED_QUADTREEH
#define INCLUDED_QUADTREEH

#include "algo/corpus.h"

#include <list>

//! A node for the quadtree
class QuadNode
{
    public:
        //! Constructs a leaf
        QuadNode(int e = -1);
        //! Construct an internal node
        QuadNode(QuadNode* a, QuadNode* b, QuadNode *c, QuadNode *d);

        //! Destructs the node and its children (if any)
        ~QuadNode();

        //! Provided for convenience
        bool isLeaf();

        //! If it's a leaf : the index of the element (else, -1)
        int elem;
        //! Else, it's an internal node : the pointer to the children (else, 0)
        QuadNode* hl;
        QuadNode* hr;
        QuadNode* ll;
        QuadNode* lr;
};

class QuadTree
{
    public:
        //! Does nothing
        QuadTree();

        //! Deletes the quadtree
        ~QuadTree();

        //! A rectangle
        struct rect
        {
            double x;
            double y;
            double w;
            double h;
        };
        typedef struct rect rect;

        //! Creates a new quadtree for a Corpus and a given rectangle
        void create(Corpus *c, rect view);

        //! Returns the index of the nearest element. Returns -1 if nothing is found
        int nearest(double x, double y);


    private:
        //! Filters the points in the list which are on the given rectangle
        std::list<int> separate(std::list<int> l, QuadTree::rect view);

        //! Generates a node for the quadtree
        QuadNode* createNode(std::list<int> points, QuadTree::rect view);

        //! Is this point in this rectangle ?
        bool isInRect(double x, double y, rect r);

        //! Browse the tree
        int nodeSearch(QuadNode *node, rect view, double x, double y);

        //! Root of the tree (0 if none)
        QuadNode* mRoot;

        //! The corpus used to build the tree
        Corpus* mCorpus;

        //! The viewport used to build the tree
        rect mViewport;
};

#endif

