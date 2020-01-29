#ifndef GTKRECYCLEWINDOW_H_INCLUDED
#define GTKRECYCLEWINDOW_H_INCLUDED

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

#include "ReferredList.h"

#include <string>

class GTKRecycleWindow : public Gtk::Window{
    friend bool isNumber(std::string str);

    Gtk::Label *label = nullptr;
    Gtk::Entry *textEntry = nullptr;
    Gtk::Button *button1 = nullptr;

    vector<Referred*> *temp;
    vector<Referred*>::iterator iter;
    ReferredList *list;

    bool closedOnExitButton = false;
public:
    GTKRecycleWindow(vector<Referred*> *t, ReferredList *l, std::string bText = "Aceptar");
    //~GTKRecycleWindow();

    bool closeOnExit() { return closedOnExitButton; }

    void updateRecycled();

    void on_button_click();
    bool on_delete_event(GdkEventAny *any_event);

    std::string get_text() { return textEntry->get_buffer()->get_text(); }
};

#endif // GTKRECYCLEWINDOW_H_INCLUDED
