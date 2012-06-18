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
#include "timesliceframe.h"
#include <limits>

DEFINE_EVENT_TYPE (TimeIntervalUpdated)

TimeSliceCanvas::TimeSliceCanvas (wxWindow* parent,
                                  wxWindowID id,
                                  TimeInterval *f)
  :wxPanel(parent, id, wxDefaultPosition, wxSize(-1,50), 0, _("TSCanvas"))
{
  this->filter = f;
  this->Connect(wxEVT_PAINT,
                wxPaintEventHandler(TimeSliceCanvas::OnPaint));
  this->Connect(wxEVT_SIZE,
                wxSizeEventHandler(TimeSliceCanvas::OnSize));

}

TimeSliceCanvas::~TimeSliceCanvas ()
{
}

void TimeSliceCanvas::setTimeIntervalFilter (TimeInterval *filter)
{
  this->filter = filter;
}

void TimeSliceCanvas::OnPaint (wxPaintEvent& event)
{
  if (!filter) return;

  wxPaintDC dc(this);
  wxCoord w, h;
  dc.GetSize(&w, &h);
  dc.DrawLine (0, 0, 0, h);
  dc.DrawLine (w-1, 0, w-1, h);

  double start = filter->startTime();
  double end = filter->endTime();

  double ss = filter->selectionStartTime();
  double se = filter->selectionEndTime();

  wxRect sel (ss/(end-start)*w, 0, (se-ss)/(end-start)*w, h);
  dc.DrawRectangle (sel);
}

void TimeSliceCanvas::OnSize (wxSizeEvent& event)
{
  Refresh();
}

TimeSliceFrame::TimeSliceFrame (wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  Create(parent, id, pos, size, style, name);
}

bool TimeSliceFrame::Create(wxWindow* parent,
                        wxWindowID id,
                        const wxPoint & pos,
                        const wxSize & size,
                        long style,
                        const wxString& name)
{
  if ( !wxPanel::Create(parent, id, pos, size, style, name) )
    return false;

  this->filter = NULL;
  int min = 0;
  int max = std::numeric_limits<int>::max();

  // update_on_change = new wxCheckBox (this, -1, _("Apply automatically"));

  wxBoxSizer *start_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *start_text = new wxStaticText (this, -1, _("Start:"));
  start_slider = new wxSlider (this, ID_START_SLIDER, min, min, max);
  start_value = new wxTextCtrl (this, ID_START_TEXT, _(""), wxDefaultPosition, wxSize(100,-1), wxTE_PROCESS_ENTER);
  start_box->Add (start_text);
  start_box->Add (start_slider, 1, wxEXPAND);
  start_box->Add (start_value);

  wxBoxSizer *size_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *size_text = new wxStaticText (this, -1, _("Size:"));
  size_slider = new wxSlider (this, ID_SIZE_SLIDER, max, min, max);
  size_value = new wxTextCtrl (this, ID_SIZE_TEXT, _(""), wxDefaultPosition, wxSize(100,-1), wxTE_PROCESS_ENTER);
  size_box->Add (size_text, 0, wxEXPAND);
  size_box->Add (size_slider, 1, wxEXPAND);
  size_box->Add (size_value, 0, wxEXPAND);

  canvas = new TimeSliceCanvas (this, -1, filter);

  wxBoxSizer *outer_box = new wxBoxSizer (wxVERTICAL);

  wxStaticBoxSizer *box1;
  box1 = new wxStaticBoxSizer (wxHORIZONTAL, this, _("Trace Time"));
  trace_duration = new wxStaticText (this, -1, wxEmptyString);
  box1->Add (trace_duration, 0, wxEXPAND);

  wxStaticBoxSizer *box2;
  box2 = new wxStaticBoxSizer (wxVERTICAL, this, _("Time Slice Configuration"));
  box2->Add (start_box, 0, wxEXPAND | wxALL, 5);
  box2->Add (size_box, 0, wxEXPAND | wxALL, 5);
  box2->Add (canvas, 0, wxEXPAND | wxALL, 5);
  // box2->Add (update_on_change, 0, wxALL, 5);
  
  wxBoxSizer *forward_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *forward_text = new wxStaticText (this, -1, _("Forward:"));
  forward_slider = new wxSlider (this, ID_FORWARD_SLIDER, 0, 0, 100);
  forward_value = new wxTextCtrl (this, -1);
  forward_box->Add (forward_text);
  forward_box->Add (forward_slider, 1, wxEXPAND);
  forward_box->Add (forward_value);

  wxBoxSizer *frequency_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *frequency_text = new wxStaticText (this, -1, _("Frequency:"));
  frequency_slider = new wxSlider (this, ID_FREQUENCY_SLIDER, 0, 0, 100);
  frequency_value = new wxTextCtrl (this, -1);
  frequency_box->Add (frequency_text);
  frequency_box->Add (frequency_slider, 1, wxEXPAND);
  frequency_box->Add (frequency_value);

  wxStaticBoxSizer *box3;
  box3 = new wxStaticBoxSizer (wxVERTICAL, this, _("Time Slice Animation"));
  box3->Add (forward_box, 0, wxEXPAND | wxALL, 5);
  box3->Add (frequency_box, 0, wxEXPAND | wxALL, 5);

  outer_box->Add (box1, 0, wxEXPAND | wxALL, 10);
  outer_box->Add (box2, 0, wxEXPAND | wxALL, 10);
  outer_box->Add (box3, 0, wxEXPAND | wxALL, 10);
  SetSizer (outer_box);

  this->Connect (wxEVT_SCROLL_THUMBTRACK,
                 wxScrollEventHandler(TimeSliceFrame::OnSliceScroll));
  this->Connect(wxEVT_COMMAND_TEXT_ENTER,
                wxCommandEventHandler(TimeSliceFrame::OnTextualSliceUpdate));
  this->Connect (TimeIntervalUpdated,
                 wxCommandEventHandler(TimeSliceFrame::OnTimeIntervalUpdated));
  return true;
}

void TimeSliceFrame::OnTextualSliceUpdate (wxCommandEvent& event)
{
  switch (event.GetId()){
  case ID_START_TEXT:
  {
    double selectionStart;
    if (!start_value->GetValue().ToDouble (&selectionStart)){
      /* error */
    }
    double selectionEnd = filter->selectionEndTime();
    filter->setTimeInterval (selectionStart, selectionEnd);
  }
  break;
  case ID_SIZE_TEXT:
  {
    double selectionStart = filter->selectionStartTime();
    double size;
    if (!size_value->GetValue().ToDouble(&size)){
      /* error */
    }
    double selectionEnd = selectionStart + size;
    filter->setTimeInterval (selectionStart, selectionEnd);
  }
  break;
  default:
    break;
  }
}

void TimeSliceFrame::OnSliceScroll (wxScrollEvent& event)
{
  int position = event.GetPosition();
  int max = std::numeric_limits<int>::max();  
  switch (event.GetId()){
  case ID_START_SLIDER:
  case ID_SIZE_SLIDER:
  {
    double traceEnd = filter->endTime();
    double start_position, end_position;
    start_position = (double)start_slider->GetValue()/(double)max;
    end_position = (double)size_slider->GetValue()/(double)max;
    
    double start = traceEnd * start_position;
    double end = start + (traceEnd) * end_position;

    filter->setTimeInterval (start, end);
  }
  break;
  case ID_FORWARD_SLIDER:
  case ID_FREQUENCY_SLIDER:
  default:
    break;
  }
}

void TimeSliceFrame::OnTimeIntervalUpdated (wxCommandEvent& event)
{
  Refresh();

  double ss = filter->selectionStartTime();
  double se = filter->selectionEndTime();
  wxString sstart_str = wxString::Format(wxT("%f"), ss);
  wxString ssize_str = wxString::Format(wxT("%f"), se-ss);

  //set values
  start_value->SetValue (sstart_str);
  size_value->SetValue (ssize_str);

  double s = filter->startTime();
  double e = filter->endTime();
  wxString duration_str = wxString::Format(wxT("%f"), e-s);

  //set values
  trace_duration->SetLabel (duration_str);

  //update sliders
  int max = std::numeric_limits<int>::max();
  start_slider->SetValue ((ss/e) * max);
  size_slider->SetValue ((se-ss)/e * max);

  
}

void TimeSliceFrame::setTimeIntervalFilter (TimeInterval *v)
{
  this->filter = v;
  canvas->setTimeIntervalFilter (v);
  filter->setTimeSliceFrame (this);
}

void TimeSliceFrame::timeLimitsChanged (void)
{
  if (!filter) return;

}
