/*
Copyright (C) 2012 Intel Corporation

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "exampleplugin.h"
#include "timestamp.h"

#include <iostream>
#include <boost/assert.hpp>
#include <glib.h>

using namespace std;

#include "debugout.h"

uint16_t accelerationX = 0;
Transmission::TransmissionPositions transmissionShiftPostion = Transmission::Neutral;
uint16_t steeringWheelAngle=0;
uint16_t throttlePos = 0;
uint16_t engineCoolant = 40;
bool machineGun = false;

static gboolean timeoutCallback(gpointer data)
{
	ExampleSourcePlugin* src = (ExampleSourcePlugin*)data;
	
	src->randomizeProperties();
	
	return true;
}

ExampleSourcePlugin::ExampleSourcePlugin(AbstractRoutingEngine* re, map<string, string> config)
:AbstractSource(re, config), velocity(0), engineSpeed(0)
{
	debugOut("setting timeout");
	g_timeout_add(1000, timeoutCallback, this );

	addPropertySupport(VehicleProperty::EngineSpeed, Zone::None);
	addPropertySupport(VehicleProperty::VehicleSpeed, Zone::None);
	addPropertySupport(VehicleProperty::AccelerationX, Zone::None);
	addPropertySupport(VehicleProperty::TransmissionShiftPosition, Zone::None);
	addPropertySupport(VehicleProperty::SteeringWheelAngle, Zone::None);
	addPropertySupport(VehicleProperty::ThrottlePosition, Zone::None);
	addPropertySupport(VehicleProperty::EngineCoolantTemperature, Zone::None);
	addPropertySupport(VehicleProperty::VIN, Zone::None);
	addPropertySupport(VehicleProperty::WMI, Zone::None);
	addPropertySupport(VehicleProperty::BatteryVoltage, Zone::None);
	addPropertySupport(VehicleProperty::MachineGunTurretStatus, Zone::None);
	addPropertySupport(VehicleProperty::ExteriorBrightness, Zone::None);
	addPropertySupport(VehicleProperty::DoorsPerRow, Zone::None);
	addPropertySupport(VehicleProperty::AirbagStatus, Zone::FrontLeft);

	re->setSupported(supported(), this);
}



extern "C" AbstractSource * create(AbstractRoutingEngine* routingengine, map<string, string> config)
{
	return new ExampleSourcePlugin(routingengine, config);
	
}

string ExampleSourcePlugin::uuid()
{
	return "6dd4268a-c605-4a06-9034-59c1e8344c8e";
}


void ExampleSourcePlugin::getPropertyAsync(AsyncPropertyReply *reply)
{
	DebugOut()<<"ExampleSource: getPropertyAsync called for property: "<<reply->property<<endl;



	if(reply->property == VehicleProperty::VehicleSpeed)
	{
		VehicleProperty::VehicleSpeedType temp(velocity);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::EngineSpeed)
	{
		VehicleProperty::EngineSpeedType temp(engineSpeed);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::AccelerationX)
	{
		VehicleProperty::AccelerationXType temp(accelerationX);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::TransmissionShiftPosition)
	{
		VehicleProperty::TransmissionShiftPositionType temp(transmissionShiftPostion);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::SteeringWheelAngle)
	{
		VehicleProperty::SteeringWheelAngleType temp(steeringWheelAngle);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::VIN)
	{
		VehicleProperty::VINType temp("ABC00000000000000");
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::WMI)
	{
		VehicleProperty::WMIType temp("abc");
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::BatteryVoltage)
	{
		VehicleProperty::BatteryVoltageType temp(12.6);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::ExteriorBrightness)
	{
		VehicleProperty::ExteriorBrightnessType temp(1000);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::DoorsPerRow)
	{
		VehicleProperty::DoorsPerRowType temp;

		BasicPropertyType<uint16_t> row1(2);
		BasicPropertyType<uint16_t> row2(2);
		BasicPropertyType<uint16_t> row3(1);

		temp.append(&row1);
		temp.append(&row2);
		temp.append(&row3);

		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::AirbagStatus)
	{

		if(reply->zoneFilter != Zone::None && reply->zoneFilter != Zone::FrontLeft)
		{
			reply->success = false;
			reply->error = AsyncPropertyReply::ZoneNotSupported;
			reply->completed(reply);
		}

		VehicleProperty::AirbagStatusType temp(Airbag::Active);
		temp.zone = Zone::FrontLeft;

		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
	else if(reply->property == VehicleProperty::MachineGunTurretStatus)
	{
		VehicleProperty::MachineGunTurretStatusType temp(true);
		reply->value = &temp;
		reply->success = true;
		reply->completed(reply);
	}
}

void ExampleSourcePlugin::getRangePropertyAsync(AsyncRangePropertyReply *reply)
{

}

AsyncPropertyReply *ExampleSourcePlugin::setProperty(AsyncSetPropertyRequest request )
{

}

void ExampleSourcePlugin::subscribeToPropertyChanges(VehicleProperty::Property property)
{
	mRequests.push_back(property);
}

PropertyList ExampleSourcePlugin::supported()
{
	return mSupported;
}

int ExampleSourcePlugin::supportedOperations()
{
	return Get | Set | GetRanged;
}

void ExampleSourcePlugin::unsubscribeToPropertyChanges(VehicleProperty::Property property)
{
	mRequests.remove(property);
}

void ExampleSourcePlugin::randomizeProperties()
{
	velocity = 1 + (255.00 * (rand() / (RAND_MAX + 1.0)));
	engineSpeed = 1 + (15000.00 * (rand() / (RAND_MAX + 1.0)));
	accelerationX = 1 + (15000.00 * (rand() / (RAND_MAX + 1.0)));
	transmissionShiftPostion = Transmission::TransmissionPositions(1 + (6.00 * (rand() / (RAND_MAX + 1.0))));
	steeringWheelAngle = 1 + (359.00 * (rand() / (RAND_MAX + 1.0)));
	throttlePos = 1 + (100.00 * (rand() / (RAND_MAX + 1.0)));
	engineCoolant = 1 + (40.00 * (rand() / (RAND_MAX + 140.0)));
	
	DebugOut()<<"setting velocity to: "<<velocity<<endl;
	DebugOut()<<"setting enginespeed to: "<<engineSpeed<<endl;
	
	VehicleProperty::VehicleSpeedType vel(velocity);
	VehicleProperty::EngineSpeedType es(engineSpeed);
	VehicleProperty::AccelerationXType ac(accelerationX);
	VehicleProperty::SteeringWheelAngleType swa(steeringWheelAngle);
	VehicleProperty::TransmissionShiftPositionType tsp(transmissionShiftPostion);
	VehicleProperty::ThrottlePositionType tp(throttlePos);
	VehicleProperty::EngineCoolantTemperatureType ec(engineCoolant);
	VehicleProperty::MachineGunTurretStatusType mgt(machineGun);

	machineGun = !machineGun;

	vel.timestamp = amb::currentTime();
	es.timestamp = amb::currentTime();
	ac.timestamp = amb::currentTime();
	swa.timestamp = amb::currentTime();
	tsp.timestamp = amb::currentTime();
	tp.timestamp = amb::currentTime();
	ec.timestamp = amb::currentTime();

	routingEngine->updateProperty(VehicleProperty::VehicleSpeed, &vel, uuid());
	routingEngine->updateProperty(VehicleProperty::EngineSpeed, &es, uuid());
	routingEngine->updateProperty(VehicleProperty::AccelerationX, &ac, uuid());
	routingEngine->updateProperty(VehicleProperty::SteeringWheelAngle, &swa, uuid());
	routingEngine->updateProperty(VehicleProperty::TransmissionShiftPosition,&tsp, uuid());
	routingEngine->updateProperty(VehicleProperty::ThrottlePosition, &tp, uuid());
	routingEngine->updateProperty(VehicleProperty::EngineCoolantTemperature, &ec, uuid());
	//routingEngine->updateProperty(VehicleProperty::MachineGunTurretStatus, &mgt);
}

void ExampleSourcePlugin::addPropertySupport(VehicleProperty::Property property, Zone::Type zone)
{
	mSupported.push_back(property);

	std::list<Zone::Type> zones;

	zones.push_back(zone);

	PropertyInfo info(0, zones);

	propertyInfoMap[property] = info;
}
