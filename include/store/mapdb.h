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

#ifndef INCLUDEDMAPDBH
#define INCLUDEDMAPDBH

#include "store/database.h"

// Serialization
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

// Stdlib
#include <string>
#include <map>
#include <iostream>

using namespace std;

class GrayComparator
{
    public:
    bool operator()(const fingerp& a, const fingerp& b)
    {
        bool inverted = false;
        int mask = 1 << 31;
        while((mask & a) == (mask & b))
        {
            if(mask & a)
                inverted = !inverted;
            mask >>= 1;
        }
        return (inverted  ? (mask & b) > (mask & a) : (mask & a) < (mask & b));
    }

    static int editDistance(fingerp a, fingerp b)
    {
        int count = 0, mask = 1 << 31;
        while(mask)
        {
            if((mask & a) != (mask & b))
                count++;
        }
        return count;
    }
};


//! Interface for a fingerprint storage class (map from int to T)
template<class T> class MapDb : public Database<T>
{
    public:
        typedef typename std::map<fingerp, T, GrayComparator>::iterator iterator;

        // Create a fresh database
        MapDb();

        ~MapDb();

        //! Load the database from a file
        bool load(string filename);

        //! Write to a file
        bool save(string filename);

        //! Create a fresh database
        void erase();

        //! Get the list of fingerprints below a given edit distance to the target
        std::list<fingerp> neighbors(fingerp target, int distance);

        //! Retrieve the binding of a fingerprint (without bounds checking)
        T get(fingerp fp);

        //! Retrieve the binding of a fingerprint (returns a default element if unbound)
        T get(fingerp fp, T &default_val);

        //! Check if a fingerprint is bound
        bool exists(fingerp p);

        //! Set the binding of a fingerprint
        void set(fingerp fp, T data);

        //! Delete a binding
        void removeBinding(fingerp fp);

        // Debug, delete me
        typename MapDb<T>::iterator d_begin();
        typename MapDb<T>::iterator d_end();
    private:
        std::map<fingerp, T, GrayComparator> mMap;
        
        //! Internal usage : for load and save
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version);
};

// Create a fresh database
template<class T>
MapDb<T>::MapDb()
{
    ;
}

template<class T>
template<class Archive>
void MapDb<T>::serialize(Archive & ar, const unsigned int version)
{
    ar & mMap;
}

//! Load the database from a file
template<class T>
bool MapDb<T>::load(string filename)
{
    std::ifstream ifs(filename.c_str());
    bool success = false;

    if(!ifs.is_open())
        std::cerr << "Unable to open the file \"" << filename
            << "\"" << std::endl;
    else
    {
        try
        {
            boost::archive::text_iarchive ar(ifs);
            ar & *this;
            success = true;
        }
        catch(boost::archive::archive_exception ex)
        {
            std::cerr << "Error while loading the database from the file \""
                << filename << "\" : " << ex.what() << std::endl;
        }
    }
    return success;
}

//! Write to a file
template<class T>
bool MapDb<T>::save(string filename)
{
    std::ofstream ofs(filename.c_str());
    bool success = false;

    if(!ofs.is_open())
        std::cerr << "Unable to write to the file \"" << filename
            << "\"" << std::endl;
    else
    {
        try
        {
            boost::archive::text_oarchive ar(ofs);
            ar & *this;
            success = true;
        }
        catch(boost::archive::archive_exception ex)
        {
            std::cerr << "Error while writing the database to the file \""
                << filename << "\" : " << ex.what() << std::endl;
        }
    }
    return success;
}

//! Create a fresh database
template<class T>
void MapDb<T>::erase()
{
    mMap.clear();
}

template<class T>
std::list<fingerp> MapDb<T>::neighbors(fingerp target, int distance)
{
    typename MapDb<T>::iterator it = mMap.find(target);
    std::list<fingerp> result;

    typename MapDb<T>::iterator itf = it;
    itf++;
    while(itf != mMap.end() &&
         GrayComparator::editDistance(target, itf->first) <= distance)
    {
        result.push_front(itf->first);
        itf++;
    }
    
    typename MapDb<T>::iterator itb = it;
    itb--;
    while(itb != mMap.end() &&
          GrayComparator::editDistance(target, itb->first) <= distance)
    {
        result.push_front(itb->first);
        itb++;
    }

    return result;
}

//! Retrieve the binding of a fingerprint (without bounds checking)
template<class T>
T MapDb<T>::get(fingerp fp)
{
    return mMap.find(fp)->second;
}

//! Retrieve the binding of a fingerprint (returns a default value if unbound)
template<class T>
T MapDb<T>::get(fingerp fp, T &default_val)
{
    typename MapDb<T>::iterator it = mMap.find(fp);
    if(it != mMap.end())
        return it->second;
    return default_val;
}

//! Check if a fingerprint is bound
template<class T>
bool MapDb<T>::exists(fingerp fp)
{
    return mMap.find(fp) != mMap.end();
}

//! Set the binding of a fingerprint
template<class T>
void MapDb<T>::set(fingerp fp, T data)
{
    mMap[fp] = data;
}

template<class T>
typename MapDb<T>::iterator MapDb<T>::d_begin()
{
   return mMap.begin();
}

template<class T>
typename MapDb<T>::iterator MapDb<T>::d_end()
{
    return mMap.end();
}

template<class T>
void MapDb<T>::removeBinding(fingerp fp)
{
    mMap.erase(mMap.find(fp));
}

template<class T>
MapDb<T>::~MapDb()
{
    erase();
}

#endif

