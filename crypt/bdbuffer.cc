// $Id$

#include "bdbuffer.h"

using namespace GPSAFE;

uint8_t*
BDBuffer::alloc_mem(size_t s) throw(GPSException) {
    uint8_t* tmp = (uint8_t*) malloc(s);
    if (tmp == NULL)
	throw GPSException("Memory exhausted");

    return tmp;
}

void
BDBuffer::free_mem(uint8_t* d, size_t s) {
    memset(d, 0, s);
    free(d);
}

BDBuffer::BDBuffer(size_t is) throw(GPSException) : _size(is) {
    data = alloc_mem(_size);
}

BDBuffer::BDBuffer() : _size(0), data(NULL) { }

BDBuffer::BDBuffer(const BDBuffer& ed) throw(GPSException) {
    if (ed.data == NULL) {
	data = NULL;
	_size = 0;
	return;
    }

    data = alloc_mem(ed._size);
    memcpy(data, ed.data, ed._size);
    _size = ed._size;
}

BDBuffer::~BDBuffer() {
    if (data == NULL) return;
    free_mem(data, _size);
}

void
BDBuffer::resize(size_t ns) throw(GPSException) {
    if (data == NULL) {
	data = alloc_mem(ns);
	_size = ns;
	return;
    }

    uint8_t* newbuf = alloc_mem(ns);

    if (ns > _size) 
	memcpy(newbuf, data, _size);
    else
	memcpy(newbuf, data, ns);

    free_mem(data, _size);

    _size = ns;
    data = newbuf;
}


uint8_t*
BDBuffer::at(size_t pos) throw(std::out_of_range) {
    if (pos > _size)
	throw std::out_of_range("Position out of range");

    return data + pos;
}

const uint8_t*
BDBuffer::at(size_t pos) const throw(std::out_of_range) {
    if (pos > _size)
	throw std::out_of_range("Position out of range");

    return data + pos;
}


const BDBuffer&
BDBuffer::operator=(const BDBuffer& ed) {
    if (this == &ed) return *this;

    if (data != NULL)
	free_mem(data, _size);

    if (ed.data != NULL) {
	data = alloc_mem(ed._size);
	memcpy(data, ed.data, ed._size);
    } else {
	data = NULL;
    }

    _size = ed._size;
    return *this;
}
