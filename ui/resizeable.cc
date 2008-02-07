// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "resizeable.h"

#ifdef HAVE_ALGORITHM
# include <algorithm>
#endif

using namespace YAPETUI;

class RemoveByAddr {
    private:
	const Resizeable* ptr;

    public:
	inline RemoveByAddr(const Resizeable* p) : ptr(p) {}
	inline bool operator()(const Resizeable* p) const {
	    if (ptr == p)
		return true;
	    return false;
	}
};

class ResizeIt {
    public:
	inline void operator()(Resizeable* p) const {
	    p->resize();
	}
};

class RefreshIt {
    public:
	inline void operator()(Resizeable* p) const {
	    p->refresh();
	}
};

//
// Static
//
std::list<Resizeable*> Resizeable::resizeable_list = std::list<Resizeable*>();


void
Resizeable::registerResizeable(Resizeable* r) {
    resizeable_list.push_back(r);
}

void
Resizeable::unregisterResizeable(Resizeable* r) {
    std::list<Resizeable*>::iterator it = 
	std::remove_if(resizeable_list.begin(),
		       resizeable_list.end(),
		       RemoveByAddr(r));

    resizeable_list.erase(it, resizeable_list.end());
}

void
Resizeable::resizeAll() {
    std::for_each(resizeable_list.begin(),
		  resizeable_list.end(),
		  ResizeIt());
    refreshAll();
}

void
Resizeable::refreshAll() {
    std::for_each(resizeable_list.begin(),
		  resizeable_list.end(),
		  RefreshIt());
}
   

//
// Non-Static
//
Resizeable::Resizeable() {
    Resizeable::registerResizeable(this);
}

Resizeable::~Resizeable() {
    Resizeable::unregisterResizeable(this);
}

