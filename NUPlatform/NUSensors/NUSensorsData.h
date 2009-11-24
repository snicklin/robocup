/*! @file NUSensorsData.h
    @brief Declaration of a sensor class to store sensor data in a platform independent way
    @author Jason Kulk
 
    @class NUSensorsData
    @brief A sensor class to store sensor data in a platform independent way
 
    @author Jason Kulk
 
  Copyright (c) 2009 Jason Kulk
 
    This file is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This file is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with NUbot.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NUSENSORSDATA_H
#define NUSENSORSDATA_H

#include "sensor_t.h"

#include <vector>
#include <string>
using namespace std;

/*! @brief Base sensor storage class
 */
class NUSensorsData
{
public:
    NUSensorsData();
    ~NUSensorsData();
    
private:
public:
    // Proprioception Sensors:
    sensor_t* JointPositions;
    sensor_t* JointVelocities;
    sensor_t* JointAccelerations;
    sensor_t* JointTargets;
    sensor_t* JointStiffnesses;
    sensor_t* JointCurrents;
    sensor_t* JointTorques;
    sensor_t* JointTemperatures;
    
    // Balance Sensors:
    sensor_t* BalanceValues;
    
    // Distance Sensors:
    sensor_t* DistanceValues;
    
    // Foot Pressure Sensors:
    sensor_t* FootSoleValues;
    sensor_t* FootBumperValues;
    
    // Buttons Sensors:
    sensor_t* ButtonValues;
    
    // Battery Sensors:
    sensor_t* BatteryValues;
    
private:
    vector<sensor_t*> data;
};

#endif
