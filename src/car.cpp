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
  this->id = id;
  //  this->car_x= x;
  //  this->car_y=y;
  this-> d=d;
  this-> s=s;
  this->speed=speed;
  this->delta_s = delta_s;
  //  double car_yaw;
  //  double car_speed;
}

Car::Car(int _id, double _s, double _d, double _speed, double _delta_s, double _projected_s,double _projected_delta_s, double _delta_speed) {
  // TODO Auto-generated constructor stub
  this->id = _id;
  //  this->car_x= x;
  //  this->car_y=y;
  this->d=_d;
  this->s=_s;
  this->speed=_speed;
  this->delta_s = _delta_s;
  this->projected_s = _projected_s;
  this->projected_delta_s = _projected_delta_s;
  this->delta_speed = _delta_speed;
  //  double car_yaw;
  //  double car_speed;
}

Car::~Car() {
  // TODO Auto-generated destructor stub
}

double Car::getSpeedMph(){
  return this->speed*2.24;
}


double Car::get_delta_s(){
  return this->delta_s;
}

//bool Car::isThreat() {
//
//}
//bool Car::operator > (const Car& str) const
//{
//    return (car_delta_s > str->delta_s);
//}
