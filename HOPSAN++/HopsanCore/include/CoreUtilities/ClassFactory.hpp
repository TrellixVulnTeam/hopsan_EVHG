/*-----------------------------------------------------------------------------
 This source file is part of Hopsan NG

 Copyright (c) 2011 
    Mikael Axin, Robert Braun, Alessandro Dell'Amico, Björn Eriksson,
    Peter Nordin, Karl Pettersson, Petter Krus, Ingo Staack

 This file is provided "as is", with no guarantee or warranty for the
 functionality or reliability of the contents. All contents in this file is
 the original work of the copyright holders at the Division of Fluid and
 Mechatronic Systems (Flumes) at Linköping University. Modifying, using or
 redistributing any part of this file is prohibited without explicit
 permission from the copyright holders.
-----------------------------------------------------------------------------*/

//!
//! @file   ClassFactory.hpp
//! @author <peter.nordin@liu.se>
//! @date   2009-12-26
//!
//! @brief Contains template class ClassFactory for automatic object instantiation through key value
//!
//$Id$

#ifndef CLASFACTORY_HPP_INCLUDED
#define CLASFACTORY_HPP_INCLUDED

#include <map>
#include <vector>

namespace hopsan {

    //! @brief Template class for automatic object instantiation by key-value.
    //!
    //! This code is based on:
    //! http://www.codeproject.com/KB/architecture/SimpleDynCreate.aspx
    //!
    template <typename _Key, typename _Base, typename _Predicator = std::less<_Key> >
    class ClassFactory
    {
    public:
        typedef std::vector< std::pair<_Key, int> > RegStatusVectorT;
        enum RegisterStatusEnumT {RegisteredOk, AllreadyRegistered, NotRegistered};

    protected:
        typedef _Base* (*CreatorFunctionT) (void);
        typedef std::map<_Key, CreatorFunctionT, _Predicator> FactoryMapT;
        typedef std::pair<_Key, CreatorFunctionT> FactoryPairT;

        //! Map where the construction info is stored
        FactoryMapT mFactoryMap;
        //! Error status map
        RegStatusVectorT mRegStatusVector;


    public:
        //! @brief Used to register creator functions
        _Key registerCreatorFunction(_Key idKey, CreatorFunctionT classCreator)
        {
            //std::cout << "Registering: " << idKey << std::endl;
            //std::cout << "BeforeInsert: Size: " << mFactoryMap.size() << std::endl;
            std::pair<typename FactoryMapT::iterator, bool> rc;
            rc = mFactoryMap.insert(FactoryPairT(idKey, classCreator));
            if (!rc.second)
            {
                //std::cout << "Warning! You are trying to register a Key value that already exist. This registration will be ignored, Key: " << idKey << std::endl;
                mRegStatusVector.push_back(std::pair<_Key, int>(idKey, AllreadyRegistered));
            }
            else
            {
                mRegStatusVector.push_back(std::pair<_Key, int>(idKey, RegisteredOk));
            }
            //std::cout << "AfterInsert: Size: " << mFactoryMap.size() << std::endl;
            return idKey;
        }

        //! @brief Reserve keyword by inserting NULL ptr,
        bool reserveKey(_Key idKey)
        {
            std::pair<typename FactoryMapT::iterator, bool> rc;
            rc = mFactoryMap.insert(FactoryPairT(idKey, static_cast<CreatorFunctionT>(0)));
            return rc.second;
        }

        //! @brief Creates an instance based on the key using creator function (if registered)
        _Base* createInstance(_Key idKey)
        {
            //std::cout << "Create: Size: " << mFactoryMap.size() << std::endl;
            typename FactoryMapT::iterator it = mFactoryMap.find(idKey);
            if (it != mFactoryMap.end())
            {
                if (it->second)
                {
                    return it->second();
                }
            }
            mRegStatusVector.push_back(std::pair<_Key, int>(idKey, NotRegistered));
            //std::cout << "Warning key: " << idKey << " not found!" << std::endl;
            return NULL;
        }

        //! @brief Check if the factory has key registerd
        bool hasKey(_Key idKey) const
        {
            if (mFactoryMap.find(idKey) != mFactoryMap.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        //! @brief Return a vector with all registered keys
        std::vector<_Key> getRegisteredKeys() const
        {
            std::vector<_Key> keys;
            keys.reserve(mFactoryMap.size());
            typename FactoryMapT::const_iterator cit;
            for (cit=mFactoryMap.begin(); cit!=mFactoryMap.end(); ++cit)
            {
                keys.push_back(cit->first);
            }
            return keys;
        }

        //! @brief Unregister creator functions for given key
        void unRegisterCreatorFunction(_Key idKey)
        {
            size_t rc;
            rc = mFactoryMap.erase(idKey);
            if (rc > 0)
            {
                //std::cout << "Sucessfully unregistered: " << idKey << std::endl;
                //! @todo Do we need a status message here to ??
            }
            else
            {
                mRegStatusVector.push_back(std::pair<_Key, int>(idKey, NotRegistered));
                //std::cout << "Failed to unregister: " << idKey << std::endl;
            }
        }

        //! @brief Get a copy of the internal error vector, it maps key values agains error codes, error codes come from registration or unregistration
        RegStatusVectorT getRegisterStatus()
        {
            return mRegStatusVector;
        }

        //! @brief Clears the internal error status vector
        void clearRegisterStatus()
        {
            mRegStatusVector.clear();
        }

        //! @brief Clear the entire factory map (unregister everything)
        void clearFactory()
        {
            mFactoryMap.clear();
            mRegStatusVector.clear();
        }
    };
}

#endif // CLASFACTORY_HPP_INCLUDED
