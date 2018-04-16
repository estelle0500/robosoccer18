#ifndef ROBOTPOSE_H
#define ROBOTPOSE_H

class robotPose {
public:
	int x, y, heading;
	robotPose();
	robotPose(int _x, int _y, int _heading);
	void init(int _x, int _y, int _heading);
};

#endif // !ROBOTPOSE_H

