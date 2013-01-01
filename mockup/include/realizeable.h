// -*- mode: c++ -*-
//
// $Id$

#ifndef REALIZEABLE_H
#define REALIZEABLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rectangle.h"

class Realizeable {
    private:
	bool realized;

    protected:
	inline virtual void setRealized(bool _r) { realized = _r; }

    public:
	inline Realizeable() : realized(false) {}
	inline Realizeable(const Realizeable& r) { realized = r.realized; }
	inline virtual ~Realizeable() {}

	inline Realizeable& operator=(const Realizeable& r) {
	    realized = r.realized;
	    return *this;
	}
	    
	virtual void refresh() = 0;
	virtual void resize(const Rectangle<>& r) = 0;
	virtual void realize(const Rectangle<>& r) = 0;
	inline virtual bool isRealized() const { return realized; }
};

#endif // REALIZEABLE_H
