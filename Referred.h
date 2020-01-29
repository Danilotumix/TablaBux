#ifndef REFERRED_H_INCLUDED
#define REFERRED_H_INCLUDED

#include <string>
#include <sstream>

#include "ClickCalendar.h"

using std::string;

// R387127011,20180105,20180121,28

class Referred{
    friend bool compareByCode(Referred i, Referred j);

    string code;
    string sinceDate;
    string sinceDateTotal;
    string lastClickDate;

    ClickCalendar calendar;

    double cost = 0.;
    double recycleCost = 0.;
    double totalCost = 0.;

    unsigned int actualViews = 0;
    unsigned int previousViews = 0;
    unsigned int totalViews = 0;

    double ingress = 0.;
    double media = 0.;
    double result = 0.;

    unsigned int color = 0;

    const int getSinceDate_Day() const { return std::stoi(sinceDate.substr(6,2)); }
    const int getSinceDate_Month() const { return std::stoi(sinceDate.substr(4,2)); }
    const int getSinceDate_Year() const { return std::stoi(sinceDate.substr(0,4)); }

    const int getSinceDateTotal_Day() const { return std::stoi(sinceDateTotal.substr(6,2)); }
    const int getSinceDateTotal_Month() const { return std::stoi(sinceDateTotal.substr(4,2)); }
    const int getSinceDateTotal_Year() const { return std::stoi(sinceDateTotal.substr(0,4)); }
public:
    Referred(string c, string sD, string sDT, string lCD, double co, double rC, double tC, int aV, int pV, unsigned int col, ClickCalendar cc) : code(c), sinceDate(sD), sinceDateTotal(sDT), lastClickDate(lCD), cost(co), recycleCost(rC), totalCost(tC), actualViews(aV), previousViews(pV), color(col), calendar(cc) {};
    Referred(string c, string sD, string lCD, int aV) : code(c), sinceDate(sD), lastClickDate(lCD), actualViews(aV) {};

    void mergeReferred(Referred &ref, const double cost);
    void recycleWith(Referred &ref, double c, unsigned int co);

    const int getDays_sinceDate() const;

    const string getData_toSave() const;
    const string getData_toShow() const;

    const string getData_toSave_list() const;

    ClickCalendar &getCalendar() { return calendar; }

    const string getCode() const { return code; }
    const string getSinceDate() const { return sinceDate; }

    double getTotalCost() const { return totalCost; }
    double getIngress() const { return ingress; }
    int gettotalViews() const { return totalViews; }

    const unsigned int get_actualViews() const{ return actualViews; }
    void set_actualViews(unsigned int v){ actualViews = v; }

    const unsigned int get_totalViews() const{ return totalViews; }
    void set_totalViews(unsigned int v){ totalViews = v; }
};

bool compareByCode(Referred i, Referred j);

#endif // REFERRED_H_INCLUDED
