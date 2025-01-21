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

#ifndef QAREABODY_NODE_H
#define QAREABODY_NODE_H


#include "qbody_node.h"
#include "QuarkPhysics/qareabody.h"

class QAreaBodyNode : public QBodyNode {
    GDCLASS(QAreaBodyNode,QBodyNode);
protected:
    bool enableGravityFree=false;
    Vector2 linearForceToApply=Vector2(0,0);
    
    static void _bind_methods();
public:
    QAreaBodyNode(){
        QAreaBody *areaBodyObject=new QAreaBody();
        bodyObject=areaBodyObject;
        bodyObject->deleteProtected=true;
        bodyNodeType=QBodyNode::bodyNodeTypes::AREA;
        init_events();
        //Area Body Events
        areaBodyObject->CollisionEnterEventListener=bind(&QAreaBodyNode::collision_enter_call_back,this,placeholders::_1,placeholders::_2);
        areaBodyObject->CollisionExitEventListener=bind(&QAreaBodyNode::collision_exit_call_back,this,placeholders::_1,placeholders::_2);
    };
    ~QAreaBodyNode(){};

    void init_body_object(){
        
    }

    void collision_enter_call_back(QAreaBody *areaBody,QBody *collidingBody);
    void collision_exit_call_back(QAreaBody *areaBody,QBody *collidingBody);

    //Get Methods
    bool get_gravity_free_enabled(){
        return enableGravityFree;
    };
    Vector2 get_linear_force_to_apply(){
        return linearForceToApply;
    };

    //Set Methods
    void set_gravity_free_enabled(bool value);
    void set_linear_force_to_apply(Vector2 value);
    

};

#endif // QAREABODY_NODE_H


