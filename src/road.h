/*
 * road.h
 *
 *  Created on: 4 Mar 2018
 *      Author: george
 */

#ifndef ROAD_H_
#define ROAD_H_

class Road {
public:
  Road();
  virtual ~Road();

  Vector<lane> lanes;
};

#endif /* ROAD_H_ */
