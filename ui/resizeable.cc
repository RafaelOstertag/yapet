// $Id$

#include "resizeable.h"

#include <algorithm>

using namespace GPSUI;

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

