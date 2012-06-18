/*
    This file is part of Viva.

    Viva is free software: you can redistribute it and/or modify it
    under the terms of the GNU Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Viva is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Public License for more details.

    You should have received a copy of the GNU Public License
    along with Viva. If not, see <http://www.gnu.org/licenses/>.
*/
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
