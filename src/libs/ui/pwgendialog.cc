// $Id$
//
// Copyright (C) 2008-2011, 2013  Rafael Ostertag
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "globals.h"
#include "intl.h"
#include "pwgendialog.h"

//
// Private
//
std::string PwGenDialog::get_name_of_rng() const {
    switch (passwordGenerator.rngEngine()) {
        case yapet::pwgen::DEVRANDOM:
            return "/dev/random";
        case yapet::pwgen::DEVURANDOM:
            return "/dev/urandom";
        case yapet::pwgen::RAND:
            return "rand()";
    }

    assert(0);
    return "oops!";
}

//
// Protected
//
void PwGenDialog::checkbox_selection_handler(YACURS::Event& _e) {
    assert(_e == YACURS::EVT_CHECKBOX_SELECTION);

    YACURS::EventEx<YACURS::CheckBox*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::CheckBox*>&>(_e);

    if (evt.data() == charpools) {
        YAPET::Globals::config.pwgen_letters.set(
            charpools->selected(_("Letters")));

        YAPET::Globals::config.pwgen_digits.set(
            charpools->selected(_("Digits")));
        YAPET::Globals::config.pwgen_punct.set(
            charpools->selected(_("Punctuation")));
        YAPET::Globals::config.pwgen_special.set(
            charpools->selected(_("Special")));
        YAPET::Globals::config.pwgen_other.set(charpools->selected(_("Other")));

        // make sure at least one character pool is selected, else
        // setNewPool() will throw an exception.
        if (YAPET::Globals::config.character_pools() == 0) {
            YAPET::Globals::config.pwgen_letters.set(true);
            charpools->set_selection(0);
        }

        passwordGenerator.characterPools(
            YAPET::Globals::config.character_pools());
        return;
    }

    if (evt.data() == sources) {
        yapet::pwgen::RNGENGINE rng;

        if (sources->selected("/dev/random")) rng = yapet::pwgen::DEVRANDOM;

        if (sources->selected("/dev/urandom")) rng = yapet::pwgen::DEVURANDOM;

        if (sources->selected("rand()")) rng = yapet::pwgen::RAND;

        YAPET::Globals::config.pwgen_rng.set(rng);
        passwordGenerator.rngEngine(rng);
    }
}

void PwGenDialog::button_press_handler(YACURS::Event& _e) {
    Dialog::button_press_handler(_e);

    YACURS::EventEx<YACURS::Button*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::Button*>&>(_e);

    if (evt.data() == &regenbutton) {
        // Transfer pwlen to configuration (might have changed)

        // Configuration takes care of sanitizing number with respect
        // to min/max value.
        YAPET::Globals::config.pwgenpwlen.set_str(pwlen.input());
        // Transfer length back to input, in case sanitation took
        // place and changed the value
        pwlen.input(YAPET::Globals::config.pwgenpwlen);

        auto password{passwordGenerator.generatePassword(
            YAPET::Globals::config.pwgenpwlen)};
        genpw.input(reinterpret_cast<const char*>(*password));
    }
}

//
// Public
//
PwGenDialog::PwGenDialog()
    : YACURS::Dialog{_("Password Generator"), YACURS::OKCANCEL,
                     YACURS::AUTOMATIC},
      passwordGenerator{YAPET::Globals::config.pwgen_rng,
                        YAPET::Globals::config.character_pools()},
      mainpack{},
      boxespack{},
      genpwlabel{_("Generated password")},
      genpw{},
      pwlenlabel{_("Password length")},
      pwlen{},
      regenbutton_spacer{},
      regenbutton{_("Regenerate")} {
    // coloring
    genpwlabel.color(YACURS::DIALOG);
    pwlenlabel.color(YACURS::DIALOG);

    genpw.max_input(256);
    pwlen.max_input(3);
    pwlen.filter(YACURS::FilterDigit());

    std::vector<std::string> labels;

    // If labels are updated, please make sure
    // PwGenDialog::checkbox_selection_handler() is adjusted
    // accordingly.
    labels.push_back(_("Letters"));
    labels.push_back(_("Digits"));
    labels.push_back(_("Punctuation"));
    labels.push_back(_("Special"));
    labels.push_back(_("Other"));
    charpools = new YACURS::CheckBox(_("Character Pools"), labels);

    if (yapet::pwgen::isLetters(YAPET::Globals::config.character_pools()))
        charpools->set_selection(_("Letters"));
    if (yapet::pwgen::isDigits(YAPET::Globals::config.character_pools()))
        charpools->set_selection(_("Digits"));
    if (yapet::pwgen::isPunct((YAPET::Globals::config.character_pools())))
        charpools->set_selection(_("Punctuation"));
    if (yapet::pwgen::isSpecial(YAPET::Globals::config.character_pools()))
        charpools->set_selection(_("Special"));
    if (yapet::pwgen::isOther(YAPET::Globals::config.character_pools()))
        charpools->set_selection(_("Other"));

    // Those labels must match what is returned by get_name_of_rng()
    // Set only those rngs that are available
    labels.clear();
    labels.push_back("/dev/random");
    labels.push_back("/dev/urandom");
    labels.push_back("rand()");

    sources = new YACURS::RadioBox(_("Sources"), labels);
    sources->set_selection(get_name_of_rng());

    mainpack.add_back(&genpwlabel);
    mainpack.add_back(&genpw);

    mainpack.add_back(&pwlenlabel);
    mainpack.add_back(&pwlen);

    mainpack.add_back(&boxespack);

    boxespack.always_dynamic(true);
    boxespack.add_back(charpools);
    boxespack.add_back(sources);

    widget(&mainpack);

    // Add regenerate button
    add_button(&regenbutton_spacer);
    add_button(&regenbutton);

    // Initialize values
    std::ostringstream conv;
    conv << YAPET::Globals::config.pwgenpwlen.get();
    pwlen.input(conv.str());

    auto password{passwordGenerator.generatePassword(
        YAPET::Globals::config.pwgenpwlen.get())};
    genpw.input(reinterpret_cast<const char*>(*password));

    YACURS::EventQueue::connect_event(
        YACURS::EventConnectorMethod1<PwGenDialog>(
            YACURS::EVT_CHECKBOX_SELECTION, this,
            &PwGenDialog::checkbox_selection_handler));
}

PwGenDialog::~PwGenDialog() {
    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<PwGenDialog>(
            YACURS::EVT_CHECKBOX_SELECTION, this,
            &PwGenDialog::checkbox_selection_handler));

    delete charpools;
    delete sources;
}
