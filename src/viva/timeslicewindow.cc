/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
#include "timeslicewindow.h"

TimeSliceWindow::TimeSliceWindow (wxWindow *parent, TimeInterval *filter)
  : wxFrame(parent, wxID_ANY, wxT("Time Slice Window"), wxDefaultPosition, wxSize(700,450))
{
  wxPanel *panel = new wxPanel(this, -1);
  TimeSliceFrame *timesliceframe = new TimeSliceFrame (panel, -1);
  timesliceframe->setTimeIntervalFilter (filter);

  wxBoxSizer *vbox = new wxBoxSizer (wxVERTICAL);
  vbox->Add (timesliceframe, 0, wxEXPAND);
  panel->SetSizer(vbox);
  Centre();
}

TimeSliceWindow::~TimeSliceWindow ()
{
}
