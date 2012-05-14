#ifndef __TIME_INTERVAL_H
#define __TIME_INTERVAL_H
#include <wx/wx.h>
#include "PajeComponent.h"
#include "timesliceframe.h"

class TimeInterval : public PajeComponent
{
private:
  TimeSliceFrame *frame;

public:
  TimeInterval ();
  ~TimeInterval ();
  void setTimeSliceFrame (TimeSliceFrame *f);
  void setTimeInterval (double start, double end);
};

#endif
