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

#include "gui/viewport.h"
#include "algo/corpus.h"

#include <list>

class QuadTree
{
    public:
        //! Does nothing
        QuadTree();

        //! Deletes the quadtree
        ~QuadTree();

        //! Creates a new quadtree for a Corpus on a given Viewport
        void create(Corpus *c, Viewport view);

        //! Returns the index of the nearest element
        int nearest(int x, int y);

    private:
        //! A node for the quadtree
        class QuadNode;

        class QuadNode
        {
            public:
                //! Constructs a leaf
                QuadNode(int e);
                //! Construct an internal node
                QuadNode(QuadNode* a, QuadNode* b, QuadNode *c, QuadNode *d);

                //! Destructs the node and its children (if any)
                ~QuadNode();

                //! Provided for convenience
                bool isLeaf();

                //! If it's a leaf : the index of the element (else, -1)
                int elem;
                //! Else, it's an internal node : the pointer to the children (else, 0)
                QuadNode* hl, hr, ll, lr;
        };

        //! Root of the tree (0 if none)
        QuadNode* mRoot;

        //! The viewport used to build the tree
        Viewport mViewport;
};

#endif

