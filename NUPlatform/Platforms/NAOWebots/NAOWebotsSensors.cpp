/*! @file NAOWebotsSensors.cpp
    @brief Implementation of NAO in Webots sensor class

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

#include "NAOWebotsSensors.h"
#include "Tools/debug.h"

// Apparently the best way to initialise a vector like an array, is to initialise the vector from an array

// init m_servo_names:
static string temp_servo_names[] = {string("HeadYaw"), string("HeadPitch"), \
                                    string("LShoulderPitch"), string("LShoulderRoll"), string("LElbowYaw"), string("LElbowRoll"), \
                                    string("RShoulderPitch"), string("RShoulderRoll"), string("RElbowYaw"), string("RElbowRoll"), \
                                    string("LHipYawPitch"), string("LHipPitch"), string("LHipRoll"), string("LKneePitch"), string("LAnklePitch"), string("LAnkleRoll"), \
                                    string("RHipYawPitch"), string("RHipPitch"), string("RHipRoll"), string("RKneePitch"), string("RAnklePitch"), string("RAnkleRoll")};
vector<string> NAOWebotsSensors::m_servo_names(temp_servo_names, temp_servo_names + sizeof(temp_servo_names)/sizeof(*temp_servo_names));

// init m_distance_names:
static string temp_distance_names[] = {string("US/TopLeft"), string("US/BottomLeft"), string("US/TopRight"), string("US/BottomRight")};
vector<string> NAOWebotsSensors::m_distance_names(temp_distance_names, temp_distance_names + sizeof(temp_distance_names)/sizeof(*temp_distance_names));

// init m_foot_names:
static string temp_foot_sole_names[] = {string("LFsrFL"), string("LFsrFR"), string("LFsrBR"), string("LFsrBL"), \
                                    string("RFsrFL"), string("RFsrFR"), string("RFsrBR"), string("RFsrBL")};
vector<string> NAOWebotsSensors::m_foot_sole_names(temp_foot_sole_names, temp_foot_sole_names + sizeof(temp_foot_sole_names)/sizeof(*temp_foot_sole_names));

// init m_button_names:
static string temp_foot_bumper_names[] = {string("LFoot/Bumper/Left"), string("LFoot/Bumper/Right"), string("RFoot/Bumper/Left"), string("RFoot/Bumper/Right")};
vector<string> NAOWebotsSensors::m_foot_bumper_names(temp_foot_bumper_names, temp_foot_bumper_names + sizeof(temp_foot_bumper_names)/sizeof(*temp_foot_bumper_names));

/*! @brief Constructs a nubot sensor class with Webots backend
 */
NAOWebotsSensors::NAOWebotsSensors(NAOWebotsPlatform* platform)
{
#if DEBUG_NUSENSORS_VERBOSITY > 4
    debug << "NAOWebotsSensors::NAOWebotsSensors()" << endl;
#endif
    m_platform = platform;
    getSensorsFromWebots(platform);
    enableSensorsInWebots();
}

/* Gets pointers to each of the sensors in the simulated NAO
 */
void NAOWebotsSensors::getSensorsFromWebots(NAOWebotsPlatform* platform)
{
    // Get the servos
    for (int i=0; i<m_servo_names.size(); i++)
        m_servos.push_back(platform->getServo(m_servo_names[i]));
    // Get the accelerometer
    m_accelerometer = platform->getAccelerometer("accelerometer");
    // Get gyro
    m_gyro = platform->getGyro("gyro");
    // Get distance sensors
    for (int i=0; i<m_distance_names.size(); i++)
        m_distance_sensors.push_back(platform->getDistanceSensor(m_distance_names[i]));
    // Get foot sole sensors
    for (int i=0; i<m_foot_sole_names.size(); i++)
        m_foot_sole_sensors.push_back(platform->getTouchSensor(m_foot_sole_names[i]));
    // Get foor bumper sensors
    for (int i=0; i<m_foot_bumper_names.size(); i++)
        m_foot_bumper_sensors.push_back(platform->getTouchSensor(m_foot_bumper_names[i]));
    
    // Get the gps if avaliable
    if (GPS::exists("gps"))
        m_gps = platform->getGPS("gps");
    else
        m_gps = NULL;
}

/* Enables the sensor feedback in the simulated NAO
 */
void NAOWebotsSensors::enableSensorsInWebots()
{
    // Enable the position and torque feedback
    for (int i=0; i<m_servos.size(); i++)
    {
        m_servos[i]->enablePosition(m_simulation_step);
        m_servos[i]->enableMotorForceFeedback(m_simulation_step);
    }
    // Enable the accelerometer
    m_accelerometer->enable(m_simulation_step);
    // Enable the  gyro
    m_gyro->enable(m_simulation_step);
    // Enable the distance sensors
    for (int i=0; i<m_distance_sensors.size(); i++)
        m_distance_sensors[i]->enable(m_simulation_step);
    // Enable the foot sole sensors
    for (int i=0; i<m_foot_sole_sensors.size(); i++)
        m_foot_sole_sensors[i]->enable(m_simulation_step);
    // Enable the foor bumper sensors
    for (int i=0; i<m_foot_bumper_sensors.size(); i++)
        m_foot_bumper_sensors[i]->enable(m_simulation_step);
    
    // Enable the the gps if avaliable
    if (m_gps != NULL)
        m_gps->enable(m_simulation_step);
}

/*! @brief Destructor for NAOWebotsSensors
 */
NAOWebotsSensors::~NAOWebotsSensors()
{
}

/*! @brief Gets the sensor data using the Webots API and puts it in the NUSensorsData data member.
 */
void NAOWebotsSensors::copyFromHardwareCommunications()
{
#if DEBUG_NUSENSORS_VERBOSITY > 4
    debug << "NAOWebotsSensors::copyFromHardwareCommunications()" << endl;
#endif
    static vector<float> positiondata(m_servos.size(), 0);
    for (int i=0; i<m_servos.size(); i++)
        positiondata[i] = m_servos[i]->getPosition();
    m_data->JointPositions->setData(m_platform->system->getPosixTimeStamp(), positiondata);
}




