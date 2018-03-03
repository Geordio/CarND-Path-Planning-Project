/*
 * Car.cpp
 *
 *  Created on: 2 Mar 2018
 *      Author: george
 */

#include "car.h"

Car::Car() {
  // TODO Auto-generated constructor stub

}

Car::Car(int id, double s, double d, double speed, double delta_s) {
  // TODO Auto-generated constructor stub
  this->car_id = id;
  //  this->car_x= x;
  //  this->car_y=y;
  this-> car_d=d;
  this-> car_s=s;
  this->car_speed=speed;
  this->car_delta_s = delta_s;
  //  double car_yaw;
  //  double car_speed;
}

Car::~Car() {
  // TODO Auto-generated destructor stub
}

double Car::getSpeedMph(){
  return this->car_speed*2.24;
}

