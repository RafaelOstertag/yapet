// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _MAINWINDOWHOTKEYS_H
#define _MAINWINDOWHOTKEYS_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>

#include "mainwindow.h"
#include "loadfile.h"
#include "createfile.h"
#include "changepassword.h"

// QUIT
class HotKeyQ : public YACURS::HotKey {
    private:
	MainWindow* ptr;

    public:
	HotKeyQ(MainWindow* p) : HotKey('Q'), ptr(p) {
	    assert(ptr!=0);
	}
	HotKeyQ(const HotKeyQ& hkq): HotKey(hkq), ptr(hkq.ptr) {}
	void action() {
	    ptr->quit();
	}

	HotKey* clone() const {
	    return new HotKeyQ(*this);
	}
};

class HotKeyq : public YACURS::HotKey {
    private:
	MainWindow* ptr;

    public:
	HotKeyq(MainWindow* p) : HotKey('q'), ptr(p) {
	    assert(ptr!=0);
	}
	HotKeyq(const HotKeyq& hkq): HotKey(hkq), ptr(hkq.ptr) {}
	void action() {
	    ptr->quit();
	}

	HotKey* clone() const {
	    return new HotKeyq(*this);
	}
};

// HELP
class HotKeyH : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyH(MainWindow* p) : HotKey('H'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyH(const HotKeyH& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_help();
	}

	HotKey* clone() const {
	    return new HotKeyH(*this);
	}
};

class HotKeyh : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyh(MainWindow* p) : HotKey('h'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyh(const HotKeyh& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_help();
	}

	HotKey* clone() const {
	    return new HotKeyh(*this);
	}
};

// INFO
class HotKeyI : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyI(MainWindow* p) : HotKey('I'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyI(const HotKeyI& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_info();
	}

	HotKey* clone() const {
	    return new HotKeyI(*this);
	}
};

class HotKeyi : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyi(MainWindow* p) : HotKey('i'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyi(const HotKeyi& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_info();
	}

	HotKey* clone() const {
	    return new HotKeyi(*this);
	}
};

// READ FILE
class HotKeyR : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyR(MainWindow* p) : HotKey('R'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyR(const HotKeyR& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // LoadFile does apoptosis
	    (new LoadFile(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeyR(*this);
	}

};

class HotKeyr : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyr(MainWindow* p) : HotKey('r'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyr(const HotKeyr& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // LoadFile does apoptosis
	    (new LoadFile(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeyr(*this);
	}

};

// CREATE FILE
class HotKeyE : public YACURS::HotKey {
    private:
	MainWindow* ptr;

    public:
	HotKeyE(MainWindow* p) : HotKey('E'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyE(const HotKeyE& hkc) : HotKey(hkc), ptr(hkc.ptr) {}

	void action() {
	    // CreateFile does apoptosis
	    (new CreateFile(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeyE(*this);
	}

};

class HotKeye : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeye(MainWindow* p) : HotKey('e'), ptr(p) {
	    assert(p!=0);
	}
	HotKeye(const HotKeye& hkc) : HotKey(hkc), ptr(hkc.ptr) {}

	void action() {
	    // CreateFile does apoptosis
	    (new CreateFile(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeye(*this);
	}

};

// SAVE FILE
class HotKeyS : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyS(MainWindow* p) : HotKey('S'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyS(const HotKeyS& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->save_records();
	}

	HotKey* clone() const {
	    return new HotKeyS(*this);
	}

};

class HotKeys : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeys(MainWindow* p) : HotKey('s'), ptr(p) {
	    assert(p!=0);
	}
	HotKeys(const HotKeys& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->save_records();
	}

	HotKey* clone() const {
	    return new HotKeys(*this);
	}

};

// ADD RECORD
class HotKeyA : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyA(MainWindow* p) : HotKey('A'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyA(const HotKeyA& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_password_record(false);
	}

	HotKey* clone() const {
	    return new HotKeyA(*this);
	}

};

class HotKeya : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeya(MainWindow* p) : HotKey('a'), ptr(p) {
	    assert(p!=0);
	}
	HotKeya(const HotKeya& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->show_password_record(false);
	}

	HotKey* clone() const {
	    return new HotKeya(*this);
	}

};

// SORT ORDER
class HotKeyO : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyO(MainWindow* p) : HotKey('O'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyO(const HotKeyO& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->sort_asc(!ptr->sort_asc());
	}

	HotKey* clone() const {
	    return new HotKeyO(*this);
	}

};

class HotKeyo : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyo(MainWindow* p) : HotKey('o'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyo(const HotKeyo& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->sort_asc(!ptr->sort_asc());
	}

	HotKey* clone() const {
	    return new HotKeyo(*this);
	}

};

// DELETE
class HotKeyD : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyD(MainWindow* p) : HotKey('D'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyD(const HotKeyD& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->delete_selected();
	}

	HotKey* clone() const {
	    return new HotKeyD(*this);
	}
};

class HotKeyd : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyd(MainWindow* p) : HotKey('d'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyd(const HotKeyd& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    ptr->delete_selected();
	}

	HotKey* clone() const {
	    return new HotKeyd(*this);
	}
};

// CHANGE PASSWORD
class HotKeyC : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyC(MainWindow* p) : HotKey('C'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyC(const HotKeyC& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // ChangePassword does apoptosis
	    (new ChangePassword(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeyC(*this);
	}
};

class HotKeyc : public YACURS::HotKey {
    private:
	MainWindow* ptr;
    public:
	HotKeyc(MainWindow* p) : HotKey('c'), ptr(p) {
	    assert(p!=0);
	}
	HotKeyc(const HotKeyc& hkh) : HotKey(hkh), ptr(hkh.ptr) {}

	void action() {
	    // ChangePassword does apoptosis
	    (new ChangePassword(ptr))->run();
	}

	HotKey* clone() const {
	    return new HotKeyc(*this);
	}
};

// LOCK SCREEN
class HotKeyL : public YACURS::HotKey {
    public:
	HotKeyL() : HotKey('L') {
	}
	HotKeyL(const HotKeyL& hkh) : HotKey(hkh) {}

	void action() {
	    // cheat by simulating an alarm event
	    YACURS::EventQueue::submit(YACURS::EVT_SIGALRM);
	}

	HotKey* clone() const {
	    return new HotKeyL(*this);
	}
};

class HotKeyl : public YACURS::HotKey {
    public:
	HotKeyl() : HotKey('l') {
	}
	HotKeyl(const HotKeyl& hkh) : HotKey(hkh) {}

	void action() {
	    // cheat by simulating an alarm event
	    YACURS::EventQueue::submit(YACURS::EVT_SIGALRM);
	}

	HotKey* clone() const {
	    return new HotKeyl(*this);
	}
};

#endif // _MAINWINDOWHOTKEYS_H
