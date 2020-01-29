#include <sstream>

#include "GTKTotalLabel.h"

GTKTotalLabel::GTKTotalLabel(std::string n, double v, std::string extra, std::string color) : GTKTotalLabel(n, "", extra){
    std::ostringstream temp;
    temp << v;

    value->set_markup("<span foreground=\"" + color + "\" size=\"large\">" + extra + temp.str() + "</span>");
}

GTKTotalLabel::GTKTotalLabel(std::string n, std::string v, std::string extra, std::string color){
    name = Gtk::manage(new Gtk::Label);
    value = Gtk::manage(new Gtk::Label);
/*
    set_column_homogeneous(true);
    set_row_homogeneous(true);
*/
    set_valign(Gtk::ALIGN_CENTER);
    set_halign(Gtk::ALIGN_CENTER);

    value->set_markup("<span foreground=\"" + color + "\" size=\"large\">" + extra + v + "</span>");
    name->set_markup("<span foreground=\"black\" size=\"large\">" + n + "</span>");

    attach(*name, 0, 0, 1, 1);
    attach(*value, 1, 0, 1, 1);
}
