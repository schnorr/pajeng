#include "TimeInterval.h"

TimeInterval::TimeInterval (void)
{
  frame = NULL;
  selectionStart = -1;
  selectionEnd = -1;
}

TimeInterval::~TimeInterval (void)
{
}

void TimeInterval::setTimeSliceFrame (TimeSliceFrame *f)
{
  frame = f;
}

void TimeInterval::timeLimitsChanged (void)
{

  PajeComponent::timeLimitsChanged ();
}

void TimeInterval::timeSelectionChanged (void)
{

  PajeComponent::timeSelectionChanged ();
}

double TimeInterval::selectionStartTime (void)
{
  if (selectionStart >= 0){
    return selectionStart;
  }else{
    return PajeComponent::selectionStartTime();
  }
}

double TimeInterval::selectionEndTime (void)
{
  if (selectionEnd >= 0){
    return selectionEnd;
  }else{
    return PajeComponent::selectionEndTime();
  }
}

void TimeInterval::setTimeInterval (double start, double end)
{
  if (end > endTime()){
    end = endTime();
  }

  selectionStart = start;
  selectionEnd = end;

  //tell GUI
  wxCommandEvent event (TimeIntervalUpdated);
  wxPostEvent (frame, event);

  //tell components
  timeSelectionChanged ();
}
