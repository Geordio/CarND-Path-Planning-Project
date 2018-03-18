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

#include <string>

//using namespace std;

class Lane {
public:
  Lane();
  virtual ~Lane();


  Car calcNearestAheadCar();
  Car calcNearestProjectedAheadCar();
  Car getNearestProjectedAheadCar();
  Car getNearestAheadCar();
  Car calcNearestProjectedBehindCar();
  Car getNearestProjectedBehindCar();
  Car calcNearestBehindCar();
  Car getNearestBehindCar();
  double getLaneAvgSpeed();
  int getNumberAheadCars();
  std::vector<Car> getThreatCars();
  double getLaneSafetyCost();
  double getLaneEfficencyCost();
  std::vector<Car> getCars(Car car);
  void addCar(Car car);
  std::string getNearestProjectedAheadCarSpeedTxt();
  std::string getNearestProjectedAheadCarSTxt();
  std::string getNearestProjectedAheadCarDeltaSTxt();
  std::string getNearestProjectedBehindCarSpeedTxt();
  std::string getNearestProjectedBehindCarSTxt();
  std::string getNearestBehindCarDeltaSTxt();

  int NUMBER_OF_LANES = 3;

  void evaluate();

  std::vector<Car> lane_cars;

  int laneNumber;
  int numberThreatCars = 0;
  int numberAheadCars = 0;
  int numberBehindCars = 0;
  int numberNearAheadCars = 0;
  Car nearest_ahead_car;
  Car nearest_projected_ahead_car;
  Car nearest_behind_car;
  Car nearest_projected_behind_car;
  int numberTotalCars = 0;

  bool hasThreatCars = false;
  bool hasAheadCar = false;
  bool hasBehindCar = false;
  bool hasProjectedAheadCar = false;
  bool hasProjectedBehindCar = false;
  bool hasCar = false;

  double laneSafetyCost = 99;
  double laneEfficencyCost = 99;

  // threat zone definitions.
  double threatZoneFrontLimit = 20;
  double threatZoneRearLimit = -10;

  // threat zone definitions.
  double nearZoneFrontLimit = 30;
  double nearZoneRearLimit = -30;

  double nearest_ahead_car_speed = 0;

    bool isCurrentLane = false;


  void sortByDeltaS();
  void sortByProjectedDeltaS();
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
