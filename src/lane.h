/*
 * lane.h
 *
 *  Created on: 2 Mar 2018
 *      Author: george
 */

#ifndef LANE_H_
#define LANE_H_

#include <vector>
#include "car.h"

using namespace std;

class Lane {
public:
  Lane();
  virtual ~Lane();


  Car getNearestAheadCar();
  Car getNearestBehindCar();
  double getLaneAvgSpeed();
  int getNoAheadCars();
  int getNoThreatCars();
  double getLaneCost();
  vector<Car> getCars();

  vector<Car> lane_cars;
};

#endif /* LANE_H_ */
