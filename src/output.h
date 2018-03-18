#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>
#include <math.h>
#include <vector>

//#include <curses.h>
#include <unistd.h>
#include <term.h>
#include "car.h"
#include "lane.h"



extern bool debug;
extern bool debug_output_traffic;
extern int data0x;
extern int data1x;
extern int data2x;
extern int data3x;

extern int labelLeftx;
extern int labelCarSummary;
extern int labelEgoCoordidy;
extern int labelEgoCoorddy;
extern int labelEgoActsy;
extern int labelEgoPthEndsy;
extern int labelEgoVy;
extern int labelEgoCoordxy;
extern int labelEgoCoordyy;
extern int labelEgoMaxvy;
extern int labelEgoTgtvy;
extern int labelEgoReqvy;
extern int labelEgoActvy;


extern int labelEgoStatey;




//extern int labelEgoStatey;

extern int labelLaneTitley;
extern int labelLaneCnty;
extern int labelLaneAhdCnty;
extern int labelLaneAhdVehSpdy;
extern int labelLaneAhdSy;
extern int labelLaneBhdVehSpdy;
extern int labelLaneBhdSy;
extern int labelLaneThreaty;
extern int labelLaneCosty;


//int labelLane0x = 0;
extern int labelLane0x;
extern int labelLane1x;
extern int labelLane2x;

extern int labelTrafCoordsx;
// Cars group heding
extern int labelCars_0y;
extern int labelCar0_0y;
extern int labelCar0_1y;
extern int labelCar0_2y;
extern int labelCar0_3y;
extern int labelCar0_4y;
extern int labelCar0_5y;
extern int labelCar0_6y;
extern int labelCar0_7y;

extern int labelCarOffsety;

void SetCursorPos(int XPos, int YPos);
void OutputData(int XPos, int YPos, std::string text);
void OutputLabels();
void output_traffic_debug(std::vector<Car> lane_cars, int datax);
void OutputLaneSummary(std::vector<Lane> lanes);
void OutputEgo(double max_speed, double ego_target_speed, double ego_req_speed, double ego_car_speed, double ego_car_x,double ego_car_y, double ego_car_actual_s, double end_path_s, double ego_car_d);


#endif /* OUTPUT_H */
