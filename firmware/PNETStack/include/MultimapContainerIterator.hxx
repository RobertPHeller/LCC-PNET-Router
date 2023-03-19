// -!- c++ -!- //////////////////////////////////////////////////////////////
//
//  System        : 
//  Module        : 
//  Object Name   : $RCSfile$
//  Revision      : $Revision$
//  Date          : $Date$
//  Author        : $Author$
//  Created By    : Robert Heller
//  Created       : Sun Mar 19 13:07:43 2023
//  Last Modified : <230319.1312>
//
//  Description	
//
//  Notes
//
//  History
//	
/////////////////////////////////////////////////////////////////////////////
//
//    Copyright (C) 2023  Robert Heller D/B/A Deepwoods Software
//			51 Locke Hill Road
//			Wendell, MA 01379-9728
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
// 
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __MULTIMAPCONTAINERITERATOR_HXX
#define __MULTIMAPCONTAINERITERATOR_HXX

#include <unordered_map>

template <class Key, class Entry, class Hash>
class MultimapContainerIterator
{
public:
    MultimapContainerIterator()
    {
        clear_iteration();
    }
    ~MultimapContainerIterator()
    {
    }
    Entry *next_entry()
    {
        if (it_ == last_) return nullptr; 
        Entry *h = &(it_->second);
        ++it_;
        return h;
    }
    void clear_iteration()
    {
    }
    void init_iteration(Key d)
    {
        auto range = registry_.equal_range(d);
        it_ = range.first;
        last_ = range.second;
    }
    void register_handler(const Entry &entry)
    {
    }
    void unregister_handler(const Entry &entry)
    {
    }

#endif // __MULTIMAPCONTAINERITERATOR_HXX

