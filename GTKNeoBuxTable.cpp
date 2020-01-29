#include <vector>
#include <sstream>
#include <string>

#include <iostream>

#include "GTKNeoBuxTable.h"

Glib::ustring strDouble_to_dolar(std::string value, unsigned int zeroes, std::string token){
    unsigned int dotPos = 0;

    for(char &c : value){
        ++dotPos;

        if(c == '.')
            break;
    }

    if(dotPos == value.size()){
        std:string z(zeroes,'0');
        return token + value + "." + z;
    }

    std::string finalStr;

    if(value.size() >= (dotPos + zeroes))
        finalStr = value.substr(0,(dotPos+zeroes));
    else{
        finalStr = value;

        while(finalStr.size() < (dotPos + zeroes))
            finalStr += '0';
    }

    return token + finalStr;
}

// 24.647
// 24.0

GTKNeoBuxTable::GTKNeoBuxTable(std::string filename) : refs(filename){
    set_default_size(1280, 720);
    set_title("TablaBux");
}

bool GTKNeoBuxTable::loadData(){
    if(!refs.updateWithList(*this))
        return false;

    refs.saveDatabase("referrals.ddb");
    refs.saveDatabase("referrals.ddb.bak");

    Gtk::Box *vBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0));
    Gtk::Box *hBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));

    scrolled_window = Gtk::manage(new Gtk::ScrolledWindow());
    treeview = Gtk::manage(new Gtk::TreeView);
    calendar = Gtk::manage(new GTKCalendar);

    tCost = Gtk::manage(new GTKTotalLabel("Inversi\u00F3n total: ", refs.gettotalCost(), "$ "));
    tViews = Gtk::manage(new GTKTotalLabel("Vistas totales: ", refs.gettotalTViews()));
    tIngress = Gtk::manage(new GTKTotalLabel("Total de ingresos: ", refs.gettotalIngress(), "$ "));

    if(refs.gettotalIngress() - refs.gettotalCost() < 0.)
        tIngloss = Gtk::manage(new GTKTotalLabel("Ganacia/P\u00E9rdida total: ", refs.gettotalIngress() - refs.gettotalCost(), "$ ", "red"));
    else if(refs.gettotalIngress() - refs.gettotalCost() > 0.)
        tIngloss = Gtk::manage(new GTKTotalLabel("Ganacia/P\u00E9rdida total: ", refs.gettotalIngress() - refs.gettotalCost(), "$ ", "green"));
    else
        tIngloss = Gtk::manage(new GTKTotalLabel("Ganacia/P\u00E9rdida total: ", refs.gettotalIngress() - refs.gettotalCost(), "$ ", "gray"));

    if(refs.getExpectative() < 0.)
        tExp = Gtk::manage(new GTKTotalLabel("Expectativa de ganancia del mes: ", refs.getExpectative(), "$ ", "red"));
    else if(refs.getExpectative() > 0.)
        tExp = Gtk::manage(new GTKTotalLabel("Expectativa de ganancia del mes: ", refs.getExpectative(), "$ ", "green"));
    else
        tExp = Gtk::manage(new GTKTotalLabel("Expectativa de ganancia del mes: ", refs.getExpectative(), "$ ", "gray"));

    grid = Gtk::manage(new Gtk::Grid);

    add(*vBox);
    vBox->set_homogeneous();
    hBox->set_homogeneous();

    scrolled_window->add(*treeview);
    vBox->set_spacing(10);
    vBox->pack_start(*scrolled_window);
    vBox->pack_start(*hBox, Gtk::PACK_SHRINK, 0);
    hBox->pack_start(*calendar);

    grid->set_row_spacing(10);

    grid->set_halign(Gtk::ALIGN_CENTER);

    grid->set_column_homogeneous(true);

    grid->attach(*tCost,    1, 0, 1, 1);
    grid->attach(*tViews,   1, 1, 1, 1);
    grid->attach(*tIngress, 1, 2, 1, 1);
    grid->attach(*tIngloss, 1, 3, 1, 1);
    grid->attach(*tExp,     1, 4, 1, 1);

    hBox->pack_start(*grid);

    calendar->set_valign(Gtk::ALIGN_CENTER);
    calendar->set_halign(Gtk::ALIGN_CENTER);

    treeview->set_hexpand(true);
    treeview->set_vexpand(true);

    refTreeModel = Gtk::ListStore::create(columns);
    treeview->set_model(refTreeModel);
    treeview->set_activate_on_single_click(true);

    treeview->get_selection()->signal_changed().connect(sigc::mem_fun(*this, &on_treeView_click));

    apprend_column(treeview->append_column("C\u00F3digo", columns.code));
    apprend_column(treeview->append_column("Referido desde", columns.refSince));
    apprend_column(treeview->append_column("D\u00EDas como referido", columns.daysAsRef));
    apprend_column(treeview->append_column("Referido desde (TOTAL)", columns.refSinceTotal));

    apprend_column(treeview->append_column("Costo inicial", columns.cost));
    apprend_column(treeview->append_column("Costo por reciclaje", columns.recycleCost));
    apprend_column(treeview->append_column("Costo total", columns.totalCost));

    apprend_column(treeview->append_column("Vistas previas", columns.previousViews));
    apprend_column(treeview->append_column("Vistas actuales", columns.actualViews));
    apprend_column(treeview->append_column("Vistas totales", columns.totalViews));
    apprend_column(treeview->append_column_numeric("Media", columns.media, "%.3lf"));

    apprend_column(treeview->append_column("Ingresos", columns.ingress));
    apprend_column(treeview->append_column("Resultado", columns.result));

    setColumnsSortable();
    refTreeModel->set_sort_func(columns.refSince, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDates), columns.refSince));
    refTreeModel->set_sort_func(columns.refSinceTotal, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDates), columns.refSinceTotal));

    refTreeModel->set_sort_func(columns.cost, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDolars), columns.cost));
    refTreeModel->set_sort_func(columns.recycleCost, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDolars), columns.recycleCost));
    refTreeModel->set_sort_func(columns.totalCost, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDolars), columns.totalCost));
    refTreeModel->set_sort_func(columns.ingress, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDolars), columns.ingress));
    refTreeModel->set_sort_func(columns.result, sigc::bind<Gtk::TreeModelColumn<Glib::ustring>>(sigc::mem_fun(*this, &compareDolars), columns.result));

    refTreeModel->set_sort_func(columns.code, sigc::mem_fun(*this, &compareCodes_byColor));

    fillTreeView(refs);

    calendar->update(&refs.getReferredCalendar_byPos(0));
    vBox->show_all();

    return true;
}

void GTKNeoBuxTable::fillTreeView(ReferredList list){
    for(ReferredList::refPos i = 0; i<refs.getSize(); ++i){
        Gtk::TreeModel::Row row = *(refTreeModel->append());

        std::istringstream temp(refs.getReferredData_byPos(i));

        row[columns.calendar] = &refs.getReferredCalendar_byPos(i);

        std::string line;

        std::getline(temp, line, ',');
        row[columns.code] = line;
        std::getline(temp, line, ',');
        row[columns.refSince] = line;
        std::getline(temp, line, ',');
        // Días como referido más uno
        row[columns.daysAsRef] = std::stoi(line) + 1;
        std::getline(temp, line, ',');
        row[columns.refSinceTotal] = line;

        std::getline(temp, line, ',');
        row[columns.cost] = strDouble_to_dolar(line,3);

        std::getline(temp, line, ',');
        row[columns.recycleCost] = strDouble_to_dolar(line,3);

        std::getline(temp, line, ',');

        double totalCost = std::stod(line);
        row[columns.totalCost] = strDouble_to_dolar(line,3);

        std::getline(temp, line, ',');
        row[columns.previousViews] = std::stoi(line);
        std::getline(temp, line, ',');
        row[columns.actualViews] = std::stoi(line);
        std::getline(temp, line, ',');

        int tempTV = std::stoi(line);
        row[columns.totalViews] = tempTV;

        std::getline(temp, line, ',');
        row[columns.media] = std::stod(line);
        std::getline(temp, line, ',');
        row[columns.ingress] = strDouble_to_dolar(line,3);
        std::getline(temp, line, ',');

        double result = std::stod(line);
        row[columns.result] = strDouble_to_dolar(line,3);

        std::getline(temp, line, ',');

        unsigned int color = std::stoi(line);
        row[columns.colorNum] = color;

        unsigned int minViews = totalCost / 0.005;

        if(tempTV >= 0 && tempTV < minViews)
            row[columns.colorViews] = Gdk::RGBA("#ffabab");
        else if(tempTV == minViews)
            row[columns.colorViews] = Gdk::RGBA("#ffee7b");
        else
            row[columns.colorViews] = Gdk::RGBA("#ace768");

        switch(color){
        case 0:
            row[columns.color] = Gdk::RGBA("#f2f2f2");
            break;
        case 1:
            row[columns.color] = Gdk::RGBA("#bdd7ee");
            break;
        case 2:
            row[columns.color] = Gdk::RGBA("#ffc5ff");
            break;
        }

        if(result > 0)
            row[columns.colorResult] = Gdk::RGBA("#6ba770");
        else if(result < 0)
            row[columns.colorResult] = Gdk::RGBA("#f26c6e");
        else
            row[columns.colorResult] = Gdk::RGBA("#929292");

        row[columns.colorElse] = Gdk::RGBA("#f2f2f2");
        row[columns.colorElse2] = Gdk::RGBA("#e2e2e2");
    }
}

Gtk::TreeView::Column *GTKNeoBuxTable::apprend_column(int idx){
    Gtk::TreeView::Column *pColumn = treeview->get_column(idx-1);
    if(pColumn){
        Gtk::CellRenderer *pRenderer = pColumn->get_first_cell();

        Gtk::CellRendererText *pTRenderer = static_cast<Gtk::CellRendererText*>(pColumn->get_first_cell());

        pRenderer->set_alignment(0.5,0.5);
        pColumn->set_alignment(0.5);
        pColumn->set_expand(true);

        if(pColumn->get_title() == "C\u00F3digo")
            pColumn->add_attribute(pRenderer->property_cell_background_rgba(), columns.color);
        else if(pColumn->get_title() == "Vistas totales")
            pColumn->add_attribute(pRenderer->property_cell_background_rgba(), columns.colorViews);
        else if(pColumn->get_title() == "Costo inicial" || pColumn->get_title() == "Costo por reciclaje" || pColumn->get_title() == "Costo total" || pColumn->get_title() == "Ingresos")
            pColumn->add_attribute(pRenderer->property_cell_background_rgba(), columns.colorElse2);
        else if(pColumn->get_title() == "Resultado"){
            pColumn->add_attribute(pRenderer->property_cell_background_rgba(), columns.colorElse);
            pColumn->add_attribute(pTRenderer->property_foreground_rgba(), columns.colorResult);
        }
        else
            pColumn->add_attribute(pRenderer->property_cell_background_rgba(), columns.colorElse);

        return pColumn;
    }
    else
        return nullptr;
}

void GTKNeoBuxTable::setColumnsSortable(){
    std::vector<Gtk::TreeViewColumn*> cols = treeview->get_columns();

    for(std::vector<Gtk::TreeViewColumn*>::size_type i = 0; i < cols.size(); ++i){
        cols[i]->set_sort_column(i);
    }
}

int GTKNeoBuxTable::compareDates(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b, Gtk::TreeModelColumn<Glib::ustring> column){
    std::string str = a->get_value(column);

    std::istringstream temp(str);

    std::getline(temp,str,'/');
    int day1 = std::stoi(str);
    std::getline(temp,str,'/');
    int month1 = std::stoi(str);
    std::getline(temp,str,'/');
    int year1 = std::stoi(str);

    temp.str(std::string());
    temp.clear();
    temp.str(b->get_value(column));

    std::getline(temp,str,'/');
    int day2 = std::stoi(str);
    std::getline(temp,str,'/');
    int month2 = std::stoi(str);
    std::getline(temp,str,'/');
    int year2 = std::stoi(str);

    return year1 > year2 ? 1 : year1 < year2 ? -1 : month1 > month2 ? 1 : month1 < month2 ? -1 : day1 > day2 ? 1 : day1 < day2 ? -1 : 0;
}

int GTKNeoBuxTable::compareCodes_byColor(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b){
    if(a->get_value(columns.colorNum) < b->get_value(columns.colorNum))
        return -1;
    else if(a->get_value(columns.colorNum) > b->get_value(columns.colorNum))
        return 1;

    return a->get_value(columns.code) < b->get_value(columns.code) ? -1 :
           a->get_value(columns.code) == b->get_value(columns.code) ? 0 :
           1;
}

int GTKNeoBuxTable::compareDolars(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b, Gtk::TreeModelColumn<Glib::ustring> column){
    std::string num1 = a->get_value(column);
    std::string num2 = b->get_value(column);

    num1 = num1.substr(2,num1.size());
    num2 = num2.substr(2,num2.size());

    double n1 = std::stod(num1);
    double n2 = std::stod(num2);

    return n1 < n2 ? -1 : n1 == n2 ? 0 : 1;
}

void GTKNeoBuxTable::on_treeView_click(){
    calendar->update(treeview->get_selection()->get_selected()->get_value(columns.calendar));
}

bool GTKNeoBuxTable::on_delete_event(GdkEventAny *any_event){
    refs.saveDatabase("referrals.ddb");
    hide();
}


GTKNeoBuxTable::~GTKNeoBuxTable(){
    //refs.saveDatabase("referrals.ddb");
}

/*
void GTKNeoBuxTable::dialog(Glib::ustring msg)
{
    Gtk::MessageDialog dlg(*this, msg, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    dlg.set_title("Gtkmm Tutorial 3");
    dlg.run();
}
*/
