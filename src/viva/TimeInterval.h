#ifndef __TIME_INTERVAL_H
#define __TIME_INTERVAL_H
#include <wx/wx.h>
#include "PajeComponent.h"
#include "timesliceframe.h"

class TimeInterval : public PajeComponent
{
private:
  TimeSliceFrame *frame;
  double selectionStart;
  double selectionEnd;

public:
  TimeInterval ();
  ~TimeInterval ();
  void setTimeSliceFrame (TimeSliceFrame *f);

  void timeLimitsChanged (void);
  void timeSelectionChanged (void);

  double selectionStartTime (void);
  double selectionEndTime (void);

  void setTimeInterval (double start, double end);
};

#endif
