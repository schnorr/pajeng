#ifndef _TIME_SLICE_WINDOW__H_
#define _TIME_SLICE_WINDOW__H_
#include <wx/wx.h>
#include "timesliceframe.h"
#include "TimeInterval.h"

class TimeInterval;

class TimeSliceWindow : public wxFrame
{
public:
  TimeSliceWindow (wxWindow *parent, TimeInterval *filter);
  ~TimeSliceWindow (void);
};


#endif
