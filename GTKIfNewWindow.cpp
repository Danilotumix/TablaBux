#include "GTKIfNewWindow.h"

#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>

#include <cctype>

bool isNumber(std::string str){
    if(str.size() == 0)
        return false;

    if(!isdigit(str[0]) || !isdigit(str[str.size() - 1]))
        return false;

    bool isDot = false;

    for(char &a : str){
        if(!isdigit(a))
            if(isDot == false && a == '.')
                isDot = true;
            else
                return false;
    }

    return true;
}

GTKIfNewWindow::GTKIfNewWindow(std::string lText, std::string bText){
    Gtk::Box *vBox = nullptr;
    vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));

    label = Gtk::manage(new Gtk::Label(lText));
    textEntry = Gtk::manage(new Gtk::Entry());
    button1 = Gtk::manage(new Gtk::Button());

    button1->set_label(bText);
    button1->signal_clicked().connect(sigc::mem_fun(*this, &GTKIfNewWindow::on_button_click));

    textEntry->signal_activate().connect(sigc::mem_fun(*this, &GTKIfNewWindow::on_button_click));

    //signal_delete_event().connect(sigc::mem_fun(*this, &GTKIfNewWindow::on_delete));

    vBox->pack_start(*label);
    vBox->pack_start(*textEntry);
    vBox->pack_start(*button1);

    add(*vBox);

    vBox->show_all();
}

void GTKIfNewWindow::on_button_click(){
    if(isNumber(get_text()))
        hide();
    else{
        Gtk::MessageDialog diag(*this, "El valor ingresado no es un n\u00FAmero", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        diag.set_title("\u00A1Error!");
        diag.run();
    }
}

/*
void GTKIfNewWindow::on_delete(){
    closedOnExitButton = true;
    hide();
}
*/
bool GTKIfNewWindow::on_delete_event(GdkEventAny *any_event){
    closedOnExitButton = true;
    hide();
}
/*
GTKIfNewWindow::~GTKIfNewWindow{

}*/
