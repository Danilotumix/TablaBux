#include "GTKUpdateListWindow.h"

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/messagedialog.h>

GTKUpdateListWindow::GTKUpdateListWindow(std::string lText, std::string bText){
    set_default_size(800, 600);

    Gtk::Box *vBox = nullptr, *hBox = nullptr;
    vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));
    hBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 10));

    Gtk::ScrolledWindow *swindow = Gtk::manage(new Gtk::ScrolledWindow());

    label = Gtk::manage(new Gtk::Label(lText));
    textEntry = Gtk::manage(new Gtk::TextView());
    button1 = Gtk::manage(new Gtk::Button());
    button2 = Gtk::manage(new Gtk::Button());

    button1->set_label(bText);
    button1->signal_clicked().connect(sigc::mem_fun(*this, &GTKUpdateListWindow::on_button1_click));

    button2->set_label("Cargar desde refs.txt");
    button2->signal_clicked().connect(sigc::mem_fun(*this, &GTKUpdateListWindow::on_button2_click));

    //textEntry->signal_activate().connect(sigc::mem_fun(*this, &GTKUpdateListWindow::on_button1_click));

    //signal_delete_event().connect(sigc::mem_fun(*this, &GTKUpdateListWindow::on_delete));

    vBox->pack_start(*label, Gtk::PACK_SHRINK, 0);
    swindow->add(*textEntry);
    vBox->pack_start(*swindow);

    hBox->pack_start(*button1);
    hBox->pack_start(*button2);

    vBox->pack_start(*hBox, Gtk::PACK_SHRINK, 0);

    add(*vBox);

    vBox->show_all();
}

void GTKUpdateListWindow::on_button1_click(){
    if(get_text() != "")
        hide();
    else{
        Gtk::MessageDialog diag(*this, "No ha ingresado texto alguno.", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
        diag.set_title("\u00A1Error!");
        diag.run();
    }
}
void GTKUpdateListWindow::on_button2_click(){
    lFF = true;
    hide();
}
/*
void GTKUpdateListWindow::on_delete(){
    closedOnExitButton = true;
    hide();
}
*/
bool GTKUpdateListWindow::on_delete_event(GdkEventAny *any_event){
    closedOnExitButton = true;
    hide();
}
/*
GTKUpdateListWindow::~GTKUpdateListWindow{

}*/
