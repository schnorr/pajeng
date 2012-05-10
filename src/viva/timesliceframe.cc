#include "timesliceframe.h"

IMPLEMENT_DYNAMIC_CLASS(TimeSliceFrame,wxPanel);

TimeSliceCanvas::TimeSliceCanvas (wxWindow* parent,
                                  wxWindowID id,
                                  VivaGraph *vivagraph)
  :wxPanel(parent, id, wxDefaultPosition, wxSize(-1,50), 0, _("TSCanvas"))
{
  this->vivagraph = vivagraph;
  this->Connect(wxEVT_PAINT,
                wxPaintEventHandler(TimeSliceCanvas::OnPaint));
}

TimeSliceCanvas::~TimeSliceCanvas ()
{
}

void TimeSliceCanvas::OnPaint (wxPaintEvent& event)
{
  wxPaintDC dc(this);
  wxCoord w, h;
  dc.GetSize(&w, &h);
//  wxColour white;
///  white.Set(wxT("#FFFFFF"));
  dc.DrawRectangle(0, 0, w, h);
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

  update_on_change = new wxCheckBox (this, -1, _("Apply automatically"));

  wxBoxSizer *start_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *start_text = new wxStaticText (this, -1, _("Start:"));
  start_slider = new wxSlider (this, -1, 0, 0, 100);
  start_value = new wxTextCtrl (this, -1);
  start_box->Add (start_text);
  start_box->Add (start_slider, 1, wxEXPAND);
  start_box->Add (start_value);

  wxBoxSizer *size_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *size_text = new wxStaticText (this, -1, _("Size:"));
  size_slider = new wxSlider (this, -1, 100, 0, 100);
  size_value = new wxTextCtrl (this, -1);
  size_box->Add (size_text, 0, wxEXPAND);
  size_box->Add (size_slider, 1, wxEXPAND);
  size_box->Add (size_value, 0, wxEXPAND);

  canvas = new TimeSliceCanvas (this, -1, vivagraph);

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
  box2->Add (update_on_change, 0, wxALL, 5);

  
  wxBoxSizer *forward_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *forward_text = new wxStaticText (this, -1, _("Forward:"));
  forward_slider = new wxSlider (this, -1, 0, 0, 100);
  forward_value = new wxTextCtrl (this, -1);
  forward_box->Add (forward_text);
  forward_box->Add (forward_slider, 1, wxEXPAND);
  forward_box->Add (forward_value);

  wxBoxSizer *frequency_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *frequency_text = new wxStaticText (this, -1, _("Frequency:"));
  frequency_slider = new wxSlider (this, -1, 0, 0, 100);
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

  return true;
}

void TimeSliceFrame::setVivaGraph (VivaGraph *vivagraph)
{
  this->vivagraph = vivagraph;
  vivagraph->setTimeSliceFrame (this);
}

void TimeSliceFrame::timeLimitsChanged (void)
{
  if (!vivagraph) return;
  double start = vivagraph->startTime();
  double end = vivagraph->startTime();
  wxString start_str = wxString::Format(wxT("%f"), start);
  wxString end_str = wxString::Format(wxT("%f"), end);
  std::cout << end_str << std::endl;
//  trace_duration->SetLabel (end_str);
}
