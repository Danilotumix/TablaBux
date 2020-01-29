#include "ClickCalendar.h"

#include <iostream>
#include <fstream>
#include <time.h>

int ClickCalendar::daysSincePD = 30;
std::string ClickCalendar::periodDate = std::string();
std::string ClickCalendar::prevPD = std::string();
bool ClickCalendar::newPeriodReached = false;

ClickCalendar::ClickCalendar(){
    get_periodDate();
    update_daysSincePD();
}

const std::string ClickCalendar::getPrevPD(){
    if(prevPD.size() == 0)
        return "Error";

    std::istringstream temp(prevPD);

    std::string number;
    std::string date;

    while(std::getline(temp,number,'/')){
        if(number.size() == 1)
            number = "0" + number;

        date+=number;
    }

    return date;
}

ClickCalendar::ClickCalendar(std::string &tempClicks, std::string &tempColor){
    std::istringstream stream_clicks(tempClicks);
    std::istringstream stream_colors(tempColor);

    for(std::vector<Day>::iterator it = days.begin(); it != days.end() && std::getline(stream_clicks,tempClicks,',') && std::getline(stream_colors,tempColor,','); ++it){
        it->clicks = std::stoi(tempClicks);
        it->color = std::stoi(tempColor);
    }

    get_periodDate();
    update_daysSincePD();
}

const std::string ClickCalendar::getData_toSave() const{
    std::ostringstream stream;

    for(const Day &day : days){
        stream << day.clicks << ',';
    }

    stream << std::endl;

    for(const Day &day : days){
        stream << day.color << ',';
    }

    return stream.str();
}

void ClickCalendar::get_periodDate(){
    if(periodDate != "")
        return;

    std::ifstream date("date.cal");

    if(!std::getline(date,periodDate)){
        periodDate = "04/02/2018";
    }
}

void ClickCalendar::update_periodDate(unsigned int month_offset){
    newPeriodReached = true;

    prevPD = periodDate;

    struct tm pD = {0};

    std::istringstream stream(periodDate);

    std::string str;

    std::getline(stream,str,'/');
    pD.tm_mday = stoi(str);
    std::getline(stream,str,'/');
    pD.tm_mon = stoi(str) - 1;
    std::getline(stream,str,'/');
    pD.tm_year = stoi(str) - 1900;

    time_t temp = mktime(&pD);

    temp = mktime(&pD) + month_offset*30*24*60*60;

    tm pT = {0};

    pT = *localtime(&temp);

    std::ostringstream meme;
    meme << pT.tm_mday << '/' << pT.tm_mon + 1 << '/' << pT.tm_year + 1900;

    periodDate = meme.str();

    savePeriodDate();
}

void ClickCalendar::update_daysSincePD(){
    if(daysSincePD != 30)
        return;

    struct tm pD = {0};

    std::istringstream stream(periodDate);

    std::string temp;

    std::getline(stream,temp,'/');
    pD.tm_mday = stoi(temp);
    std::getline(stream,temp,'/');
    pD.tm_mon = stoi(temp) - 1;
    std::getline(stream,temp,'/');
    pD.tm_year = stoi(temp) - 1900;

    time_t today = time(NULL);

    daysSincePD = static_cast<int>(difftime(today,mktime(&pD))/60/60/24);

    if(daysSincePD < 30)
        return;

    update_periodDate(daysSincePD / 30);
    daysSincePD = daysSincePD % 30;
}

void ClickCalendar::updateDay_firstDay(const int clicks){
    days[0].clicks = clicks;

    if(days[0].clicks == 0)
        days[0].color = 0;
    else
        days[0].color = 1;
}

void ClickCalendar::updateDay_daysSincePD(const int clicks){
    days[daysSincePD].clicks += clicks;

    if(days[daysSincePD].clicks == 0)
        days[daysSincePD].color = 0;
    else
        days[daysSincePD].color = 1;
}

void ClickCalendar::reset(){
    days = std::vector<Day>(30);
}

void ClickCalendar::updateDay(const int clicks, std::string date){
    struct tm nD = {0};

    nD.tm_mday = std::stoi(date.substr(6,2));
    nD.tm_mon = std::stoi(date.substr(4,2)) - 1;
    nD.tm_year = std::stoi(date.substr(0,4)) - 1900;

    tm pD = {0};

    std::istringstream stream(periodDate);

    std::string temp;

    std::getline(stream,temp,'/');
    pD.tm_mday = stoi(temp);
    std::getline(stream,temp,'/');
    pD.tm_mon = stoi(temp) - 1;
    std::getline(stream,temp,'/');
    pD.tm_year = stoi(temp) - 1900;

    if(mktime(&nD) < mktime(&pD)){
        updateDay_daysSincePD(0);
        return;
    }

    int daysSPD = static_cast<int>(difftime(mktime(&nD),mktime(&pD))/60/60/24);

    if(daysSPD >= 30)
        daysSPD = daysSPD % 30;

    updateDay_daysSincePD(0);

    days[daysSPD].clicks += clicks;

    if(days[daysSPD].clicks == 0)
        days[daysSPD].color = 0;
    else
        days[daysSPD].color = 1;
}

void ClickCalendar::savePeriodDate(){
    std::ofstream file("date.cal");

    file << periodDate;
}
