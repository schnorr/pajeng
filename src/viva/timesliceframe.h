#ifndef __TIME_SLICE_FRAME_H
#define __TIME_SLICE_FRAME_H
#include <wx/wx.h>

class VivaGraph;

class TimeSliceCanvas : public wxPanel
{
private:
  VivaGraph *vivagraph;
  void OnPaint (wxPaintEvent& event);

public:
  TimeSliceCanvas (wxWindow *parent, wxWindowID id, VivaGraph *vivagraph);
  ~TimeSliceCanvas ();
};

class TimeSliceFrame : public wxPanel
{
  DECLARE_DYNAMIC_CLASS(TimeSliceFrame);

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
  void setVivaGraph (VivaGraph *vivagraph);
  void timeLimitsChanged (void);

private:
  VivaGraph *vivagraph;

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
};

#include "VivaGraph.h"
#endif
