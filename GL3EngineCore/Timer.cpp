/*
 * Timer.cpp
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#include "Timer.h"
#include <iostream>

Timer::Timer() {
#ifdef UNIX
  clock_gettime(CLOCK_MONOTONIC, &startTime);
#endif
}

Timer::~Timer() {
  // TODO Auto-generated destructor stub
}

#ifdef UNIX
const float BILLION = 1000000000; // 10^9
const float MILLION = 1000000;

/* Returns a timespec representing the elapsed time between
 * start and end.
 * Assumption: end is bigger (later) than start. */
timespec Timer::elapsed(timespec &start, timespec &end) {
    timespec result;
    if(end.tv_nsec < start.tv_nsec) { // borrow from tv_sec
        result.tv_sec = (end.tv_sec - 1) - start.tv_sec;
        result.tv_nsec = (end.tv_nsec + BILLION) - start.tv_nsec;
    } else {
        result.tv_sec = end.tv_sec - start.tv_sec;
        result.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return result;
}

void Timer::updateFPS() {
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  frameTime = elapsed(lastTime, now);
  lastTime = now;
}

float Timer::getFPS() {
  return BILLION / float(frameTime.tv_nsec);
}

float Timer::getSPF() {
  return float(frameTime.tv_nsec) / MILLION;
}
#else

void Timer::updateFPS() {
}

float Timer::getFPS() {
  return 0;
}

float Timer::getSPF() {
  return 0;
}

#endif

void Timer::printFPS() {
  std::cout << "FPS" << getFPS() << "," << getSPF() << "ms per Frame\n";
}

float Timer::getTime() {
#ifdef UNIX
  timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return (float(now.tv_sec) + float(now.tv_nsec)/BILLION);
#endif
}
