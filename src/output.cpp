/*
 * output.cpp
 *
 *  Created on: 12 Mar 2018
 *      Author: george
 */

#include <iomanip>
#include "output.h"

using namespace std;

bool debug = true;
bool debug_output_traffic = false;

int data0x = 22;
int data1x = 62;
int data2x = 102;
int data3x = 142;

int labelcol1 = 42;
int labelcol2 = 82;
int labelcol3 = 122;

// int labelEgoCoord[2] = {col0, ;
int labelLeftx = 0;
int labelCarSummary = 3;
int labelEgoCoordidy = labelCarSummary + 1;
int labelEgoCoorddy = labelCarSummary + 2;
int labelEgoActsy = labelCarSummary + 3;
int labelEgoPthEndsy = labelCarSummary + 4;
//int labelEgoVy = labelCarSummary + 5;
int labelEgoCoordxy = labelCarSummary + 5;
int labelEgoCoordyy = labelCarSummary + 6;
int labelEgoMaxvy = labelCarSummary + 7;
int labelEgoTgtvy = labelCarSummary + 8;
int labelEgoReqvy = labelCarSummary + 9;
int labelEgoActvy = labelCarSummary + 10;

int labelEgoStatey = labelCarSummary + 11;

//int labelBestLane = 9;

//int labelEgoCoordsy = 3;
//int labelEgoVx = 0;

//int labelEgoStatey = 0;

int labelLaneTitley = 17;
int labelLaneCnty = labelLaneTitley + 1;
int labelLaneAhdCnty = labelLaneTitley + 2;
int labelLaneAhdVehSpdy = labelLaneTitley + 3;
int labelLaneAhdDSpdy = labelLaneTitley + 4;
int labelLaneAhdSy = labelLaneTitley + 5;
int labelLaneAhdDSy = labelLaneTitley + 6;
int labelLaneAhdPDSy = labelLaneTitley + 7;

int labelLaneBhdVehSpdy = labelLaneAhdPDSy + 1;
int labelLaneBhdDSpdy = labelLaneAhdPDSy + 2;
int labelLaneBhdSy = labelLaneAhdPDSy + 3;
int labelLaneBhdDSy = labelLaneAhdPDSy + 4;
int labelLaneBhdPDSy = labelLaneAhdPDSy + 5;


int labelLaneThreaty = labelLaneBhdPDSy + 1;
int labelLaneCosty = labelLaneBhdPDSy + 2;

int labelTrafCoordsx = 2;
// Cars group heading
int labelCars_0y = 35;
int labelCar0_0y = labelCars_0y + 1;
int labelCar0_1y = labelCars_0y + 2;
int labelCar0_2y = labelCars_0y + 3;
int labelCar0_3y = labelCars_0y + 4;
int labelCar0_4y = labelCars_0y + 5;
int labelCar0_5y = labelCars_0y + 6;
int labelCar0_6y = labelCars_0y + 7;
int labelCar0_7y = labelCars_0y + 8;

int labelCarOffsety = 7;
int labelLane0x = data0x;
int labelLane1x = data1x;
int labelLane2x = data2x;

// set the location to output the text to
void SetCursorPos(int XPos, int YPos) {
  printf("\033[%d;%dH", YPos + 1, XPos + 1);
  // printf("test");
  // printf("\033[%d;%dH", 10, 10);
  // cout<< "test2";
}

void OutputData(int XPos, int YPos, string text) {

  SetCursorPos(XPos, YPos);
  //  std::cout << std::setprecision(2);
  //  std::cout << std::fixed;
  cout << text << std::flush;
}

// output the debug labels in the terminal
void OutputLabels() {
  // clear the terminal
  cout << "\033[2J\033[1;1H";
  OutputData(labelLeftx, labelCarSummary, "CAR SUMMARY: ");
  OutputData(data0x, labelCarSummary, "EGO SUMMARY: ");
  OutputData(data1x, labelCarSummary, "AHEAD SUMMARY: ");
  OutputData(data2x, labelCarSummary, "BEHIND SUMMARY: ");

  // EGO vehicle labels
  OutputData(labelLeftx, labelEgoCoordidy, "Veh id: ");
  OutputData(labelLeftx, labelEgoCoorddy, "Veh d: ");
  OutputData(labelLeftx, labelEgoActsy, "Veh s: ");
  OutputData(labelLeftx, labelEgoPthEndsy, "Veh Path s: ");
//  OutputData(labelLeftx, labelEgoVy, "Veh v: ");
  OutputData(labelLeftx, labelEgoCoordxy, "Veh x: ");
  OutputData(labelLeftx, labelEgoCoordyy, "Veh y: ");
  OutputData(labelLeftx, labelEgoMaxvy, "Veh Max V: ");
  OutputData(labelLeftx, labelEgoTgtvy, "Veh Tgt V: ");
  OutputData(labelLeftx, labelEgoReqvy, "Veh Req V: ");
  OutputData(labelLeftx, labelEgoActvy, "Veh Act V: ");

  // Lane summary
  OutputData(labelLeftx, labelEgoStatey, "State: ");
  OutputData(labelLeftx, labelLaneTitley, "LANE: ");
  OutputData(data0x, labelLaneTitley, "Lane0: ");
  OutputData(data1x, labelLaneTitley, "Lane1 : ");
  OutputData(data2x, labelLaneTitley, "Lane2 : ");
  OutputData(labelLeftx, labelLaneCnty, "Total Veh Cnt: ");
  OutputData(labelLeftx, labelLaneAhdCnty, "Ahead Veh Cnt: ");
  OutputData(labelLeftx, labelLaneAhdVehSpdy, "Ahead Veh Spd: ");
  OutputData(labelLeftx, labelLaneAhdDSpdy, "Ahead Veh Del Spd: ");
  OutputData(labelLeftx, labelLaneAhdSy, "Ahead Veh S: ");
  OutputData(labelLeftx, labelLaneAhdDSy, "Ahead Veh Del S: ");
  OutputData(labelLeftx, labelLaneAhdPDSy, "Ahead Veh Proj Del S: ");

  OutputData(labelLeftx, labelLaneBhdVehSpdy, "Behind Veh Spd: ");
  OutputData(labelLeftx, labelLaneBhdDSpdy, "Behind Veh Del Spd: ");
  OutputData(labelLeftx, labelLaneBhdSy, "Behind Veh S: ");
  OutputData(labelLeftx, labelLaneBhdDSy, "Behind Veh Del S: ");
  OutputData(labelLeftx, labelLaneBhdPDSy, "Behind Veh Proj Del S: ");

  OutputData(labelLeftx, labelLaneThreaty, "No Threat Veh: ");
  OutputData(labelLeftx, labelLaneCosty, "Lane Cost: ");

  OutputData(labelLeftx, labelCars_0y, "CARS: ");

  for (int i = 0; i < 6; i++) {
    OutputData(labelTrafCoordsx, labelCar0_0y + i * labelCarOffsety,
        "veh id: ");
    OutputData(labelTrafCoordsx, labelCar0_1y + i * labelCarOffsety, "veh d: ");
    OutputData(labelTrafCoordsx, labelCar0_2y + i * labelCarOffsety, "veh s: ");
    OutputData(labelTrafCoordsx, labelCar0_3y + i * labelCarOffsety, "veh v: ");
    OutputData(labelTrafCoordsx, labelCar0_4y + i * labelCarOffsety,
        "veh delta s: ");
    OutputData(labelTrafCoordsx, labelCar0_5y + i * labelCarOffsety,
        "veh proj s: ");
  }

  SetCursorPos(0, 0);
}

// output the debug information for the other cars in the traffic
void output_traffic_debug(vector<Car> lane_cars, int datax) {
  for (int i = 0; i < 6; i++) {
    OutputData(datax, labelCar0_0y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_1y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_2y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_3y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_4y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_5y + i * labelCarOffsety, "               ");
    OutputData(datax, labelCar0_6y + i * labelCarOffsety, "               ");
  }
  for (int i = 0; i < lane_cars.size(); i++) {
    Car car = lane_cars[i];
    OutputData(datax, labelCar0_0y + i * labelCarOffsety,
        std::to_string(car.id));
    OutputData(datax, labelCar0_1y + i * labelCarOffsety,
        std::to_string(car.d)); //,"veh 1 d: ");
    OutputData(datax, labelCar0_2y + i * labelCarOffsety,
        std::to_string(car.s)); //,"veh 2 s: ");
    OutputData(datax, labelCar0_3y + i * labelCarOffsety,
        std::to_string(car.getSpeedMph())); //,"veh 3 v: ");
    OutputData(datax, labelCar0_4y + i * labelCarOffsety,
        std::to_string(car.delta_s)); //,"veh 4 delta s: ");
    OutputData(datax, labelCar0_5y + i * labelCarOffsety,
        std::to_string(car.projected_delta_s)); //,"veh 4 delta s: ");
  }
}

void OutputEgo(double max_speed, double ego_target_speed, double ego_req_speed, double ego_car_speed, double ego_car_x,double ego_car_y, double ego_car_actual_s, double end_path_s, double ego_car_d){
  OutputData(data0x, labelEgoMaxvy, std::to_string(max_speed));
  OutputData(data0x, labelEgoTgtvy, std::to_string(ego_target_speed));
  OutputData(data0x, labelEgoReqvy, std::to_string(ego_req_speed));
  OutputData(data0x, labelEgoActvy, std::to_string(ego_car_speed));
  OutputData(data0x, labelEgoCoordxy, std::to_string(ego_car_x));
  OutputData(data0x, labelEgoCoordyy, std::to_string(ego_car_y));
  OutputData(data0x, labelEgoActsy, std::to_string(ego_car_actual_s));
  OutputData(data0x, labelEgoPthEndsy, std::to_string(end_path_s));
  OutputData(data0x, labelEgoCoorddy, std::to_string(ego_car_d));
}

void OutputLaneSummary(vector<Lane> lanes) {
  Lane lane0 = lanes[0];
  Lane lane1 = lanes[1];
  Lane lane2 = lanes[2];

  OutputData(data0x, labelLaneCosty, "                                        ");
  OutputData(data1x, labelLaneCosty, "                                        ");
  OutputData(data2x, labelLaneCosty, "                                        ");
  OutputData(data0x, labelLaneAhdDSy, "                                        ");
  OutputData(data1x, labelLaneAhdDSy, "                                        ");
  OutputData(data2x, labelLaneAhdDSy, "                                        ");
  OutputData(data0x, labelLaneCosty, "                                        ");
  OutputData(data1x, labelLaneCosty, "                                        ");
  OutputData(data2x, labelLaneCosty, "                                        ");

  OutputData(data0x, labelLaneCnty, std::to_string(lane0.numberTotalCars));
  OutputData(data1x, labelLaneCnty, std::to_string(lane1.numberTotalCars));
  OutputData(data2x, labelLaneCnty, std::to_string(lane2.numberTotalCars));
  OutputData(data0x, labelLaneAhdCnty, std::to_string(lane0.numberAheadCars));
  OutputData(data1x, labelLaneAhdCnty, std::to_string(lane1.numberAheadCars));
  OutputData(data2x, labelLaneAhdCnty, std::to_string(lane2.numberAheadCars));
  //  OutputData(data0x, labelLaneAhdVehSpdy,
  //      std::to_string(lane0.nearest_ahead_car.car_speed * 2.24));
  //  OutputData(data1x, labelLaneAhdVehSpdy,
  //      std::to_string(lane1.nearest_ahead_car.car_speed * 2.24));
  //  OutputData(data2x, labelLaneAhdVehSpdy,
  //      std::to_string(lane2.nearest_ahead_car.car_speed * 2.24));

  OutputData(data0x, labelLaneAhdPDSy,lane0.getNearestProjectedAheadCarDeltaSTxt());
  OutputData(data1x, labelLaneAhdPDSy,lane1.getNearestProjectedAheadCarDeltaSTxt());
  OutputData(data2x, labelLaneAhdPDSy,lane2.getNearestProjectedAheadCarDeltaSTxt());

  OutputData(data0x, labelLaneAhdSy,lane0.getNearestProjectedAheadCarSTxt());
  OutputData(data1x, labelLaneAhdSy,lane1.getNearestProjectedAheadCarSTxt());
  OutputData(data2x, labelLaneAhdSy,lane2.getNearestProjectedAheadCarSTxt());

  OutputData(data0x, labelLaneAhdDSy,std::to_string(lane0.nearest_ahead_car.delta_s));
  OutputData(data1x, labelLaneAhdDSy,std::to_string(lane1.nearest_ahead_car.delta_s));
  OutputData(data2x, labelLaneAhdDSy,std::to_string(lane2.nearest_ahead_car.delta_s));

  OutputData(data0x, labelLaneAhdDSpdy,std::to_string(lane0.getNearestAheadCar().delta_speed));
  OutputData(data1x, labelLaneAhdDSpdy,std::to_string(lane1.getNearestAheadCar().delta_speed));
  OutputData(data2x, labelLaneAhdDSpdy,std::to_string(lane2.getNearestAheadCar().delta_speed));

  OutputData(data0x, labelLaneAhdVehSpdy, lane0.getNearestProjectedAheadCarSpeedTxt());
  OutputData(data1x, labelLaneAhdVehSpdy, lane1.getNearestProjectedAheadCarSpeedTxt());
  OutputData(data2x, labelLaneAhdVehSpdy, lane2.getNearestProjectedAheadCarSpeedTxt());

  //  OutputData(data0x, labelLaneAhdSy,
  //      std::to_string(lane0.nearest_ahead_car.car_projected_delta_s));
  //  OutputData(data1x, labelLaneAhdSy,
  //      std::to_string(lane1.nearest_ahead_car.car_projected_delta_s));
  //  OutputData(data2x, labelLaneAhdSy,
  //      std::to_string(lane2.nearest_ahead_car.car_projected_delta_s));

    OutputData(data0x, labelLaneBhdPDSy,lane0.getNearestBehindCarDeltaSTxt());
    OutputData(data1x, labelLaneBhdPDSy,lane1.getNearestBehindCarDeltaSTxt());
    OutputData(data2x, labelLaneBhdPDSy,lane2.getNearestBehindCarDeltaSTxt());

    OutputData(data0x, labelLaneBhdSy,lane0.getNearestProjectedBehindCarSTxt());
    OutputData(data1x, labelLaneBhdSy,lane1.getNearestProjectedBehindCarSTxt());
    OutputData(data2x, labelLaneBhdSy,lane2.getNearestProjectedBehindCarSTxt());

    OutputData(data0x, labelLaneBhdDSpdy,std::to_string(lane0.getNearestBehindCar().delta_speed));
    OutputData(data1x, labelLaneBhdDSpdy,std::to_string(lane1.getNearestBehindCar().delta_speed));
    OutputData(data2x, labelLaneBhdDSpdy,std::to_string(lane2.getNearestBehindCar().delta_speed));

    OutputData(data0x, labelLaneBhdVehSpdy,
        std::to_string(lane0.nearest_behind_car.speed * 2.24));
    OutputData(data1x, labelLaneBhdVehSpdy,
        std::to_string(lane1.nearest_behind_car.speed * 2.24));
    OutputData(data2x, labelLaneBhdVehSpdy,
        std::to_string(lane2.nearest_behind_car.speed * 2.24));
    OutputData(data0x, labelLaneBhdSy,
        std::to_string(lane0.nearest_behind_car.projected_delta_s));
    OutputData(data1x, labelLaneBhdSy,
        std::to_string(lane1.nearest_behind_car.projected_delta_s));
    OutputData(data2x, labelLaneBhdSy,
        std::to_string(lane2.nearest_behind_car.projected_delta_s));
    OutputData(data0x, labelLaneThreaty, std::to_string(lane0.hasThreatCars)+", "+ std::to_string(lane0.numberThreatCars) );
    OutputData(data1x, labelLaneThreaty, std::to_string(lane1.numberThreatCars));
    OutputData(data2x, labelLaneThreaty, std::to_string(lane2.numberThreatCars));
    OutputData(data0x, labelLaneCosty, std::to_string(lane0.laneEfficencyCost));
    OutputData(data1x, labelLaneCosty, std::to_string(lane1.laneEfficencyCost));
    OutputData(data2x, labelLaneCosty, std::to_string(lane2.laneEfficencyCost));

  if (debug_output_traffic) {
    output_traffic_debug(lane0.lane_cars, data0x);
    output_traffic_debug(lane1.lane_cars, data1x);
    output_traffic_debug(lane2.lane_cars, data2x);
  }

}
