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
  vector<Car> getThreatCars();
  double getLaneCost();
  vector<Car> getCars(Car car);
  void addCar(Car car);

  vector<Car> lane_cars;

  bool hasThreatCars = false;
  bool hasAheadCar = false;
  bool hasCar = false;
  double threatZoneFrontLimit = 40;
  double threatZoneRearLimit = -20;
};

#endif /* LANE_H_ */
