#include "GTKRecycleWindow.h"

#include <gtkmm/box.h>
#include <gtkmm/messagedialog.h>

#include <sstream>
#include <string>

GTKRecycleWindow::GTKRecycleWindow(vector<Referred*> *t, ReferredList *l, std::string bText) : temp(t), list(l), iter(t->begin()){
    Gtk::Box *vBox = nullptr;
    vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 10));

    label = Gtk::manage(new Gtk::Label());
    textEntry = Gtk::manage(new Gtk::Entry());
    button1 = Gtk::manage(new Gtk::Button());

    textEntry->signal_activate().connect(sigc::mem_fun(*this, &GTKRecycleWindow::on_button_click));

    button1->set_label(bText);
    button1->signal_clicked().connect(sigc::mem_fun(*this, &GTKRecycleWindow::on_button_click));

    vBox->pack_start(*label);
    vBox->pack_start(*textEntry);
    vBox->pack_start(*button1);

    add(*vBox);

    std::ostringstream stream;

    stream << (temp->size() > 1 ? "Detectados " : "Detectado ") << temp->size()
         << (temp->size() > 1 ? " referidos reciclados." : " referido reciclado.") << "\n\n";

    Gtk::MessageDialog diag(*this, stream.str(), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK);
    diag.set_title("Info");
    diag.run();

    stream.str(std::string());
    stream.clear();

    stream << "Escriba el c\u00F3digo del referido con el que se reemplazar\u00E1 al referido de c\u00F3digo: " << (*iter)->getCode();

    label->set_text(stream.str());

    vBox->show_all();
}

void GTKRecycleWindow::on_button_click(){
    if(get_text() != ""){
        Referred *tr;

        tr = list->getReferredBy_Code(get_text());
        textEntry->set_text("");
        if(!tr){
            Gtk::MessageDialog diag(*this, "C\u00F3digo inv\u00E1lido o no encontrado", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK);
            diag.set_title("\u00A1Error!");
            diag.run();
        }
        else{
            Gtk::MessageDialog diag(*this, "\u00BFEl referido ha sido reciclado manualmente?", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
            diag.set_title("Referido encontrado");

            if(diag.run() == Gtk::RESPONSE_YES)
                (*iter)->recycleWith(*tr, 0.07, 1);
            else
                (*iter)->recycleWith(*tr, 0., 2);

            ++iter;
        }
    }

    if(iter == temp->end())
        hide();
    else{
        std::ostringstream stream;

        stream << "Escriba el c\u00F3digo del referido con el que se reemplazar\u00E1 al referido de c\u00F3digo: " << (*iter)->getCode();

        label->set_text(stream.str());
    }
}

bool GTKRecycleWindow::on_delete_event(GdkEventAny *any_event){
    closedOnExitButton = true;
    hide();
}
