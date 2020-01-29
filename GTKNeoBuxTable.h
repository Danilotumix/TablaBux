#ifndef GTKNEOBUXTABLE_H_INCLUDED
#define GTKNEOBUXTABLE_H_INCLUDED

#include <string>
#include <vector>

#include <gtkmm.h>
#include "ReferredList.h"
#include "GTKCalendar.h"
#include "GTKTotalLabel.h"

class GTKNeoBuxTable : public Gtk::Window{
    Gtk::TreeView *treeview = nullptr;
    ReferredList refs;
    Gtk::ScrolledWindow *scrolled_window = nullptr;
    Gtk::Grid *grid = nullptr;
    GTKCalendar *calendar = nullptr;

    GTKTotalLabel *tCost = nullptr;
    GTKTotalLabel *tViews = nullptr;
    GTKTotalLabel *tIngress = nullptr;
    GTKTotalLabel *tIngloss = nullptr;
    GTKTotalLabel *tExp = nullptr;

    class ModelColumns : public Gtk::TreeModel::ColumnRecord{
    public:
        ModelColumns(){
            add(code);
            add(refSince);
            add(daysAsRef);
            add(refSinceTotal);
            add(cost);
            add(recycleCost);
            add(totalCost);
            add(previousViews);
            add(actualViews);
            add(totalViews);
            add(media);
            add(ingress);
            add(result);
            add(calendar);
            add(colorNum);
            add(color);
            add(colorElse);
            add(colorElse2);
            add(colorViews);
            add(colorResult);
        }
/*
            "Codigo: " << code <<
            "\nReferido desde: " << getSinceDate_Day() << '/' << getSinceDate_Month() << '/' << getSinceDate_Year() <<
            "\nDias como referido: " << getDays_sinceDate() <<
            "\nReferido desde (TOTAL): " << getSinceDateTotal_Day() << '/' << getSinceDateTotal_Month() << '/' << getSinceDateTotal_Year() <<
            "\nInversion inicial: " << cost <<
            "\nCosto por reciclaje: " << recycleCost <<
            "\nCosto total: " << totalCost <<
            "\nVistas previas: " << previousViews <<
            "\nVistas actuales: " << actualViews <<
            "\nVistas totales: " << totalViews <<
            "\nMedia: " << media <<
            "\nIngresos: " << ingress <<
            "\nResultado: " << result <<
            "\nColor: " << colorstr;
*/
        Gtk::TreeModelColumn<Glib::ustring> code;
        Gtk::TreeModelColumn<Glib::ustring> refSince;
        Gtk::TreeModelColumn<unsigned int> daysAsRef;
        Gtk::TreeModelColumn<Glib::ustring> refSinceTotal;

        Gtk::TreeModelColumn<Glib::ustring> cost;
        Gtk::TreeModelColumn<Glib::ustring> recycleCost;
        Gtk::TreeModelColumn<Glib::ustring> totalCost;

        Gtk::TreeModelColumn<unsigned int> previousViews;
        Gtk::TreeModelColumn<unsigned int> actualViews;
        Gtk::TreeModelColumn<unsigned int> totalViews;
        Gtk::TreeModelColumn<double> media;

        Gtk::TreeModelColumn<Glib::ustring> ingress;
        Gtk::TreeModelColumn<Glib::ustring> result;

        Gtk::TreeModelColumn<ClickCalendar*> calendar;
        Gtk::TreeModelColumn<unsigned int> colorNum;
        Gtk::TreeModelColumn<Gdk::RGBA> color;
        Gtk::TreeModelColumn<Gdk::RGBA> colorElse;
        Gtk::TreeModelColumn<Gdk::RGBA> colorElse2;
        Gtk::TreeModelColumn<Gdk::RGBA> colorViews;
        Gtk::TreeModelColumn<Gdk::RGBA> colorResult;
    };

    ModelColumns columns;
    Glib::RefPtr<Gtk::ListStore> refTreeModel;
    bool isError = false;
protected:
    void on_treeView_click();

    int compareCodes_byColor(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b);
    int compareDates(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b, Gtk::TreeModelColumn<Glib::ustring> column);

    int compareDolars(const Gtk::TreeModel::iterator& a, const Gtk::TreeModel::iterator& b, Gtk::TreeModelColumn<Glib::ustring> column);
public:
    GTKNeoBuxTable(std::string filename);
    ~GTKNeoBuxTable();
    void fillTreeView(ReferredList list);
    Gtk::TreeView::Column *apprend_column(int idx);

    bool loadData();

    void setColumnsSortable();

    bool on_delete_event(GdkEventAny *any_event);

};

Glib::ustring strDouble_to_dolar(std::string value, unsigned int zeroes, std::string token = "$ ");

#endif // GTKNEOBUXTABLE_H_INCLUDED
