#include "GTKCalendar.h"
#include "GTKDayClickWindow.h"

#include <gtkmm/grid.h>

#include <sstream>

GTKCalendar::GTKCalendar(){
    Gtk::Grid *grid = Gtk::manage(new Gtk::Grid);

    set_orientation(Gtk::ORIENTATION_VERTICAL);
    set_spacing(10);

    grid->set_column_spacing(5);
    grid->set_row_spacing(5);

    //set_hexpand(true);
    //set_vexpand(true);

    cards = std::vector<Gtk::Paned*>(30);
    labels = std::vector<Gtk::Label*>(30);
    buttons = std::vector<Gtk::Button*>(30);

    Gtk::Label *date = Gtk::manage(new Gtk::Label("Calendario de clicks desde la fecha: " + ClickCalendar::getPeriodDate()));

    pack_start(*date);

    for(unsigned int i = 0; i != 5; ++i)
        for(unsigned int j = 0; j != 6; ++j){
            cards[j + (6*i)] = Gtk::manage(new Gtk::Paned(Gtk::ORIENTATION_VERTICAL));
            labels[j + (6*i)] = Gtk::manage(new Gtk::Label);
            buttons[j + (6*i)] = Gtk::manage(new Gtk::Button);
            buttons[j + (6*i)]->signal_clicked().connect(sigc::bind<unsigned int>(sigc::mem_fun(*this, &on_button_click), (j + (6*i))));

            Glib::ustring str("D\u00EDa: ");
            std::ostringstream temp;
            temp << j + (6*i) + 1;

            str += temp.str();

            labels[j + (6*i)]->set_text(str);
            //buttons[j + (6*i)]->set_label("-");

            cards[j + (6*i)]->pack1(*labels[j + (6*i)]);
            cards[j + (6*i)]->pack2(*buttons[j + (6*i)]);

            grid->attach(*cards[j + (6*i)], j, i, 1, 1);
    }

    pack_start(*grid);
}

void GTKCalendar::update(ClickCalendar *data){
    if(data != nullptr)
        calendar = data;

    for(std::vector<ClickCalendar::Day>::size_type i = 0; i != 30; ++i){
        if(calendar->getDays()[i].color == -1){
            static_cast<Gtk::Button*>(cards[i]->get_child2())->set_label("\u00D7");
            static_cast<Gtk::Button*>(cards[i]->get_child2())->unset_color();
            continue;
        }

        std::ostringstream temp;
        temp << calendar->getDays()[i].clicks;

        static_cast<Gtk::Button*>(cards[i]->get_child2())->set_label(temp.str());

        if(calendar->getDays()[i].color == 1)
            static_cast<Gtk::Button*>(cards[i]->get_child2())->override_color(Gdk::RGBA("#544ddf"));
        else
            static_cast<Gtk::Button*>(cards[i]->get_child2())->unset_color();
    }
}

void GTKCalendar::on_button_click(unsigned int id){
    GTKDayClickWindow *window = new GTKDayClickWindow("Ingrese el nuevo valor para los clicks del d\u00EDa", *calendar, id, this);
    window->show();
}

void GTKCalendar::deactivate_buttons(){
    for(Gtk::Button *b : buttons)
        b->set_sensitive(false);
}

void GTKCalendar::activate_buttons(){
    for(Gtk::Button *b : buttons)
        b->set_sensitive(true);
}
