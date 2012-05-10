#include "timesliceframe.h"

IMPLEMENT_DYNAMIC_CLASS(TimeSliceFrame,wxPanel);

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

  wxBoxSizer *start_box = new wxBoxSizer (wxHORIZONTAL);
  wxSlider *start_slider = new wxSlider (this, -1, 0, 0, 100);
  wxStaticText *start_text = new wxStaticText (this, -1, _("Start:"));
  wxTextCtrl *start_val = new wxTextCtrl (this, -1);
  start_box->Add (start_text);
  start_box->Add (start_slider, 1, wxEXPAND);
  start_box->Add (start_val);

  wxBoxSizer *size_box = new wxBoxSizer (wxHORIZONTAL);
  wxStaticText *size_text = new wxStaticText (this, -1, _("Size:"));
  wxSlider *size_slider = new wxSlider (this, -1, 100, 0, 100);
  wxTextCtrl *size_val = new wxTextCtrl (this, -1);
  size_box->Add (size_text, 0, wxEXPAND);
  size_box->Add (size_slider, 1, wxEXPAND);
  size_box->Add (size_val, 0, wxEXPAND);

  wxBoxSizer *vbox = new wxBoxSizer (wxVERTICAL);
  vbox->Add (start_box, 0, wxEXPAND);
  vbox->Add (size_box, 0, wxEXPAND);
  SetSizer (vbox);
  return true;
}

void TimeSliceFrame::setVivaGraph (VivaGraph *vivagraph)
{
  this->vivagraph = vivagraph;
  // vivagraph->setTimeSliceFrame (this);
}
