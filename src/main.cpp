#include <string>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu_Item.H>

#include "EditorWindow.h"
#include "callbacks.h"

int changed = 0;
Fl_Text_Buffer* textbuffer = 0;
char filename[256] = "";
std::string title = "Untitled";
int loading = 0;

static void buildMenu(Fl_Menu_Bar* menu, Fl_Window* window)
{
    Fl_Menu_Item menuItems[] = 
    {
        {"&File", 0, 0, 0, FL_SUBMENU},
            {"&New file", 0, newCallback},
            {"&Open file", FL_COMMAND + 'o', openCallback},
            {"&Save file", FL_COMMAND + 's', (Fl_Callback*)saveCallback},
            {"&Save file as", FL_COMMAND + FL_SHIFT + 's', (Fl_Callback*)saveAsCallback, 0, FL_MENU_DIVIDER}, 
            {"&Exit", FL_COMMAND + 'q', exitCallback, 0},
            { 0 },
        {"&Edit", 0, 0, 0, FL_SUBMENU },
            {"&Undo", FL_COMMAND + 'z', (Fl_Callback*)undoCallback, 0, FL_MENU_DIVIDER},
            {"&Cut", FL_COMMAND + 'x', cutCallback, window},
            {"&Copy", FL_COMMAND + 'c', copyCallback, window},
            {"&Paste", FL_COMMAND + 'v', pasteCallback, window},
            {"&Delete", 0, deleteCallback},
            { 0 },
        {"&Search", 0, 0, 0, FL_SUBMENU},
            {"&Find", FL_COMMAND + 'f', findCallback, window},
            {"&Find again", FL_COMMAND + 'g', find2Callback, window},
            {"&Replace", FL_COMMAND + 'r', replaceCallback, window},
            {"&Replace again", FL_COMMAND + 't', replace2Callback, window},
            { 0 },
        { 0 },
    };
    menu->copy(menuItems);
}

Fl_Window* newView()
{
    EditorWindow* window = new EditorWindow(800, 600, title);
    
    window->begin();
    
    window->editor = new Fl_Text_Editor(10, 30, 780, 560);
    window->editor->buffer(textbuffer);
    
    Fl_Menu_Bar* menuBar = new Fl_Menu_Bar(0, 0, 800, 30);
    buildMenu(menuBar, window);

    window->end();
    window->resizable(window->editor);
    window->editor->linenumber_width(60);

    textbuffer->add_modify_callback(changedCallback, window);
    textbuffer->call_modify_callbacks();
    return window;
}

int main()
{
    textbuffer = new Fl_Text_Buffer();
    Fl_Window* window = newView();
    window->show();

    return Fl::run();
}