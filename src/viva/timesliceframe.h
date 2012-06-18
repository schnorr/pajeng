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
#ifndef __TIME_SLICE_FRAME_H
#define __TIME_SLICE_FRAME_H
#include <wx/wx.h>

class TimeInterval;

class TimeSliceCanvas : public wxPanel
{
private:
  TimeInterval *filter;
  void OnPaint (wxPaintEvent& event);
  void OnSize (wxSizeEvent& event);

public:
  TimeSliceCanvas (wxWindow *parent, wxWindowID id, TimeInterval *filter);
  ~TimeSliceCanvas ();
  void setTimeIntervalFilter (TimeInterval *filter);
};

class TimeSliceFrame : public wxPanel
{

public:
  TimeSliceFrame () {;};
  TimeSliceFrame (wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint & pos = wxDefaultPosition,
                  const wxSize & size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL,
              const wxString & name = _T("TimeSliceFrame"));
  ~TimeSliceFrame () {;};
  bool Create(wxWindow* parent,
              wxWindowID id = wxID_ANY,
              const wxPoint & pos = wxDefaultPosition,
              const wxSize & size = wxDefaultSize,
              long style = wxTAB_TRAVERSAL,
              const wxString & name = _T("TimeSliceFrame"));
  void setTimeIntervalFilter (TimeInterval *filter);
  void timeLimitsChanged (void);

private:
  TimeInterval *filter;

  wxStaticText *trace_duration;

  wxSlider *start_slider;
  wxSlider *size_slider;
  wxTextCtrl *start_value;
  wxTextCtrl *size_value;
  TimeSliceCanvas *canvas;
  wxCheckBox *update_on_change;

  wxSlider *forward_slider;
  wxSlider *frequency_slider;
  wxTextCtrl *forward_value;
  wxTextCtrl *frequency_value;

  void OnTextualSliceUpdate (wxCommandEvent& event);
  void OnSliceScroll (wxScrollEvent& event);
  void OnTimeIntervalUpdated (wxCommandEvent& event);
};

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(TimeIntervalUpdated, -1)
END_DECLARE_EVENT_TYPES()

#define ID_START_SLIDER     100
#define ID_START_TEXT       1000
#define ID_SIZE_SLIDER      101
#define ID_SIZE_TEXT       1001
#define ID_FORWARD_SLIDER   102
#define ID_FREQUENCY_SLIDER 103

#include "TimeInterval.h"
#endif
