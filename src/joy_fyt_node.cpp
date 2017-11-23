#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "cmg_msgs/Signal.h"
#include "fyt_mae/fyt_commons.h"

class Controller {
	private:
		ros::NodeHandle n;
		ros::Subscriber s;
		ros::Publisher p;
		bool state[4];
		const uint8_t signals[4] = {SIG_END, SIG_START, SIG_ALARM, SIG_GOOD};
	public:
		Controller(): n("~") {
			s = n.subscribe("/joy", 10, &Controller::cb, this);
			p = n.advertise<cmg_msgs::Signal>("/mae/signal", 10);
			for (int i = 0; i < 4; i++) {
				state[i] = false;
			}
		}
		
		void cb(const sensor_msgs::Joy::ConstPtr & msg) {
			for (int i = 0; i < 4; i++) {
				if (msg->buttons[i] && !state[i]) {
					cmg_msgs::Signal s;
					s.signal = signals[i];
					p.publish(s);
				}
				state[i] = msg->buttons[i];
			}
		}
};

int main(int argc, char * argv[]) {
	ros::init(argc, argv, "joy_fyt");
	Controller c;
	ros::spin();
}
