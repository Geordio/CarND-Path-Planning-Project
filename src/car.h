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
  virtual ~Car();

  int car_id;
  double car_x;
  double car_y;
  double car_s;
  double car_d;
  double car_yaw;
  double car_speed;
  double car_delta_s;

 double getSpeedMph();
 int get_lane();

};





#endif /* CAR_H_ */
