#include <fstream>
#include <math.h>
#include <uWS/uWS.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include "json.hpp"
#include "spline.h"
//#include <curses.h>
#include <unistd.h>
#include <term.h>
#include "car.h"
#include "lane.h"

using namespace std;

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

int target_lane = 1;
double ref_vel = 0; //49.5



bool debug = true;

int SF_ID_IND = 0;
const int SF_IND_X = 1;
const int SF_IND_Y = 2;
const int SF_VX_IND = 3;
const int SF_VY_IND = 4;
const int SF_S_IND = 5;
const int SF_D_IND = 6;


const int lane_num0 = 0;
const int lane_num1 = 1;
const int lane_num2 = 2;

const int data0x = 22;
const int data1x = 62;
const int data2x = 102;
const int data3x = 142;

const int labelcol1 = 42;
const int labelcol2 = 82;
const int labelcol3 = 122;

//const int labelEgoCoord[2] = {col0, ;
int labelLeftx = 0;
int labelCarSummary = 3;
int labelEgoCoordidy = labelCarSummary+1;
int labelEgoCoorddy = labelCarSummary+2;
int labelEgoActsy = labelCarSummary+3;
int labelEgoPthEndsy = labelCarSummary+4;
int labelEgoVy = labelCarSummary+5;
int labelEgoCoordxy = labelCarSummary+6;
int labelEgoCoordyy = labelCarSummary+7;
int labelEgoMaxvy = labelCarSummary+8;
int labelEgoTgtvy = labelCarSummary+9;
int labelEgoReqvy = labelCarSummary+10;
int labelEgoActvy = labelCarSummary+11;


int labelNoWayPtsy = labelCarSummary+12;

const double max_speed = 49.5;
double ego_req_speed = 0;
//int labelBestLane = 9;

//int labelEgoCoordsy = 3;
//int labelEgoVx = 0;


int labelEgoStatey = 0;

int labelLaneTitley =17;
int labelLaneCnty =labelLaneTitley+1;
int labelLaneAhdCnty =labelLaneTitley+2;
int labelLaneAhdVehSpdy = labelLaneTitley+3;
int labelLaneAhdSy = labelLaneTitley+4;
int labelLaneBhdVehSpdy = labelLaneTitley+5;
int labelLaneBhdSy = labelLaneTitley+6;
int labelLaneThreaty = labelLaneTitley+7;
int labelLaneCosty = labelLaneTitley+8;


//int labelLane0x = 0;
int labelLane0x = data0x;
int labelLane1x = data1x;
int labelLane2x = data2x;

int labelTrafCoordsx = 2;
// Cars group heading
int labelCars_0y = 28;
int labelCar0_0y = labelCars_0y +1;
int labelCar0_1y = labelCars_0y +2;
int labelCar0_2y = labelCars_0y +3;
int labelCar0_3y = labelCars_0y +4;
int labelCar0_4y = labelCars_0y +5;
int labelCar0_5y = labelCars_0y +6;
int labelCar0_6y = labelCars_0y +7;
int labelCar0_7y = labelCars_0y +8;

int labelCarOffsety = 7;
//int
Car car = Car();
Lane ln;

// compare the delta s values in the lane to sort by largest s to smallest s
// note: s is not absolute here, so can be negative.
// hence does not sort in distance to the car
struct lane_delta_s_comparer
{
  bool operator()(const Car& lhs, const Car& rhs)
  {
    return lhs.car_delta_s > rhs.car_delta_s;
  }
};


int getBestLane(Lane lane0, Lane lane1, Lane lane2){


  int bestLane = 99;


  if (lane0.getLaneCost() < lane1.getLaneCost()) {
    // lane0 is lower that lane 1
    if (lane0.getLaneCost() < lane2.getLaneCost())
      // lane 0 is lower that lane 2 => lane 0 is the lowest
      bestLane = lane_num0;
    else
      // lane 2 is lower than lane 0. lane 0 was lower than lane 1 => lane 2 is the lowest
      bestLane = lane_num2;
  }
  else if (lane1.getLaneCost() < lane2.getLaneCost()){
    bestLane = lane_num1;
  }
  else
    bestLane = lane_num2;

}


int getNextLane (Lane lane0, Lane lane1, Lane lane2, int currentLane){

  int nextLane = 1;

  int bestLane = getBestLane(lane0, lane1, lane2);
  //TODO delete cout

//  cout << "______________________________________________________" << bestLane << endl;
//  cout << "______________________________________________________" << currentLane << endl;

  //  if (currentLane == bestLane){
  //    //stay in this lane
  //    nextLane = currentLane;
  //  }
  //  else
  if (currentLane > bestLane)
  {
    // left change lane
    nextLane = currentLane -1;

  }
  else if (currentLane < bestLane)
  {
    // right change lane
    nextLane = currentLane +1;

  }


  return nextLane;

}

int getCurrentLaneNumber(double d){
  int thelane;
  if (d > 0 && d <= 4)
    thelane = lane_num0;
  else if (d > 4 && d <= 8)
    thelane = lane_num1;
  else if (d > 8 && d <= 12)
    thelane = lane_num2;
  else
    thelane = 99;
  return thelane;
}

Lane getCurrentLane(double d, Lane lane0, Lane lane1, Lane lane2 ){
  Lane currentlane;



  int current_lane_number = getCurrentLaneNumber(d);

  if (current_lane_number == 0) {
    currentlane = lane0;
  } else
  if (current_lane_number == 1) {
    currentlane = lane1;
  } else
  if (current_lane_number == 2) {
    currentlane = lane2;
  } else {
  }

  return currentlane;
}



void SetCursorPos(int XPos, int YPos)
{
  printf("\033[%d;%dH", YPos+1, XPos+1);
  // printf("test");
  // printf("\033[%d;%dH", 10, 10);
  // cout<< "test2";
}

void OutputData(int XPos, int YPos, string text)
{

  SetCursorPos( XPos, YPos);
  cout<< text<< std::flush;
}



// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_first_of("}");
  if (found_null != string::npos) {
    return "";
  } else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 2);
  }
  return "";
}

double distance(double x1, double y1, double x2, double y2)
{
  return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}
int ClosestWaypoint(double x, double y, const vector<double> &maps_x, const vector<double> &maps_y)
{

  double closestLen = 100000; //large number
  int closestWaypoint = 0;

  for(int i = 0; i < maps_x.size(); i++)
  {
    double map_x = maps_x[i];
    double map_y = maps_y[i];
    double dist = distance(x,y,map_x,map_y);
    if(dist < closestLen)
    {
      closestLen = dist;
      closestWaypoint = i;
    }

  }

  return closestWaypoint;

}

int NextWaypoint(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
{

  int closestWaypoint = ClosestWaypoint(x,y,maps_x,maps_y);

  double map_x = maps_x[closestWaypoint];
  double map_y = maps_y[closestWaypoint];

  double heading = atan2((map_y-y),(map_x-x));

  double angle = fabs(theta-heading);
  angle = min(2*pi() - angle, angle);

  if(angle > pi()/4)
  {
    closestWaypoint++;
    if (closestWaypoint == maps_x.size())
    {
      closestWaypoint = 0;
    }
  }

  return closestWaypoint;
}

// Transform from Cartesian x,y coordinates to Frenet s,d coordinates
vector<double> getFrenet(double x, double y, double theta, const vector<double> &maps_x, const vector<double> &maps_y)
                        {
  int next_wp = NextWaypoint(x,y, theta, maps_x,maps_y);

  int prev_wp;
  prev_wp = next_wp-1;
  if(next_wp == 0)
  {
    prev_wp  = maps_x.size()-1;
  }

  double n_x = maps_x[next_wp]-maps_x[prev_wp];
  double n_y = maps_y[next_wp]-maps_y[prev_wp];
  double x_x = x - maps_x[prev_wp];
  double x_y = y - maps_y[prev_wp];

  // find the projection of x onto n
  double proj_norm = (x_x*n_x+x_y*n_y)/(n_x*n_x+n_y*n_y);
  double proj_x = proj_norm*n_x;
  double proj_y = proj_norm*n_y;

  double frenet_d = distance(x_x,x_y,proj_x,proj_y);

  //see if d value is positive or negative by comparing it to a center point

  double center_x = 1000-maps_x[prev_wp];
  double center_y = 2000-maps_y[prev_wp];
  double centerToPos = distance(center_x,center_y,x_x,x_y);
  double centerToRef = distance(center_x,center_y,proj_x,proj_y);

  if(centerToPos <= centerToRef)
  {
    frenet_d *= -1;
  }

  // calculate s value
  double frenet_s = 0;
  for(int i = 0; i < prev_wp; i++)
  {
    frenet_s += distance(maps_x[i],maps_y[i],maps_x[i+1],maps_y[i+1]);
  }

  frenet_s += distance(0,0,proj_x,proj_y);

  return {frenet_s,frenet_d};

                        }

// Transform from Frenet s,d coordinates to Cartesian x,y
vector<double> getXY(double s, double d, const vector<double> &maps_s, const vector<double> &maps_x, const vector<double> &maps_y)
                        {
  int prev_wp = -1;

  while(s > maps_s[prev_wp+1] && (prev_wp < (int)(maps_s.size()-1) ))
  {
    prev_wp++;
  }

  int wp2 = (prev_wp+1)%maps_x.size();

  double heading = atan2((maps_y[wp2]-maps_y[prev_wp]),(maps_x[wp2]-maps_x[prev_wp]));
  // the x,y,s along the segment
  double seg_s = (s-maps_s[prev_wp]);

  double seg_x = maps_x[prev_wp]+seg_s*cos(heading);
  double seg_y = maps_y[prev_wp]+seg_s*sin(heading);

  double perp_heading = heading-pi()/2;

  double x = seg_x + d*cos(perp_heading);
  double y = seg_y + d*sin(perp_heading);

  return {x,y};

                        }
int count_of_msgs = 0;


// Sort the Lanes by their cost
int SortLanesByCost(Lane lane0, Lane lane1, Lane lane2){
  int bestLane=99;

  vector<Lane> lanes;
  lanes.push_back(lane0);
  lanes.push_back(lane1);
  lanes.push_back(lane2);


  std::sort(lanes.begin(), lanes.end(),
            [](Lane const & a, Lane const & b) -> bool
            { return a.laneCost < b.laneCost; } );

  bestLane = lanes[0].laneNumber;
  //TODO delete cout
//  cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tbestLane: " << bestLane<<endl;
  return bestLane;
}

// output the debug labels in the terminal
void OutputLabels() {
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
  OutputData(labelLeftx, labelEgoVy, "Veh v: ");
  OutputData(labelLeftx, labelEgoCoordxy, "Veh x: ");
  OutputData(labelLeftx, labelEgoCoordyy, "Veh y: ");
  OutputData(labelLeftx, labelEgoMaxvy, "Veh Max V: ");
  OutputData(labelLeftx, labelEgoTgtvy, "Veh Tgt V: ");
  OutputData(labelLeftx, labelEgoReqvy, "Veh Req V: ");
  OutputData(labelLeftx, labelEgoActvy, "Veh Act V: ");


// Lane summary
  OutputData(labelLeftx, labelNoWayPtsy, "No of prev Waypts : ");
  OutputData(labelLeftx, labelLaneTitley, "Lane: ");
  OutputData(data0x, labelLaneTitley, "Lane0: ");
  OutputData(data1x, labelLaneTitley, "Lane1 : ");
  OutputData(data2x, labelLaneTitley, "Lane2 : ");
  OutputData(labelLeftx, labelLaneCnty, "Total Veh Cnt: ");
  OutputData(labelLeftx, labelLaneAhdCnty, "Ahead Veh Cnt: ");
  OutputData(labelLeftx, labelLaneAhdVehSpdy, "Ahead Veh Spd: ");
  OutputData(labelLeftx, labelLaneAhdSy, "Ahead Veh S: ");
  OutputData(labelLeftx, labelLaneBhdVehSpdy, "Behind Veh Spd: ");
  OutputData(labelLeftx, labelLaneBhdSy, "Behind Next Veh S: ");


  OutputData(labelLeftx, labelLaneThreaty, "No Threat Veh: ");
  OutputData(labelLeftx, labelLaneCosty, "Lane Cost: ");

  OutputData(labelLeftx, labelCars_0y, "CARS: ");

  for (int i = 0; i < 6; i++) {
    OutputData(labelTrafCoordsx, labelCar0_0y + i * labelCarOffsety, "veh id: ");
    OutputData(labelTrafCoordsx, labelCar0_1y + i * labelCarOffsety, "veh d: ");
    OutputData(labelTrafCoordsx, labelCar0_2y + i * labelCarOffsety, "veh s: ");
    OutputData(labelTrafCoordsx, labelCar0_3y + i * labelCarOffsety, "veh v: ");
    OutputData(labelTrafCoordsx, labelCar0_4y + i * labelCarOffsety, "veh delta s: ");
    OutputData(labelTrafCoordsx, labelCar0_5y + i * labelCarOffsety, "veh proj s: ");
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
        std::to_string(car.car_id));
    OutputData(datax, labelCar0_1y + i * labelCarOffsety,
        std::to_string(car.car_d)); //,"veh 1 d: ");
    OutputData(datax, labelCar0_2y + i * labelCarOffsety,
        std::to_string(car.car_s)); //,"veh 2 s: ");
    OutputData(datax, labelCar0_3y + i * labelCarOffsety,
        std::to_string(car.getSpeedMph())); //,"veh 3 v: ");
    OutputData(datax, labelCar0_4y + i * labelCarOffsety,
        std::to_string(car.car_delta_s)); //,"veh 4 delta s: ");
    OutputData(datax, labelCar0_5y + i * labelCarOffsety,
        std::to_string(car.car_projected_delta_s)); //,"veh 4 delta s: ");
  }
}


void OutputLaneSummary(Lane& lane0, Lane& lane1, Lane& lane2) {
  OutputData(data0x, labelLaneCnty, std::to_string(lane0.numberTotalCars));
  OutputData(data1x, labelLaneCnty, std::to_string(lane1.numberTotalCars));
  OutputData(data2x, labelLaneCnty, std::to_string(lane2.numberTotalCars));
  OutputData(data0x, labelLaneAhdCnty, std::to_string(lane0.numberAheadCars));
  OutputData(data1x, labelLaneAhdCnty, std::to_string(lane1.numberAheadCars));
  OutputData(data2x, labelLaneAhdCnty, std::to_string(lane2.numberAheadCars));
  OutputData(data0x, labelLaneAhdVehSpdy,
      std::to_string(lane0.nearest_ahead_car.car_speed * 2.24));
  OutputData(data1x, labelLaneAhdVehSpdy,
      std::to_string(lane1.nearest_ahead_car.car_speed * 2.24));
  OutputData(data2x, labelLaneAhdVehSpdy,
      std::to_string(lane2.nearest_ahead_car.car_speed * 2.24));
  OutputData(data0x, labelLaneAhdSy,
      std::to_string(lane0.nearest_ahead_car.car_projected_delta_s));
  OutputData(data1x, labelLaneAhdSy,
      std::to_string(lane1.nearest_ahead_car.car_projected_delta_s));
  OutputData(data2x, labelLaneAhdSy,
      std::to_string(lane2.nearest_ahead_car.car_projected_delta_s));
  OutputData(data0x, labelLaneBhdVehSpdy,
      std::to_string(lane0.nearest_behind_car.car_speed * 2.24));
  OutputData(data1x, labelLaneBhdVehSpdy,
      std::to_string(lane1.nearest_behind_car.car_speed * 2.24));
  OutputData(data2x, labelLaneBhdVehSpdy,
      std::to_string(lane2.nearest_behind_car.car_speed * 2.24));
  OutputData(data0x, labelLaneBhdSy,
      std::to_string(lane0.nearest_behind_car.car_projected_delta_s));
  OutputData(data1x, labelLaneBhdSy,
      std::to_string(lane1.nearest_behind_car.car_projected_delta_s));
  OutputData(data2x, labelLaneBhdSy,
      std::to_string(lane2.nearest_behind_car.car_projected_delta_s));
  OutputData(data0x, labelLaneThreaty, std::to_string(lane0.numberThreatCars));
  OutputData(data1x, labelLaneThreaty, std::to_string(lane1.numberThreatCars));
  OutputData(data2x, labelLaneThreaty, std::to_string(lane2.numberThreatCars));
  OutputData(data0x, labelLaneCosty, std::to_string(lane0.laneCost));
  OutputData(data1x, labelLaneCosty, std::to_string(lane1.laneCost));
  OutputData(data2x, labelLaneCosty, std::to_string(lane2.laneCost));

  if (debug){
    output_traffic_debug(lane0.lane_cars, data0x);
    output_traffic_debug(lane1.lane_cars, data1x);
    output_traffic_debug(lane2.lane_cars, data2x);
  }

}

int main() {
  uWS::Hub h;

  // Load up map values for waypoint's x,y,s and d normalized normal vectors
  vector<double> map_waypoints_x;
  vector<double> map_waypoints_y;
  vector<double> map_waypoints_s;
  vector<double> map_waypoints_dx;
  vector<double> map_waypoints_dy;

  // output the labels used for debug
  OutputLabels();
  //  labelLane0x
  // Waypoint map to read from
  string map_file_ = "../data/highway_map.csv";
  // The max s value before wrapping around the track back to 0
  double max_s = 6945.554;
  ifstream in_map_(map_file_.c_str(), ifstream::in);
  string line;
  while (getline(in_map_, line)) {
    istringstream iss(line);
    double x;
    double y;
    float s;
    float d_x;
    float d_y;
    iss >> x;
    iss >> y;
    iss >> s;
    iss >> d_x;
    iss >> d_y;
    map_waypoints_x.push_back(x);
    map_waypoints_y.push_back(y);
    map_waypoints_s.push_back(s);
    map_waypoints_dx.push_back(d_x);
    map_waypoints_dy.push_back(d_y);
  }
  h.onMessage(
      [&map_waypoints_x, &map_waypoints_y, &map_waypoints_s, &map_waypoints_dx, &map_waypoints_dy](uWS::WebSocket<uWS::SERVER> ws, char* data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    //auto sdata = string(data).substr(0, length);
    //cout << sdata << endl;
    count_of_msgs++;
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(data);
      if (s != "") {
        auto j = json::parse(s);
        string event = j[0].get<string>();
        int no_of_pts = 50;
        if (event == "telemetry") {
          // j[1] is the data JSON object
          // Main car's localization Data
          double ego_car_x = j[1]["x"];
          double ego_car_y = j[1]["y"];
          double ego_car_actual_s = j[1]["s"];
          double ego_car_d = j[1]["d"];
          double ego_car_yaw = j[1]["yaw"];
          double ego_car_speed = j[1]["speed"];

          // Previous path data given to the Planner
          auto previous_path_x = j[1]["previous_path_x"];
          auto previous_path_y = j[1]["previous_path_y"];

          // Previous path's end s and d values
          double end_path_s = j[1]["end_path_s"];
          double end_path_d = j[1]["end_path_d"];

          // Sensor Fusion Data, a list of all other cars on the same side of the road.
          auto sensor_fusion = j[1]["sensor_fusion"];
          json msgJson;

          int prev_size = previous_path_x.size();

          // TODO: define a path made up of (x,y) points that the car will visit sequentially every .02 seconds
          if (prev_size == 0)
          {
            end_path_s = ego_car_actual_s;
          }

          bool too_close = false;
          // TODO make constants at the start
          //          [ id, x, y, vx, vy, s, d]
          int safety_distance = 30;

          double lead_veh_speed = ref_vel;
          int lane0_count = 0;
          int lane1_count = 0;
          int lane2_count = 0;
          int lane0_ahead_count = 0;
          int lane1_ahead_count = 0;
          int lane2_ahead_count = 0;
          vector<Car> traffic;
          vector<Car> lane0_cars;
          vector<Car> lane1_cars;
          vector<Car> lane2_cars;

          // create the lane objects
          Lane lane0;
          Lane lane1;
          Lane lane2;

          // allocate the IDs to each lane
          lane0.laneNumber = lane_num0;
          lane1.laneNumber = lane_num1;
          lane2.laneNumber = lane_num2;

          // parse the sensor fusion data and create cars and laes to represent each element
          for (int i = 0;i < sensor_fusion.size();i++) {
            // retreive teh sensor data
            int sensed_car_id = sensor_fusion[i][SF_ID_IND];
            float sensed_car_d = sensor_fusion[i][SF_D_IND];
            double sensed_car_s = sensor_fusion[i][SF_S_IND];
            double sensed_car_vx = sensor_fusion[i][SF_VX_IND];
            double sensed_car_vy = sensor_fusion[i][SF_VY_IND];

            // calculate so additional data items
            double sensed_car_v = sqrt(sensed_car_vx * sensed_car_vx + sensed_car_vy * sensed_car_vy);
            double delta_s = sensed_car_s - ego_car_actual_s;
            double projected_s = sensed_car_s + ((double)(((prev_size))) * 0.02 * sensed_car_v);
                //calculate the projected delta s
                // should this be form the ego_car_s or the projected_ego_car_s?
                double projected_delta_s = projected_s - end_path_s;
                Car this_car = Car(sensed_car_id, sensed_car_s, sensed_car_d, sensed_car_v, delta_s, projected_s, projected_delta_s);
                //lane 0
                if (sensed_car_d < (2 + 4 * lane_num0 + 2) && sensed_car_d > (2 + 4 * lane_num0 - 2)) {
                  lane0.addCar(this_car);
                } else
                if (sensed_car_d < (2 + 4 * lane_num1 + 2) && sensed_car_d > (2 + 4 * lane_num1 - 2)) {
                  lane1.addCar(this_car);
                } else
                if (sensed_car_d < (2 + 4 * lane_num2 + 2) && sensed_car_d > (2 + 4 * lane_num2 - 2)) {
                  lane2.addCar(this_car);
                }
              }

              // evaluate the lanes
              lane0.evaluate();
              lane1.evaluate();
              lane2.evaluate();
              target_lane = SortLanesByCost(lane0, lane1, lane2);


              //////////////////////////////////////
              // analsye the current lane
              Lane current_lane_obj  = getCurrentLane(ego_car_d, lane0, lane1, lane2 );
//              int current_lane = getCurrentLane(ego_car_d, lane0, lane1, lane2 );
//              if (current_lane == 0) {
//                current_lane_obj = lane0;
//              } else
//              if (current_lane == 1) {
//                current_lane_obj = lane1;
//              } else
//              if (current_lane == 2) {
//                current_lane_obj = lane2;
//              } else {
//              }

              ///////////////////////////////////////////////////////////////////////////////////////////////
              // SPEED
              // check if the ego vehicle is close to the target vehicle
              double ego_target_speed = max_speed;
              if (current_lane_obj.hasAheadCar) {
                //            cout << "car ahead"<< endl;
                Car target_car = current_lane_obj.getNearestAheadCar();
                OutputData(data0x, labelEgoVy, std::to_string(ego_car_speed));
                OutputData(data0x, labelEgoActvy, std::to_string(ego_car_speed));

                if ((target_car.car_projected_delta_s) < safety_distance) {
                  ego_target_speed = target_car.car_speed * 2.24;
                }
              }
              else // no vehicle ahead
              {
                ego_target_speed = max_speed;
              }
              if (ego_car_speed > ego_target_speed) {
                // going to fast, decrease speed
                ego_req_speed -= 0.224;
              } else {
                ego_req_speed += 0.224;
              }
              if (ego_req_speed > max_speed) {
                ego_req_speed = max_speed;
              }


              OutputData(data0x, labelEgoMaxvy, std::to_string(max_speed));
              OutputData(data0x, labelEgoTgtvy, std::to_string(ego_target_speed));
              OutputData(data0x, labelEgoReqvy, std::to_string(ego_req_speed));
              OutputData(data0x, labelEgoActvy, std::to_string(ego_car_speed));
              OutputData(data0x, labelEgoCoordxy, std::to_string(ego_car_x));
              OutputData(data0x, labelEgoCoordyy, std::to_string(ego_car_y));
              OutputData(data0x, labelEgoActsy, std::to_string(ego_car_actual_s));
              OutputData(data0x, labelEgoPthEndsy, std::to_string(end_path_s));
              OutputData(data0x, labelEgoCoorddy, std::to_string(ego_car_d));

              OutputLaneSummary(lane0, lane1, lane2);


              ///////////////////////////////////////////////////////////////////////////////
              // create the new path
          vector<double> ptsx;
          vector<double> ptsy;
          double ref_x = ego_car_x;
          double ref_y = ego_car_y;
          double ref_yaw = deg2rad(ego_car_yaw);


          if (prev_size < 2) {
            double prev_car_x = ego_car_x - cos(ego_car_yaw);
            double prev_car_y = ego_car_y - sin(ego_car_yaw);
            ptsx.push_back(prev_car_x);
            ptsx.push_back(ego_car_x);
            ptsy.push_back(prev_car_y);
            ptsy.push_back(ego_car_y);
          } else // use the previous paths end point as the starting point
          {
            ref_x = previous_path_x[prev_size-1];
            ref_y = previous_path_y[prev_size-1];

            double ref_x_prev = previous_path_x[prev_size-2];
            double ref_y_prev = previous_path_y[prev_size-2];
            ref_yaw = atan2(ref_y - ref_y_prev, ref_x - ref_x_prev);

            ptsx.push_back(ref_x_prev);
            ptsx.push_back(ref_x);

            ptsy.push_back(ref_y_prev);
            ptsy.push_back(ref_y);
          }


          int target_s = 30;
          vector<double> next_wp0 = getXY(end_path_s + target_s, (2+4*target_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
          vector<double> next_wp1 = getXY(end_path_s + target_s *2 , (2+4*target_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);
          vector<double> next_wp2 = getXY(end_path_s + target_s *3, (2+4*target_lane), map_waypoints_s, map_waypoints_x, map_waypoints_y);


          // push the new points onto the next waypoints vectors
          ptsx.push_back(next_wp0[0]);
          ptsx.push_back(next_wp1[0]);
          ptsx.push_back(next_wp2[0]);

          ptsy.push_back(next_wp0[1]);
          ptsy.push_back(next_wp1[1]);
          ptsy.push_back(next_wp2[1]);

          //iterate through the points, and convert to vehicle coordinates
          for(int i = 0; i < ptsx.size(); i++)
          {

            double shift_x = ptsx[i] - ref_x;
            double shift_y = ptsy[i] - ref_y;

            ptsx[i] = (shift_x * cos(0-ref_yaw)-shift_y*sin(0-ref_yaw));
            ptsy[i] = (shift_x * sin(0-ref_yaw)+shift_y*cos(0-ref_yaw));

          }

          tk::spline s;
          s.set_points(ptsx, ptsy);

          vector<double> next_x_vals;
          vector<double> next_y_vals;

          for(int i = 0; i< previous_path_x.size(); i++)
          {
            next_x_vals.push_back(previous_path_x[i]);
            next_y_vals.push_back(previous_path_y[i]);

          }


          double target_x = 30.0;
          double target_y = s(target_x);
          double target_dist = sqrt(target_x * target_x + target_y * target_y);

          double x_add_on = 0;

          // fill up the rest of the path planner, appending to the original points. Always output 50 points
          for (int i = 1; i <= no_of_pts - previous_path_x.size(); i++) {
            double N = (target_dist / (0.02 * ego_req_speed / 2.24));
            double x_point = x_add_on+(target_x) / N;
            double y_point = s(x_point);
            x_add_on = x_point;
            double x_ref = x_point;
            double y_ref = y_point;
            x_point = (x_ref * cos(ref_yaw) - y_ref * sin(ref_yaw));
            y_point = (x_ref * sin(ref_yaw) + y_ref * cos(ref_yaw));

            x_point += ref_x;
            y_point += ref_y;


            next_x_vals.push_back(x_point);
            next_y_vals.push_back(y_point);
          }


          msgJson["next_x"] = next_x_vals;
          msgJson["next_y"] = next_y_vals;

          auto msg = "42[\"control\","+ msgJson.dump()+"]";

          //this_thread::sleep_for(chrono::milliseconds(1000));
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the
  // program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data,
      size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1) {
      res->end(s.data(), s.length());
    } else {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code,
      char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
