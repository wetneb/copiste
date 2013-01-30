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
#include <boost/serialization/map.hpp>

#include <string>
#include <map>
#include <iostream>

using namespace std;

//! Interface for a fingerprint storage class (map from int to T)
template<class T> class MapDb : public Database<T>
{
    public:
        // Create a fresh database
        MapDb();

        //! Load the database from a file
        bool load(string filename);

        //! Write to a file
        bool save(string filename);

        //! Create a fresh database
        void erase(int nbStates);

        //! Retrieve the binding of a fingerprint (returns 0 if unbound)
        T get(fingerp fp);

        //! Set the binding of a fingerprint
        void set(fingerp fp, T data);

    private:
        std::map<fingerp, T> mMap;
        
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
void MapDb<T>::erase(int nbStates)
{

}

//! Retrieve the binding of a fingerprint (returns 0 if unbound)
template<class T>
T MapDb<T>::get(fingerp fp)
{
    return *((T*)0);
}

//! Set the binding of a fingerprint
template<class T>
void MapDb<T>::set(fingerp fp, T data)
{
    ;
}

#endif

