#include<ros/ros.h>
#include<std_msgs/Int32.h>
#include<geometry_msgs/Twist.h>
#include<sensor_msgs/LaserScan.h>

using namespace ros;
geometry_msgs::Twist var;

void callbackFunction(const sensor_msgs::LaserScan::ConstPtr& msg){
    int length = 720;
    ROS_INFO("%f",msg->ranges[length/2]);
    if (msg->ranges[length/2]>1){
        var.linear.x=0.1;
        var.angular.z=0.0;
        if(msg->ranges[length - length/4] < 0.4){
            var.linear.x=-0.1;
            var.angular.z=-0.2;
        }
        else if(msg->ranges[length/4] < 0.4){
            var.linear.x=-0.1;
            var.angular.z=0.2;
        }
    }
    else if (msg->ranges[length/2]<1){
        var.linear.x=0.0;
        if (msg->ranges[length/2]<0.7){
            var.linear.x=-0.1;
        }
        else if (msg->ranges[length - length/4] < msg->ranges[length/4])
            var.angular.z=-0.3;
        else
            var.angular.z=0.3;
    }
}

int main(int argc, char**argv){
    init(argc,argv,"publisher");
    NodeHandle nh;
    Publisher pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1000);
    Subscriber sub;
    Rate loop_rate(2);
    
    var.linear.x=0.1;
    
    sub=nh.subscribe("/kobuki/laser/scan",1000,callbackFunction);
    
    while(ok()){
        pub.publish(var);
        spinOnce();
        loop_rate.sleep();
    }
    return 0;
}