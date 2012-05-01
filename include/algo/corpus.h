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


#ifndef INCLUDEDCORPUSH
#define INCLUDEDCORPUSH

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <QFile>
#include <QDomDocument>

class Corpus;

//! Stores a set of training examples for a neural net
class Corpus
{
    public:
        /// Construction, destruction

        //! Default constructor. Loads a file if specified.
        Corpus(std::string file = "");
        //! Constructor, creating an empty corpus of dimension dim
        Corpus(int dim);
        //! Copy constructor
        Corpus(const Corpus &c, unsigned int keepOnly = -1);
        //! Frees the memory
        ~Corpus();
        //! Erases the corpus and start a new one with the given dimension
        void erase(int dimension = 0);

        /// Input, output

        //! Loads the corpus from an XML file
        bool load(std::string file, bool verbose = false);
        //! Write the corpus to a file
        void write(std::string file);
        //! Writes to the standard output the set of elements
        void display() const;

        /// Accessors

        //! Returns the number of elements (i.e. vectors) contained in the corpus
        unsigned int size() const;
        //! Returns the number of inputs of each training example (i.e. the number of coordinates of each vector)
        unsigned int dimension() const;

        //! Returns the elem pointed by the given index
        float* elem(unsigned int index) const { return mPool[index]; }
        //! Returns the name of the elem pointed by the given index
        std::string name(unsigned int index) const { return mNames[index]; }
        //! Add a sample to the corpus
        void addElem(float* elem, std::string name = "");
        //! Retuns the bounds of the corpus (the vector is : min_1 max_1 min_2 max_2 ... min_n max_n)
        std::vector<float> bounds() const;

    private:
        float** mPool;
        int mSize; // Stores the number of elements stored in the pool
        int mDimension;

        int mPoolSize; // Stores the actual capacity of the pool

        std::vector<std::string> mNames; // Stores the names of the samples
};

#endif
