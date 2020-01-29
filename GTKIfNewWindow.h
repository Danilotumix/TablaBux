#ifndef GTKIFNEWINDOW_H_INCLUDED
#define GTKIFNEWINDOW_H_INCLUDED

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

#include <string>

class GTKIfNewWindow : public Gtk::Window{
    friend bool isNumber(std::string str);

    Gtk::Label *label = nullptr;
    Gtk::Entry *textEntry = nullptr;
    Gtk::Button *button1 = nullptr;

    bool closedOnExitButton = false;
public:
    GTKIfNewWindow(std::string lText, std::string bText = "Aceptar");
    //~GTKIfNewWindow();

    bool closeOnExit() { return closedOnExitButton; }

    void on_button_click();
    //void on_delete();
    bool on_delete_event(GdkEventAny *any_event);

    std::string get_text() { return textEntry->get_buffer()->get_text(); }
};

#endif // GTKIFNEWINDOW_H_INCLUDED
