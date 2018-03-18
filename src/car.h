/*
 * car.h
 *
 *  Created on: 2 Mar 2018
 *      Author: george
 */

#ifndef CAR_H_
#define CAR_H_

class Car {
public:
  Car();
  Car(int id, double s, double d, double speed,double delta_s);
  Car(int id, double s, double d, double speed,double delta_s, double projected_s,double projected_delta_s,double  _delta_speed);
  virtual ~Car();

  int id;
  double x;
  double y;
  double s;
  double d;
  double car_yaw;
  double speed;
  double delta_speed;
  double delta_s;
  double projected_s;
  double projected_delta_s;

 double getSpeedMph();
 int get_lane();
 double get_delta_s();
// bool Car::operator > (const Car& str) const;
};





#endif /* CAR_H_ */
