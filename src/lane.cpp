/*
 * lane.cpp
 *
 *  Created on: 2 Mar 2018
 *      Author: george
 */


#include "lane.h"
#include "car.h"
#include <vector>
#include <iostream>
#include <algorithm>


using namespace std;



Lane::Lane() {
  // TODO Auto-generated constructor stub

}

Lane::~Lane() {
  // TODO Auto-generated destructor stub
}



Car Lane::getNearestAheadCar(){

  //  Car nearest_car;
  int nearest_s = 99999;
  for (int i = 0; i < this->lane_cars.size(); i++){
    if (this->lane_cars[i].car_projected_delta_s > 0 ) {
      //      hasAheadCar=true;
      if (this->lane_cars[i].car_projected_delta_s < nearest_s) {
        nearest_s = this->lane_cars[i].car_projected_delta_s;
        nearest_ahead_car = this->lane_cars[i];
//        this->nearest_ahead_car_speed = nearest_ahead_car.car_speed;
      }
    }
  }

  return nearest_ahead_car;
}

Car Lane::getNearestBehindCar(){

//  Car nearest;
  int nearest_s = 99999;
  for (int i = 0; i < this->lane_cars.size(); i++){
    if (this->lane_cars[i].car_projected_delta_s < 0 ) {
      if (abs(this->lane_cars[i].car_projected_delta_s) < nearest_s) {
        nearest_s = this->lane_cars[i].car_projected_delta_s;
        nearest_behind_car = this->lane_cars[i];
//        nearest = this->lane_cars[i];
      }
    }
  }

  return nearest_behind_car;
}


// Add a car to the vector of cars in the lane
// increment the counts for total, ahead and behind cars
void Lane::addCar(Car car) {
  //cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tadd_car"<<endl;

  lane_cars.push_back(car);
  this->numberTotalCars++;

  bool hasCar = true;


  if (car.car_projected_delta_s > 0 ) {
    //    cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tahead_car"<<endl;
    this->hasAheadCar = true;
    this->numberAheadCars++;
//    this->numberNearAheadCars++;

  }
  else {
    this->hasBehindCar = true;
    this->numberBehindCars++;
  }

}



// calculate and return the average speed of the other cars in the lane
double Lane::getLaneAvgSpeed(){
  double total_speed = 0;

  for (int i = 0; i < this->lane_cars.size(); i++){
    total_speed += this->lane_cars[i].car_speed;
  }

  return total_speed / this->lane_cars.size();
}

//int Lane::getNoThreatCars() {
//
//
//}


// Calculate if any of the cars in the lane should be consider a threat
// EG have a trajectory that makes them a risk to the EGO car
vector<Car> Lane::getThreatCars() {

  vector<Car> threatCars;
  //  int nearest_s = 99999;

  // iterate through the cars in the lane and identify is they are deemed a threat (i.e they are in vicinty
  // do this by checking if it is within a range
  // TODO:Note that this is based on the target cars current location, not their predicted one.
  // TODO handle the scenario where a car behind us is travelling so fast that its trajectory would take it through and out the threat area
  for (int i = 0; i < this->lane_cars.size(); i++){

    if ((this->lane_cars[i].car_projected_delta_s > threatZoneRearLimit ) && (this->lane_cars[i].car_projected_delta_s < threatZoneFrontLimit )) {
      //        nearest_s = this->lane_cars[i].car_delta_s;
      threatCars.push_back(lane_cars[i]);
      hasThreatCars = true;
      numberThreatCars++;
    }

//    else if (this->lane_cars[i].car_projected_delta_s < 0) {
//      // if the car is behind
//      if (this->lane_cars[i])
//    }
  }
  return threatCars;
}

void Lane::evaluate(){
  getThreatCars();
  getLaneAvgSpeed();
  getNearestAheadCar();
  getNearestBehindCar();
  getLaneCost();
  sortByDeltaS();
}

//TODO add ahead lane congestion
double Lane::getLaneCost() {
  double cost = 0;
  double safe_cost = 0;
  double congestion_cost = 0;
  double ahead_speed_cost = 0;
  double next_ahead_car_cost = 0;
  double not_inside_lane_cost = 0;

//  getThreatCars();

  // safety
  if (hasThreatCars)
    safe_cost = 2;
  else
    safe_cost = 0;

  if (hasAheadCar)
    next_ahead_car_cost = (50-this->nearest_ahead_car_speed)/50 + 10/nearest_ahead_car.car_projected_delta_s;
  else
    next_ahead_car_cost =0;

  //  if (hasAheadCar)
  //    next_ahead_car_cost = (50-this->ahead_car_speed)/50;
  //  else
  //    next_ahead_car_cost =0;

  // TODO consider all ahead cars
  if (hasAheadCar)
    congestion_cost = (this->getNoAheadCars())/10;
  else
    congestion_cost =0;


  not_inside_lane_cost = (NUMBER_OF_LANES - laneNumber-1) / 10;

  cost = safe_cost+ next_ahead_car_cost+ congestion_cost+not_inside_lane_cost;
  this->laneCost = cost;
  return cost;

}
//vector<Car> getCars();


void Lane::sortByDeltaS(){
  //  std::sort(lane_cars.begin(), lane_cars.end(), compareCars);
  //  std::sort(cars.begin(), cars.end(), compareCars);

  std::sort(lane_cars.begin(), lane_cars.end(),
      [](Car const & a, Car const & b) -> bool
      { return a.car_delta_s < b.car_delta_s; } );

  string order ="";
  for (int i = 0; i < lane_cars.size(); i++){
    order = order + std::to_string(lane_cars[i].car_delta_s) + ",";
  }

  //TODO delete cout
//  cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tOrder:"<<order<< endl;

}

void Lane::sortByProjectedDeltaS(){
  //  std::sort(lane_cars.begin(), lane_cars.end(), compareCars);
  //  std::sort(cars.begin(), cars.end(), compareCars);

  std::sort(lane_cars.begin(), lane_cars.end(),
      [](Car const & a, Car const & b) -> bool
      { return a.car_projected_delta_s < b.car_projected_delta_s; } );

  string order ="";
  for (int i = 0; i < lane_cars.size(); i++){
    order = order + std::to_string(lane_cars[i].car_projected_delta_s) + ",";
  }

  //TODO delete cout
//  cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tOrder:"<<order<< endl;

}

int Lane::getNoAheadCars() {

  return this->numberAheadCars;
}

// compare the delta s values in the lane to sort by largest s to smallest s
// note: s is not absolute here, so can be negative.
// hence does not sort in distance to the car
//struct Lane::lane_delta_s_comparer
//{
//  bool operator()(const Car& lhs, const Car& rhs)
//  {
//    return lhs.car_delta_s > rhs.car_delta_s;
//  }
//};

//struct lane_delta_s_comparer
//{
//  bool operator()(const Car& lhs, const Car& rhs)
//  {
//    return lhs.car_delta_s > rhs.car_delta_s;
//  }
//};

//static bool compareCars(const Car* lhs, const Car* rhs){
//    return lhs->getPassengers() < rhs->getPassengers();
//}

/*...*/

//std::sort(cars.begin(), cars.end(), compareCars);
