/*
 * Timer.h
 *
 *  Created on: Apr 10, 2011
 *      Author: bmonkey
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

class Timer {
private:
#ifdef UNIX
  timespec startTime, lastTime, frameTime;
  static timespec elapsed(timespec &start, timespec &end);
#else
  //TODO: Timer for Win
#endif

public:
  Timer();
  virtual ~Timer();
  void updateFPS();
  void printFPS();
  float getFPS();
  float getSPF();
  float getTime();
};

#endif /* TIMER_H_ */
