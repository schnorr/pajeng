#ifndef __TIME_SLICE_FRAME_H
#define __TIME_SLICE_FRAME_H
#include <wx/wx.h>

class VivaGraph;

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

private:
  VivaGraph *vivagraph;
};

#include "VivaGraph.h"
#endif
