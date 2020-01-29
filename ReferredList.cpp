#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <gtkmm/dialog.h>

#include <windows.h>

#include "ReferredList.h"
#include "GTKIfNewWindow.h"
#include "GTKUpdateListWindow.h"
#include "GTKRecycleWindow.h"

using std::cout;
using std::cin;
using std::istringstream;

Referred *binarySearch(string code, vector<Referred> &vec){
    if(vec.empty())
        return nullptr;

    vector<Referred>::iterator beg = vec.begin(), end = vec.end(), mid = beg + (end - beg) / 2;

    while(mid != vec.end() && mid != beg && mid->getCode() != code){
        if(code < mid->getCode())
            end = mid;
        else
            beg = mid + 1;

        mid = beg + (end - beg) / 2;
    }

    return (mid->getCode() == code ? &*mid : nullptr);
}

bool ReferredList::checkIfReferredExists(string code, vector<Referred> vec){
    Referred *temp = binarySearch(code, vec);

    if(temp == nullptr)
        return false;

    return true;
}

ReferredList::ReferredList(const string filename){
    if(filename == "referrals.ddb"){
        std::ifstream file(filename);
        readFromDatabase(file);
    }
    else{
        std::ifstream file(filename);
        readFromList(file);
    }
}

void ReferredList::readFromDatabase(std::istream &data){
    string line, line2;

    while(getline(data,line) && line != ""){
        istringstream stream(line);

        getline(stream, line, ',');
        string code = line;

        getline(stream, line, ',');
        string sinceDate = line;

        getline(stream, line, ',');
        string sinceDateTotal = line;

        getline(stream, line, ',');
        string lastClickDate = line;

        getline(stream, line, ',');
        double cost = std::stod(line);

        getline(stream, line, ',');
        double recycleCost = std::stod(line);

        getline(stream, line, ',');
        double totalCost = std::stod(line);

        getline(stream, line, ',');
        int actualViews = std::stoi(line);

        getline(stream, line, ',');
        int previousViews = std::stoi(line);

        getline(stream, line, ',');
        int color = std::stoi(line);

        getline(data,line);
        getline(data,line2);

        referrals.push_back(Referred(code,sinceDate,sinceDateTotal,lastClickDate,cost,recycleCost,totalCost,actualViews,previousViews,color, ClickCalendar(line,line2)));
    }
}

void ReferredList::readFromList(std::istream &data){
    string line;

    while(getline(data,line) && line != ""){
        istringstream stream(line);

        getline(stream, line, ',');
        string code = line;

        getline(stream, line, ',');
        string sinceDate = line;

        getline(stream, line, ',');
        string lastClickDate = line;

        getline(stream, line, ',');
        int actualViews = std::stoi(line);

        referrals.push_back(Referred(code,sinceDate,lastClickDate,actualViews));
    }
}

void ReferredList::sortBy_code(){
    std::sort(referrals.begin(), referrals.end(), compareByCode);
}

Referred *ReferredList::getReferredBy_Code(string code){
    return binarySearch(code, referrals);
}

void ReferredList::restWith(ReferredList list){
    for(Referred &a : list.referrals){
        Referred *temp = getReferredBy_Code(a.getCode());
        if(temp){
            temp->set_actualViews(temp->get_actualViews() - a.get_actualViews());
        }
    }
}

bool ReferredList::updateWithList(Gtk::Window &window){
    GTKUpdateListWindow uList("Introduzca la lista de referidos de NeoBux: ");

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();

    app->run(uList);

    if(uList.closeOnExit())
        return false;

    ReferredList list;

    if(!uList.loadFromFile()){
        std::istringstream data(uList.get_text());
        list.readFromList(data);
    }
    else{
        std::ifstream data("refs.txt");
        list.readFromList(data);
    }

    list.saveDatabase_list("refs.txt");

    sortBy_code();
    list.sortBy_code();

    if(ClickCalendar::ifNewPeriod()){
        CreateDirectory("Saves", NULL);

        saveDatabase("Saves/referrals" + ClickCalendar::getPrevPD() + ".ddb");
    }

    list.restWith(ReferredList("lastrefs.txt"));

    if(!updateWithList_ifRecycled(list, window))
        return false;
    sortBy_code();

    double cost = 0.;
    if(!updateWithList_ifNew(list, cost, window))
        return false;
    sortBy_code();

    for(Referred &r : referrals){
        r.mergeReferred(*list.getReferredBy_Code(r.getCode()), cost);
        r.getCalendar().setParentRef(&r);
    }

    return true;
}

bool ReferredList::updateWithList_ifRecycled(ReferredList list, Gtk::Window &window){
    vector<Referred*> temp;

    for(Referred &r : referrals){
        if(!checkIfReferredExists(r.getCode(), list.referrals))
            temp.push_back(&r);
    }

    /* TODO:
    Excepción cuando la lista nueva de referidos es de menor tamaño que la de la database.
    */

    if(!temp.empty()){
        GTKRecycleWindow rWindow(&temp, &list);

        Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();

        app->run(rWindow);

        if(rWindow.closeOnExit())
            return false;
    }

    return true;
}

bool ReferredList::updateWithList_ifNew(ReferredList list, double &cost, Gtk::Window &window){
    vector<Referred> temp;

    for(Referred &r : list.referrals){
        if(!checkIfReferredExists(r.getCode(), referrals))
            temp.push_back(r);
    }

    if(temp.empty())
        return true;

    for(Referred &r : temp){
        r.set_actualViews(0);
        referrals.push_back(r);
    }

    std::ostringstream stream;

    stream << (temp.size() > 1 ? "Detectados " : "Detectado ") << temp.size()
         << (temp.size() > 1 ? " nuevos elementos. Se han agregado a la base de datos." : " nuevo elemento. Se ha agregado a la base de datos.");

    stream << "\n\nIngrese el costo de los nuevos referidos: ";

    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create();

    GTKIfNewWindow textWindow(stream.str());

    app->run(textWindow);

    if(textWindow.closeOnExit())
        return false;

    cost = std::stod(textWindow.get_text());

    return true;
}

/*
string ReferredList::getReferredData_byCode(string code){
    Referred *temp = getReferredBy_Code(code);
    return (temp != nullptr ? temp->getData_toShow() : "Referido no encontrado.");
}
*/

string ReferredList::getReferredData_byPos(vector<Referred>::size_type pos){
    if(!(pos < getSize()))
       return "Error";


    return referrals[pos].getData_toShow();
}
int ReferredList::getSize(){
    return referrals.size();
}

void ReferredList::saveDatabase(std::string filename){
    ofstream file(filename);

    std::ostringstream temp;
    for(Referred &r : referrals){
        temp << r.getData_toSave() << std::endl;
    }

    file << temp.str();
}

void ReferredList::saveDatabase_list(std::string filename){
    ofstream file(filename);

    std::ostringstream temp;
    for(Referred &r : referrals){
        temp << r.getData_toSave_list() << std::endl;
    }

    file << temp.str();
}

double ReferredList::getExpectative() const{
    return (gettotalIngress()/(ClickCalendar::getdaysSincePD() + 1)*30) - gettotalCost();
}
