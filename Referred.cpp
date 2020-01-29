#include <time.h>
#include <sstream>

#include "Referred.h"

using std::string;
using std::istringstream;

bool compareByCode(Referred i, Referred j){
    return i.code < j.code;
}

const int Referred::getDays_sinceDate() const{
    struct tm sD = {0};

    sD.tm_mday = getSinceDate_Day();
    sD.tm_mon = getSinceDate_Month() - 1;
    sD.tm_year = getSinceDate_Year() - 1900;

    time_t today = time(NULL);

    double seconds = difftime(today,mktime(&sD));

    return static_cast<int>(seconds/60/60/24);
}

const string Referred::getData_toSave() const{
    std::ostringstream temp;

    temp << code << ',' << sinceDate << ',' << sinceDateTotal << ',' << lastClickDate << ',' << cost << ',' << recycleCost << ',' << totalCost << ',' << actualViews << ',' << previousViews << ',' << color
         << std::endl << calendar.getData_toSave();

    return temp.str();
}

const string Referred::getData_toSave_list() const{
    std::ostringstream temp;

    temp << code << ',' << sinceDate << ',' << lastClickDate << ',' << actualViews;

    return temp.str();
}

const string Referred::getData_toShow() const{
    std::ostringstream temp;

    temp << code <<
        ',' << getSinceDate_Day() << '/' << getSinceDate_Month() << '/' << getSinceDate_Year() <<
        ',' << getDays_sinceDate() <<
        ',' << getSinceDateTotal_Day() << '/' << getSinceDateTotal_Month() << '/' << getSinceDateTotal_Year() <<
        ',' << cost <<
        ',' << recycleCost <<
        ',' << totalCost <<
        ',' << previousViews <<
        ',' << actualViews <<
        ',' << totalViews <<
        ',' << media <<
        ',' << ingress <<
        ',' << result <<
        ',' << color;

    return temp.str();
}

void Referred::mergeReferred(Referred &ref, const double c){
    if(sinceDateTotal == "")
        sinceDateTotal = sinceDate;

    if(cost == 0.)
        cost = c;

    if(calendar.ifNewPeriod()){
        calendar.reset();
        recycleCost = 0;
        color = 0;
    }

    totalCost = cost + recycleCost;

    lastClickDate = ref.lastClickDate;

    if(lastClickDate == "20011231")
        calendar.updateDay_daysSincePD(0);
    else
        calendar.updateDay(ref.actualViews - actualViews, lastClickDate);

    if(calendar.getdaysSincePD() == 0)
        previousViews = 0;

    actualViews = ref.actualViews;
    totalViews = previousViews + actualViews;

    if(getDays_sinceDate() != 0)
        media = static_cast<double>(totalViews) / getDays_sinceDate();
    else
        media = -1.;
    ingress = static_cast<double>(totalViews) / 200;
    result = ingress - totalCost;
}

void Referred::recycleWith(Referred &ref, double c = 0., unsigned int co = 0){
    code = ref.code;
    sinceDate = ref.sinceDate;
    previousViews += actualViews;
    recycleCost += c;
    color = co;
}
