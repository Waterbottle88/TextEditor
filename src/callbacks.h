#pragma once

#include <FL/Fl_Widget.H>

void findCallback(Fl_Widget* widget, void* data);
void find2Callback(Fl_Widget* widget, void* data);

void changedCallback(int, int nInserted, int nDeleted, int, const char*, void* data);

void replaceCallback(Fl_Widget* widget, void* data);
void replace2Callback(Fl_Widget* widget, void* data);
void replallCallback(Fl_Widget* widget, void* data);
void replcanCallback(Fl_Widget* widget, void* data);

void exitCallback(Fl_Widget* widget, void* data);
void pasteCallback(Fl_Widget* widget, void* data);

void copyCallback(Fl_Widget* widget, void* data);
void cutCallback(Fl_Widget* widget, void* data);
void deleteCallback(Fl_Widget* widget, void* data);
void undoCallback(void);

void saveAsCallback(void);
void saveCallback(void);
void openCallback(Fl_Widget* widget, void* data);
void newCallback(Fl_Widget* widget, void* data);