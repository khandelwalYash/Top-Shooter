class Top
{
public:
	Top();
	void draw_top();
	void power_level(float velocity);
	void drawCylinder(float rad);
	int checkBoundary(float x, float z);
	void put_friction();
	void draw_arrow();
	float lattu_x,lattu_y,lattu_z;
	float vel_x,vel_y,vel_z;
	float power_length;
	float shoot,flag;
	float theta;
};