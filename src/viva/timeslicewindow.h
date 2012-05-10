#ifndef _TIME_SLICE_WINDOW__H_
#define _TIME_SLICE_WINDOW__H_
#include <wx/wx.h>
#include "timesliceframe.h"
#include "VivaGraph.h"

class VivaGraph;

class TimeSliceWindow : public wxFrame
{
private:
  VivaGraph *vivagraph;
/* private: */
/*   wxMenuBar *menubar; */
/*   wxMenu *file; */
/*   wxMenu *edit; */
/*   wxMenu *view; */
/*   wxMenu *help; */

/* protected: */
/*   void OnTimeIntervalMenu (wxCommandEvent& event); */
/*   void OnGraphConfigurationMenu (wxCommandEvent& event); */
/*   void OnUpdateQuality (wxCommandEvent& event); */
/*   void OnQuit(wxCommandEvent& event); */
/*   void OnOpen(wxCommandEvent& event); */
/*   void OnAbout (wxCommandEvent& event); */

public:
  TimeSliceWindow (wxWindow *parent, VivaGraph *vivagraph);
  ~TimeSliceWindow (void);
};


#endif
