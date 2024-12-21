/************************************************************************************
 * MIT License
 *
 * Copyright (c) 2023 Eray Zesen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * https://github.com/erayzesen/QuarkPhysics
 * https://github.com/erayzesen/Quark-Physics-Godot-Module
 *
**************************************************************************************/

#ifndef QPLATFORMERBODY_NODE_H
#define QPLATFORMERBODY_NODE_H

#include "qrigidbody_node.h"
#include "QuarkPhysics/extensions/qplatformerbody.h"



class QPlatformerBodyNode : public QRigidBodyNode {
    GDCLASS(QPlatformerBodyNode,QRigidBodyNode);
protected:
    static void _bind_methods();

    
public:
    QPlatformerBodyNode(){
        bodyObject=new QPlatformerBody();
        bodyNodeType=QBodyNode::bodyNodeTypes::RIGID;
        init_events();
    };
    ~QPlatformerBodyNode(){};


    //QPlatformerBody Methods

    //Floor

    QPlatformerBodyNode *set_moving_floor_snap_offset(float value);
    float get_moving_floor_snap_offset();

    QPlatformerBodyNode *set_floor_max_angle(float value);
    float get_floor_max_angle();

    QPlatformerBodyNode *set_floor_max_angle_degree(float value);
    float get_floor_max_angle_degree();

    bool get_is_on_floor();

    bool get_is_on_ceiling();

    //Gravity

    QPlatformerBodyNode *set_gravity(Vector2 value);
    Vector2 get_gravity();

    QPlatformerBodyNode *set_gravity_multiplier(float value);
    float get_gravity_multiplier();

    //Walk

    QPlatformerBodyNode *set_walk_speed(float value);
    float get_walk_speed();

    QPlatformerBodyNode *set_walk_aceleration_rate(float value);
    float get_walk_aceleration_rate();

    QPlatformerBodyNode *set_walk_deceleration_rate(float value);
    float get_walk_deceleration_rate();

    QPlatformerBodyNode *walk(int side);

    //Controller Velocities
    QPlatformerBodyNode *set_controller_horizontal_velocity(Vector2 value);
    Vector2 get_controller_horizontal_velocity();

    QPlatformerBodyNode *set_controller_vertical_velocity(Vector2 value);
    Vector2 get_controller_vertical_velocity();

    bool get_is_falling();

    bool get_is_rising();

    //Jump

    QPlatformerBodyNode *set_jump_duration_frame_count(int value);
    int get_jump_duration_frame_count();

    QPlatformerBodyNode *set_jump_gravity_multiplier(float value);
    float get_jump_gravity_multiplier();

    QPlatformerBodyNode *set_jump_fall_gravity_multiplier(float value);
    float get_jump_fall_gravity_multiplier();

    QPlatformerBodyNode *set_max_jump_count(int value);
    int get_max_jump_count();

    QPlatformerBodyNode *jump(float force, bool unconditional=false);

    QPlatformerBodyNode *release_jump();

    bool get_is_jumping();

    bool get_is_jump_released();

    QPlatformerBodyNode *set_specific_platform_layers(int layersBit);
    int get_specific_platform_layers();

    Dictionary get_platform_collisions(Vector2 test_position, Vector2 nearest_on_axis);

    Dictionary get_right_wall(float offset);

    Dictionary get_left_wall(float offset);

    Dictionary get_floor(float offset);

    Dictionary get_ceiling(float offset);




    

};




#endif // QPLATFORMERBODY_NODE_H
