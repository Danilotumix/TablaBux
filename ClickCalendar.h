#ifndef CLICKCALENDAR_H_INCLUDED
#define CLICKCALENDAR_H_INCLUDED

#include <vector>
#include <string>
#include <sstream>

class Referred;

class ClickCalendar{
public:
    struct Day{
        unsigned int clicks = 0;
        int color = -1;
    };
private:
    std::vector<Day> days = std::vector<Day>(30);
    static std::string periodDate;
    static std::string prevPD;

    static int daysSincePD;

    static bool newPeriodReached;

    Referred *parentref = nullptr;

    void get_periodDate();
    void update_daysSincePD();
    void update_periodDate(unsigned int month_offset);
public:
    ClickCalendar();
    ClickCalendar(std::string &tempClicks, std::string &tempColor);

    const std::string getData_toSave() const;
    std::vector<Day> &getDays()  { return days; };

    void updateDay(const int clicks, std::string date);
    void updateDay_firstDay(const int clicks);
    void updateDay_daysSincePD(const int clicks);

    int getSumClicks() { int sum = 0; for(Day d : days) sum += d.clicks; return sum; }
    Referred *getParent() { return parentref; }

    void setParentRef(Referred *p){ parentref = p; }

    void reset();

    const static std::string getPrevPD();

    const static bool ifNewPeriod(){ return newPeriodReached; }

    const static std::string getPeriodDate(){ return periodDate; }
    const static int getdaysSincePD(){ return daysSincePD; }

    void savePeriodDate();
};

#endif // CLICKCALENDAR_H_INCLUDED
