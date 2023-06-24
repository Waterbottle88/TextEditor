#include "callbacks.h"
#include <cstdlib> // exit
#include <string>

#include "EditorWindow.h"
#include "variables.h"

#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Editor.H>

void set_title(Fl_Window* widget)
{
    if(filename[0] == '\0') title.assign("Untitled");
    else 
    {
        char* slash;
        slash = strrchr(filename, '/');
#ifdef WIN32
        if(slash == NULL) slash = strrchr(filename, '\\');
#endif
        if(slash != NULL) title.assign(slash + 1);
        else title.assign(filename); 
    }

    if(changed) title.append(" (modified)");
    widget->label(title.c_str());
}

void load_file(char *newfile, int ipos) 
{
    loading = 1;
    int insert = (ipos != -1);
    changed = insert;
    if (!insert) strcpy(filename, "");
    int result;
    if (!insert) result = textbuffer->loadfile(newfile);
    else result = textbuffer->insertfile(newfile, ipos);
    if (result)
        fl_alert("Error reading from file \'%s\':\n%s.", newfile, strerror(errno));
    else
        if (!insert) strcpy(filename, newfile);
    loading = 0;
    textbuffer->call_modify_callbacks();
}

void save_file(char *newfile) 
{
    if (textbuffer->savefile(newfile))
        fl_alert("Error writing to file \'%s\':\n%s.", newfile, strerror(errno));
    else
        strcpy(filename, newfile);
    changed = 0;
    textbuffer->call_modify_callbacks();
}

int check_save() 
{
    if (!changed) return 1;
 
    int result = fl_choice("The current file has not been saved.\n"
                           "Would you like to save it now?",
                           "Cancel", "Save", "Discard");
 
    if (result == 1) 
    {
        saveCallback(); 
        return !changed;
    }
 
    return (result == 2) ? 1 : 0;
}

void find2Callback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    if(window->search[0] == '\0')
    {
        findCallback(widget, data);
        return;
    }
    
    window->editor->insert_position(0);
    
    int pos = window->editor->insert_position();
    int found = textbuffer->search_forward(pos, window->search, &pos);
    
    if (found) 
    {
        textbuffer->select(pos, pos + strlen(window->search));
        window->editor->insert_position(pos + strlen(window->search));
        window->editor->show_insert_position();
    }
    else fl_alert("No occurrences of \'%s\' found!", window->search);
}

void findCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    const char* value = fl_input("Search String:", window->search);

    if(value != NULL)
    {
        strcpy(window->search, value);
        find2Callback(widget, data);
    }
}

void changedCallback(int, int nInserted, int nDeleted, int, const char*, void* data)
{
    if((nInserted || nDeleted) && !loading) changed = 1;
    EditorWindow* window = (EditorWindow*)data;
    set_title(window);
    if(loading) window->editor->show_insert_position();
}

void replaceCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    window->replace_dlg->show();
}

void replace2Callback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    const char* find = window->replace_find->value();
    const char* replace = window->replace_with->value();
    
    if(find[0] == '\0')
    {
        window->replace_dlg->show();
        return;
    }

    window->replace_dlg->hide();

    window->editor->insert_position(0);

    int pos = window->editor->insert_position();
    int found = textbuffer->search_forward(pos, find, &pos);
    
    if(found)
    {
        textbuffer->select(pos, pos + strlen(find));
        textbuffer->remove_selection();
        textbuffer->insert(pos, replace);
        textbuffer->select(pos, pos + strlen(replace));
        
        window->editor->insert_position(pos + strlen(replace));
        window->editor->show_insert_position();
    }
    else fl_alert("No occurrences of \'%s\' found!", find);
}

void replallCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    const char* find = window->replace_find->value();
    const char* replace = window->replace_with->value();

    find = window->replace_find->value();
    
    if(find[0] == '\0')
    {
        window->replace_dlg->show();
        return;
    }

    window->replace_dlg->hide();

    window->editor->insert_position(0);
    int times = 0;

    for(int found = 1; found;)
    {
        int pos = window->editor->insert_position();
        found = textbuffer->search_forward(pos, find, &pos);

        if(found)
        {
            textbuffer->select(pos, pos + strlen(find));
            textbuffer->remove_selection();
            textbuffer->insert(pos, replace);
            
            window->editor->insert_position(pos + strlen(replace));
            window->editor->show_insert_position();
            times++;
        }
    }

    if(times) fl_message("Replaced %d occurrences.", times);
    else fl_alert("No occurrences of \'%s\' found!", find);
}

void replcanCallback(Fl_Widget* widget, void* data) 
{
    EditorWindow* window = (EditorWindow*)data;
    window->replace_dlg->hide();
}

void exitCallback(Fl_Widget* widget, void* data)
{
    exit(0);
}

void pasteCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    Fl_Text_Editor::kf_paste(0, window->editor);
}

void copyCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    Fl_Text_Editor::kf_copy(0, window->editor);
}

void cutCallback(Fl_Widget* widget, void* data)
{
    EditorWindow* window = (EditorWindow*)data;
    Fl_Text_Editor::kf_cut(0, window->editor);
}

void deleteCallback(Fl_Widget* widget, void* data)
{
    textbuffer->remove_selection();
}

void undoCallback(void)
{
    textbuffer->undo();
}

void saveAsCallback(void) 
{
    char* newfile;
 
    newfile = fl_file_chooser("Save File As?", "*", filename);
    if (newfile != nullptr) save_file(newfile);
}

void saveCallback(void) 
{
    if (filename[0] == '\0') 
    {
        saveAsCallback();
        return;
    }
    else save_file(filename);
}

void openCallback(Fl_Widget* widget, void* data) 
{
    if (!check_save()) return;
 
    char *newfile = fl_file_chooser("Open File?", "*", filename);
    if (newfile != nullptr) load_file(newfile, -1);
}

void newCallback(Fl_Widget* widget, void* data) 
{
    if (!check_save()) return;
 
    filename[0] = '\0';
    textbuffer->select(0, textbuffer->length());
    textbuffer->remove_selection();
    changed = 0;
    textbuffer->call_modify_callbacks();
}
