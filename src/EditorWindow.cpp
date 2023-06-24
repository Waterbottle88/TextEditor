#include "EditorWindow.h"
#include "callbacks.h"

EditorWindow::EditorWindow(int width, int height, std::string& title) 
    : Fl_Window(width, height, title.c_str())
{
    replace_dlg = new Fl_Window(300, 105, "Replace");
    
    replace_find = new Fl_Input(80, 10, 210, 25, "Find:");
   
    replace_with = new Fl_Input(80, 40, 210, 25, "Replace:");
    
    replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
    replace_all->callback((Fl_Callback *)replallCallback, this);
  
    replace_next = new Fl_Return_Button(105, 70, 120, 25, "Replace Next");
    replace_next->callback((Fl_Callback *)replace2Callback, this);
   
    replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");
    replace_cancel->callback((Fl_Callback *)replcanCallback, this);
    replace_dlg->end();
    editor = 0;
    *search = (char)0;
}

EditorWindow::~EditorWindow() 
{
    delete replace_dlg;
}
