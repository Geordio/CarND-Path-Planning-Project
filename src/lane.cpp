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

using namespace std;



Lane::Lane() {
  // TODO Auto-generated constructor stub

}

Lane::~Lane() {
  // TODO Auto-generated destructor stub
}



Car Lane::getNearestAheadCar(){

  Car nearest;
  int nearest_s = 99999;
  for (int i = 0; i < this->lane_cars.size(); i++){
    if (this->lane_cars[i].car_delta_s > 0 ) {
//      hasAheadCar=true;
      if (this->lane_cars[i].car_delta_s < nearest_s) {
        nearest_s = this->lane_cars[i].car_delta_s;
        nearest = this->lane_cars[i];
      }
    }
  }

  return nearest;
}

Car Lane::getNearestBehindCar(){

  Car nearest;
  int nearest_s = 99999;
  for (int i = 0; i < this->lane_cars.size(); i++){
    if (this->lane_cars[i].car_delta_s < 0 ) {
      if (this->lane_cars[i].car_delta_s > nearest_s) {
        nearest_s = this->lane_cars[i].car_delta_s;
        nearest = this->lane_cars[i];
      }
    }
  }

  return nearest;
}

void Lane::addCar(Car car) {
cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tadd_car"<<endl;

  lane_cars.push_back(car);

  bool hasCar = true;
  if (car.car_delta_s > 0 ) {
    cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\tahead_car"<<endl;
    this->hasAheadCar = true;
  }
}


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

vector<Car> Lane::getThreatCars() {

  vector<Car> threatCars;
  //  int nearest_s = 99999;
  for (int i = 0; i < this->lane_cars.size(); i++){
    if ((this->lane_cars[i].car_delta_s > threatZoneRearLimit ) && (this->lane_cars[i].car_delta_s < threatZoneFrontLimit )) {
      //        nearest_s = this->lane_cars[i].car_delta_s;
      threatCars.push_back(lane_cars[i]);
      hasThreatCars = true;
    }
  }
  return threatCars;
}



//double getLaneCost() {
//double cost = 0;
//double safe_cost = 0;
//
//getThreatCars();
//
//// safety
//if (hasThreatCars)
//  safe_cost = 1;
//else
//  safe_cost = 0;
//
//
//
//return cost;
//
//}
//vector<Car> getCars();
//int getNoAheadCars();
