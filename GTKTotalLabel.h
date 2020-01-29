#ifndef GTKTOTALLABEL_H_INCLUDED
#define GTKTOTALLABEL_H_INCLUDED

#include <gtkmm/grid.h>
#include <gtkmm/label.h>

#include <string>

class GTKTotalLabel : public Gtk::Grid{
    Gtk::Label* name = nullptr;
    Gtk::Label* value = nullptr;
public:
    GTKTotalLabel(std::string n, std::string v, std::string extra = "", std::string color = "black");
    GTKTotalLabel(std::string n, double v, std::string extra = "", std::string color = "black");

    Gtk::Label *getname(){ return name; }
};

#endif // GTKTOTALLABEL_H_INCLUDED
