#pragma once

// for Gazebo data
namespace gz
{
	class realTimeData
	{
	public:
		int isTouch;
		int isTake;
		double fullTime;
		double coeffCopy[6][6];
	};

}
