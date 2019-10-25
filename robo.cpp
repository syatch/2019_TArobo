#include <stdio.h>
#include <vector>
#include <stdbool.h>
#include <iostream>
#include <math.h>

typedef struct ball_point{
    double x;
    double y;
}ball_point;

typedef struct data{
    // now coordinate of x (cm)
    double x;
    // now coordinate of y (cm)
    double y;
    // left lngth (cm)
    double left_length;
    // move direction when go straight
    int direction;
    // start angle when go straight (radius)
    double start_theta;
    // end angle when go straight (radius)
    double end_theta;
}data;

const int UP = 1;
const int DOWN = 0;
const bool BALL = true;
const bool ROBO = false;

const double correct_degree = 5;

// road length(cm)
double road_length = 0; 

// radius of circle (cm)
const double circle_radius = 40;

// distance of pole to pole (cm)
const double foward_length = 290;

// correct distance
const double correct_distance = foward_length / 4;

// distance from robot's center to ball's robot
const double arm_distance = 35.75;

// one cycle time(sec)
const double cycle_time = 20;

// one step time(sec)
const double step_time = 0.1;

// one step length
double step_length = 0;

// one step degree
double step_degree = 0;

// count step num
int count = 0;
        
std::vector <ball_point> ball_points;
std::vector <double> robo_degrees;

void calc_roadlength ()
{
    road_length += 2 * M_PI * circle_radius;
    //printf("foward length = %f\n", road_length);
    road_length += 2 * foward_length;
    //printf("foward length = %f\n", 2 * foward_length);
    //printf("road_length = %f\n", road_length);
    step_length = road_length / (cycle_time / step_time);
    //printf("num of step = %f\n", cycle_time / step_time);
    //printf("step length = %f\n", step_length);
    step_degree = step_length / (2 * M_PI * circle_radius) * 2 * M_PI;
}

double go_straight (data &data)
{
    double add_length = data.left_length;
    double theta;
    data.x = pow(-1, data.direction) * circle_radius;
    data.y += pow(-1, data.direction + 1) * data.left_length;
    while (1) {
        ball_point p;
        p.x = data.x;
        p.y = data.y;
        ball_points.push_back(p);
        theta = pow(-1, data.direction) * (M_PI / 2);
        if (add_length < correct_distance) {
            theta = theta * add_length / correct_distance + data.start_theta * (correct_distance - add_length) / correct_distance;
        } else if (add_length > foward_length - correct_distance) {
            theta = theta * (foward_length - add_length) / correct_distance + data.end_theta * (correct_distance - (foward_length - add_length)) / correct_distance;
        }
        robo_degrees.push_back(theta);
        count++;
        if (add_length + step_length > foward_length)
            break;
        data.y += pow(-1, data.direction + 1) * step_length;
        add_length += step_length;
    }
    data.left_length = add_length + step_length - foward_length;
    return add_length + step_length - foward_length;
}

int go_around (data &data)
{
    double add_degree = data.left_length / (2 * M_PI * circle_radius) * 2 * M_PI;
    double degree = data.direction * M_PI + pow(-1, data.direction) * data.left_length / (2 * M_PI * circle_radius) * 2 * M_PI;
    double theta;
    data.x = circle_radius * cos(degree);
    data.y = circle_radius * sin(degree) + data.direction * foward_length;
    while (1) {
        ball_point p;
        p.x = data.x;
        p.y = data.y;
        ball_points.push_back(p);
        theta = degree;
        theta += correct_degree / 180 * M_PI;
        robo_degrees.push_back(theta);
        count++;
        if (add_degree + step_degree > M_PI)
            break;
        degree -= step_degree;
        add_degree += step_degree;
        data.x = circle_radius * cos(degree);
        data.y = circle_radius * sin(degree) + data.direction * foward_length;
    }
    data.left_length = circle_radius * (add_degree + step_degree - M_PI);
    data.direction = !data.direction;
    data.start_theta = theta;
    data.end_theta = M_PI * data.direction + correct_degree / 180 * M_PI;
    return circle_radius * (add_degree + step_degree - M_PI);
}

int main ()
{
    bool MODE = ROBO;
    calc_roadlength();
    data data;
    data.x = -circle_radius;
    data.y = 0;
    data.left_length = 0;
    data.direction = UP;
    data.start_theta = -M_PI / 2;
    data.end_theta = -M_PI + correct_degree / 180 * M_PI;

    // one loop set
    go_straight(data);
    go_around(data);
    go_straight(data);
    go_around(data);

    if (MODE) { // if BALL, print only ball point
        for (int i = 0; i < ball_points.size(); i++)
           printf("%f, %f,\n", ball_points[i].x, ball_points[i].y);
    } else { // if ROBO, print robo's coordinate and arm's angle
        std::cout << "double params["<< ball_points.size() <<"][3] = {" << std::endl;
        for (int i = 0; i < ball_points.size(); i++) {
            double x = ball_points[i].x + arm_distance * cos(robo_degrees[i]);
            double y = ball_points[i].y + arm_distance * sin(robo_degrees[i]);
            //std::cout << x <<", " << y << ","<< std::endl;
            std::cout << "  {" << x <<", " << y << ", " << robo_degrees[i] + M_PI << "}," << std::endl;
        }
        std::cout << "};" << std::endl;
    }
    return 0;
}
