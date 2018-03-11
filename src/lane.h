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

  int laneNumber;
  Car getNearestAheadCar();
  Car getNearestBehindCar();
  double getLaneAvgSpeed();
  int getNoAheadCars();
  int noThreatCars =0;
  vector<Car> getThreatCars();
  double getLaneCost();
  vector<Car> getCars(Car car);
  void addCar(Car car);

  int NUMBER_OF_LANES = 3;

  void evaluate();

  vector<Car> lane_cars;

  int numberAheadCars = 0;
  int numberNearAheadCars = 0;
  bool hasThreatCars = false;
  bool hasAheadCar = false;
  bool hasCar = false;
  double laneCost = 99;

  // threat zone definitions.
  double threatZoneFrontLimit = 5;
  double threatZoneRearLimit = -25;

  // threat zone definitions.
  double nearZoneFrontLimit = 40;
  double nearZoneRearLimit = -10;

  double nearest_ahead_car_speed = 0;
    Car nearest_ahead_car;
    bool isCurrentLane = false;


  void sortByDeltaS();
//  static struct lane_delta_s_comparer;

//  static struct lane_delta_s_comparer{
//    bool operator()(const Car& lhs, const Car& rhs);
//  };
//  bool compareCars(const Car* lhs, const Car* rhs);

//  }
};

//static struct lane_delta_s_comparer{
//  bool operator()(const Car& lhs, const Car& rhs);
//};
#endif /* LANE_H_ */
