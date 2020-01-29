#include <iostream>
#include <string>
#include <locale.h>

#include "ReferredList.h"
#include "GTKNeoBuxTable.h"

#include <gtkmm.h>

using std::cout;
using std::cin;

//using namespace Gtk;

//Other Compiler Options: `pkg-config gtkmm-3.0 --cflags`
//Other Linker Options: `pkg-config gtkmm-3.0 --libs`
//ded

int main(int argc, char *argv[])
{
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.gtkmm.tutorial2.base");

    GTKNeoBuxTable program("referrals.ddb");
    if(!program.loadData())
        return EXIT_FAILURE;

    return app->run(program);
}
