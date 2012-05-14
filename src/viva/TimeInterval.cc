#include "TimeInterval.h"

TimeInterval::TimeInterval (void)
{
  frame = NULL;
}

TimeInterval::~TimeInterval (void)
{
}

void TimeInterval::setTimeSliceFrame (TimeSliceFrame *f)
{
  frame = f;
}

void TimeInterval::setTimeInterval (double start, double end)
{
  if (end > endTime()){
    end = endTime();
  }

  //tell GUI
  wxCommandEvent event (TimeIntervalUpdated);
  wxPostEvent (frame, event);

  //tell previous components
  setSelectionStartEndTime (start, end);
}
