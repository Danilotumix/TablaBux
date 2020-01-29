#include "GTKDayClickWindow.h"

#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>

#include "Referred.h"

#include <cctype>

bool isIntegrer(std::string str){
    if(str.size() == 0)
        return false;

    for(char &a : str){
        if(!isdigit(a))
            return false;
    }

    return true;
}

GTKDayClickWindow::GTKDayClickWindow(std::string lText, ClickCalendar &calendar, int i, GTKCalendar *cal, std::string b1Text, std::string b2Text) : calendar(calendar), id(i){
    Gtk::Box *vBox = nullptr;
    Gtk::Box *hBox = nullptr;

    vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
    hBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));

    label = Gtk::manage(new Gtk::Label(lText));
    textEntry = Gtk::manage(new Gtk::Entry());
    button1 = Gtk::manage(new Gtk::Button());
    button2 = Gtk::manage(new Gtk::Button());

    button1->set_label(b1Text);
    button1->signal_clicked().connect(sigc::bind<GTKCalendar*>(sigc::mem_fun(*this, &GTKDayClickWindow::on_button1_click),cal));

    button2->set_label(b2Text);
    button2->signal_clicked().connect(sigc::mem_fun(*this, &GTKDayClickWindow::on_button2_click));

    textEntry->signal_activate().connect(sigc::bind<GTKCalendar*>(sigc::mem_fun(*this, &GTKDayClickWindow::on_button1_click),cal));

    vBox->pack_start(*label);
    vBox->pack_start(*textEntry);

    hBox->pack_start(*button1);
    hBox->pack_start(*button2);

    vBox->pack_start(*hBox);

    add(*vBox);

    vBox->show_all();
}

void GTKDayClickWindow::on_button1_click(GTKCalendar *cal){
    if(isIntegrer(get_text())){
        int temp = 0;

        temp = calendar.getDays()[id].clicks;
        calendar.getDays()[id].clicks = std::stoi(get_text());

        if(calendar.getSumClicks() > calendar.getParent()->gettotalViews()){
            calendar.getDays()[id].clicks = temp;

            Gtk::MessageDialog diag(*this, "La suma del total de clicks no debe ser mayor a la cantidad de vistas totales", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
            diag.set_title("\u00A1Error!");
            diag.run();
        }
        else{
            if(calendar.getDays()[id].clicks != 0)
                calendar.getDays()[id].color = 1;
            else
                calendar.getDays()[id].color = 0;

            cal->update(nullptr);
            hide();
        }
    }
    else{
        Gtk::MessageDialog diag(*this, "El valor ingresado no es un n\u00FAmero entero positivo", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        diag.set_title("\u00A1Error!");
        diag.run();
    }
}


void GTKDayClickWindow::on_button2_click(){
    hide();
}
