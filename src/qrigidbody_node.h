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

#ifndef QRIGIDBODY_NODE_H
#define QRIGIDBODY_NODE_H

#include "qbody_node.h"
#include "QuarkPhysics/qrigidbody.h"

class QRigidBodyNode : public QBodyNode {
    GDCLASS(QRigidBodyNode,QBodyNode);
protected:
    
    static void _bind_methods();
public:
    QRigidBodyNode(){
        bodyObject=new QRigidBody();
        bodyObject->manualDeletion=true;
        bodyNodeType=QBodyNode::bodyNodeTypes::RIGID;
        init_events();
    };
    ~QRigidBodyNode(){};

    

    //Get Methods
    bool get_fixed_rotation_enabled();
    bool get_kinematic_enabled();
    bool get_kinematic_collisions_enabled();
    Vector2 get_force();
    float get_angular_force();
    //Set Methods
    QRigidBodyNode *set_fixed_rotation_enabled(bool value);
    QRigidBodyNode *set_kinematic_enabled(bool value);
    QRigidBodyNode *set_kinematic_collisions_enabled(bool value);

    QRigidBodyNode *set_body_position_and_collide(Vector2 value, bool with_previous_position=true);
    QRigidBodyNode *apply_force(Vector2 value,Vector2 r, bool update_mesh_transforms=true);
    QRigidBodyNode *apply_impulse(Vector2 value,Vector2 r);
    QRigidBodyNode *set_force(Vector2 value);

    QRigidBodyNode *add_force(Vector2 value);

    QRigidBodyNode *set_angular_force(float value);

    QRigidBodyNode *add_angular_force(float value);

    

};




#endif // QRIGIDBODY_NODE_H
