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

#ifndef INCLUDEDDATABASEH
#define INCLUDEDDATABASEH

#include "core/fingerprinter.h"

#include <string>

using namespace std;

//! Interface for a fingerprint storage class (map from int to T)
template<class T> class Database
{
    public:
        //! Load the database from a file (the whole database may not fit in memory…)
        virtual bool load(string filename) = 0;

        //! Write to a file
        virtual bool save(string filename) = 0;

        //! Create a fresh database
        virtual void erase(int nbStates) = 0;

        //! Retrieve the binding of a fingerprint (returns 0 if unbound)
        virtual T get(fingerp fp) = 0;

        //! Set the binding of a fingerprint
        virtual void set(fingerp fp, T data) = 0;
};

#endif

