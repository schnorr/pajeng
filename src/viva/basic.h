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
#ifndef __BASIC_FRAME_H
#define __BASIC_FRAME_H
#include <wx/wx.h>
#include <gtk/gtk.h>
#include <cairo.h>

class BasicFrame : public wxPanel
{
  DECLARE_DYNAMIC_CLASS(BasicFrame);

public:
  BasicFrame () {;};
  BasicFrame (wxWindow* parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style,
              const wxString& name);
  ~BasicFrame () {;};
  bool Create(wxWindow* parent,
              wxWindowID id,
              const wxPoint& pos,
              const wxSize& size,
              long style,
              const wxString& name);

protected:
  double ratio;
  wxRealPoint translate;
  wxPoint mousePosition;

private:
  void OnMouseMotion (wxMouseEvent& event);
  void OnMouseWheel (wxMouseEvent& event);
  void OnMouseWheelCairo (wxMouseEvent& event);
  void OnSize (wxSizeEvent& event);
};

#endif
