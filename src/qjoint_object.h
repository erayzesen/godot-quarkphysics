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

#ifndef QJOINT_OBJECT_H
#define QJOINT_OBJECT_H



#include "QuarkPhysics/qjoint.h"
#include "qrigidbody_node.h"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/object.hpp>

using namespace godot;



class QJointObject: public RefCounted{
    GDCLASS(QJointObject,RefCounted);
protected:
    static void _bind_methods();
    QJoint *jointObject=nullptr;
    QRigidBodyNode *bodyA=nullptr;
    QRigidBodyNode *bodyB=nullptr;
    
public:
    QJointObject(){
        jointObject=new QJoint(nullptr,QVector(0,0),QVector(0,0),nullptr);
        jointObject->deleteProtected=true;
    };
    ~QJointObject(){
        if(jointObject!=nullptr){
            delete jointObject;
            jointObject=nullptr;
        }
    };
    Ref<QJointObject> configure(Object *rigid_body_node_a,Vector2 anchor_world_pos_a, Vector2 anchor_world_pos_b, Object *rigid_body_node_b,float length){
        
        set_body_node_a(rigid_body_node_a);
        set_body_node_b(rigid_body_node_b);
        
        set_anchor_position_a(anchor_world_pos_a);
        set_anchor_position_b(anchor_world_pos_b);

        set_length(length);
        return Ref<QJointObject>(this);
        
        
    };

    Ref<QJointObject> configure_with_current_distance(Object *rigid_body_node_a,Vector2 anchor_world_pos_a, Vector2 anchor_world_pos_b, Object *rigid_body_node_b){
        
        set_body_node_a(rigid_body_node_a);
        set_body_node_b(rigid_body_node_b);
        
        set_anchor_position_a(anchor_world_pos_a);
        set_anchor_position_b(anchor_world_pos_b);

        set_length( (anchor_world_pos_b-anchor_world_pos_a).length() );
        return Ref<QJointObject>(this);
        
    };

    Ref<QJointObject> configure_with_common_anchor_position(Object *rigid_body_node_a,Vector2 common_anchor_world_pos, Object *rigid_body_node_b){
        
        set_body_node_a(rigid_body_node_a);
        set_body_node_b(rigid_body_node_b);
        
        set_anchor_position_a(common_anchor_world_pos);
        set_anchor_position_b(common_anchor_world_pos);
        
        return Ref<QJointObject>(this);
    };

    

    //Get Methods
    QRigidBodyNode *get_body_node_a();
    QRigidBodyNode *get_body_node_b();
    Vector2 get_anchor_position_a();
    Vector2 get_anchor_position_b();
    Vector2 get_anchor_global_position_a();
    Vector2 get_anchor_global_position_b();
    bool get_collisions_enabled();
    bool get_groove_enabled();
    float get_length();
    float get_balance();
    float get_rigidity();
    bool get_enabled();

    //Set Methods
    Ref<QJointObject> set_body_node_a(Object *rigid_body_node);
    Ref<QJointObject> set_body_node_b(Object *rigid_body_node);
    Ref<QJointObject> set_anchor_position_a(Vector2 value);
    Ref<QJointObject> set_anchor_position_b(Vector2 value);
    Ref<QJointObject> set_collision_enabled(bool value);
    Ref<QJointObject> set_groove_enabled(bool value);
    Ref<QJointObject> set_length(float value);
    Ref<QJointObject> set_balance(float value);
    Ref<QJointObject> set_rigidity(float value);
    Ref<QJointObject> set_enabled(bool value);

    friend class QWorldNode;
    
};


#endif // QJOINT_OBJECT_H





