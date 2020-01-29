#ifndef GTKCALENDAR_H_INCLUDED
#define GTKCALENDAR_H_INCLUDED

#include <vector>


#include <gtkmm/paned.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/box.h>

#include "ClickCalendar.h"

class GTKCalendar : public Gtk::Box{
    std::vector<Gtk::Paned*> cards;
    std::vector<Gtk::Label*> labels;
    std::vector<Gtk::Button*> buttons;

    ClickCalendar *calendar;

    void on_button_click(unsigned int id);
public:
    GTKCalendar();

    void deactivate_buttons();
    void activate_buttons();

    void update(ClickCalendar *data);
};

#endif // GTKCALENDAR_H_INCLUDED
