// Timer.cc - source file for the Agoris program
// Copyright (c) 2001  Andreas Bauer <baueran@in.tum.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.


#include "Timer.hh"
extern "C" {
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
}


Timer::Timer() {
  start = 0;
}


double Timer::cpuTime(void) {
  struct rusage ru;  
  getrusage(RUSAGE_SELF, &ru);
  return ru.ru_utime.tv_sec + 1.0e-6*ru.ru_utime.tv_usec;
}


double Timer::timeElapsed(void) {
  return cpuTime() - startTime();
}


void Timer::resetTimer(void) {
  start = cpuTime();
}


double Timer::startTime(void) {
  return start;
}
