#ifndef GTKUPDATELISTWINDOW_H_INCLUDED
#define GTKUPDATELISTWINDOW_H_INCLUDED

#include <gtkmm/window.h>
#include <gtkmm/textview.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

#include <string>

class GTKUpdateListWindow : public Gtk::Window{
    Gtk::Label *label = nullptr;
    Gtk::TextView *textEntry = nullptr;
    Gtk::Button *button1 = nullptr;
    Gtk::Button *button2 = nullptr;

    bool closedOnExitButton = false;
    bool lFF = false;
public:
    GTKUpdateListWindow(std::string lText, std::string bText = "Aceptar");

    bool closeOnExit() { return closedOnExitButton; }
    bool loadFromFile() { return lFF; }

    void on_button1_click();
    void on_button2_click();
    bool on_delete_event(GdkEventAny *any_event);

    std::string get_text() { return textEntry->get_buffer()->get_text(); }
};

#endif // GTKUPDATELISTWINDOW_H_INCLUDED
