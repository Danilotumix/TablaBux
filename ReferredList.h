#ifndef REFERREDLIST_H_INCLUDED
#define REFERREDLIST_H_INCLUDED

#include <vector>
#include <string>
#include <gtkmm/window.h>

#include "Referred.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;

class ReferredList{
public:
    typedef vector<Referred>::size_type refPos;
private:
    vector<Referred> referrals;
    //unsigned int total;

    void readFromDatabase(std::istream &data);
    void readFromList(std::istream &data);

    void updateWithList_ifRecycled(ReferredList list);
    void updateRecycled(vector<Referred*> temp, ReferredList list);
    double updateWithList_ifNew(ReferredList list);

    bool updateWithList_ifRecycled(ReferredList list, Gtk::Window &window);
    bool updateWithList_ifNew(ReferredList list, double &cost, Gtk::Window &window);
public:
    ReferredList() = default;
    ReferredList(const string filename);

    void updateWithList(ReferredList list);
    bool updateWithList(Gtk::Window &window);
    void saveDatabase(std::string filename);
    void saveDatabase_list(std::string filename);

    void sortBy_code();

    string getReferredData_byPos(vector<Referred>::size_type pos);

    ClickCalendar &getReferredCalendar_byPos(vector<Referred>::size_type pos) { return referrals[pos].getCalendar(); }

    bool checkIfReferredExists(string code, vector<Referred> vec);
    void restWith(ReferredList list);

    int getSize();

    double gettotalCost() const { double sum = 0.; for(const Referred &r : referrals) sum += r.getTotalCost(); return sum; }
    double gettotalIngress() const { double sum = 0.; for(const Referred &r : referrals) sum += r.getIngress(); return sum; }
    int gettotalTViews() const { int sum = 0.; for(const Referred &r : referrals) sum += r.gettotalViews(); return sum; }

    double getExpectative() const;

    Referred *getReferredBy_Code(string code);
};

Referred *binarySearch(string code, vector<Referred> &vec);

#endif // REFERREDLIST_H_INCLUDED
