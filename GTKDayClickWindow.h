#ifndef GTKDAYCLICKWINDOW_H_INCLUDED
#define GTKDAYCLICKWINDOW_H_INCLUDED

#include <gtkmm/window.h>
#include <gtkmm/entry.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>

#include <string>

#include "ClickCalendar.h"
#include "GTKCalendar.h"

class GTKDayClickWindow : public Gtk::Window{
    friend bool isIntegrer(std::string str);

    Gtk::Label *label = nullptr;
    Gtk::Entry *textEntry = nullptr;
    Gtk::Button *button1 = nullptr;
    Gtk::Button *button2 = nullptr;

    ClickCalendar &calendar;
    int id;

    void on_button1_click(GTKCalendar *cal);
    void on_button2_click();
public:
    GTKDayClickWindow(std::string lText, ClickCalendar &calendar, int id, GTKCalendar *cal, std::string b1Text = "Aceptar", std::string b2Text = "Cancelar");

    std::string get_text() { return textEntry->get_buffer()->get_text(); }
};

#endif // GTKDAYCLICKWINDOW_H_INCLUDED
