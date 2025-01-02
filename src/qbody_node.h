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

#ifndef QBODY_NODE_H
#define QBODY_NODE_H

#include <godot_cpp/classes/node2d.hpp>
#include "QuarkPhysics/qbody.h"
#include "qmesh_node.h"

class QWorldNode;
class QJointObject;
class QBodyNode: public Node2D{
    GDCLASS(QBodyNode,Node2D);
public:
    enum Modes{
        DYNAMIC,
        STATIC,
    };

    enum bodyNodeTypes{
        RIGID,
        AREA,
        SOFT,
    };

    
protected:
    void _notification(int what);
    void on_post_enter_tree();
    bool isConfigured=false;
    QBody *bodyObject=nullptr;
    QWorldNode *worldNode=nullptr;
    vector<QMeshNode*> meshNodes;
    static void _bind_methods();
    bodyNodeTypes bodyNodeType;
    bool enabled=true;
    

    
public:
    QBodyNode(){
        
    };
    ~QBodyNode(){
        if(bodyObject!=nullptr){
            delete bodyObject;
            bodyObject=nullptr;
        }
    };

    virtual void init_events(){
        bodyObject->CollisionEventListener=bind(&QBodyNode::on_collision_callback,this,placeholders::_1,placeholders::_2);
        bodyObject->PreStepEventListener=bind(&QBodyNode::pre_step_callback,this,placeholders::_1);
        bodyObject->StepEventListener=bind(&QBodyNode::step_callback,this,placeholders::_1);
    };
    

    //Get Methods

    QWorldNode *get_world_node();

    Vector2 get_body_position();

    Vector2 get_body_previous_position();

    float get_body_rotation();

    float get_body_previous_rotation();

    float get_body_rotation_degree();

    Rect2 get_aabb();

    float get_total_initial_area();

    float get_total_polygons_initial_area();

    float get_total_area();

    float get_total_polygons_area();

    Modes get_mode();

    int get_layers_bit();

    int get_collidable_layers_bit();

    bool get_overlap_with_collidable_layers_bit(int layersBit);

    bool get_overlap_with_layers_bit(int layersBit);

    bool get_is_sleeping();

    bool get_can_sleep();

    float get_friction();

    float get_static_friction();

    float get_air_friction();

    float get_mass();

    float get_restitution();

    bool get_body_specific_time_scale_enabled();

    float get_body_specific_time_scale();

    bool get_enabled();

    float get_velocity_limit();

    bool get_integrated_velocities_enabled();

    //Set Methods

    QBodyNode *set_body_position(Vector2 value,bool with_previous_position=true);

    QBodyNode *add_body_position(Vector2 value);

    QBodyNode *set_body_previous_position(Vector2 value);

    QBodyNode *add_body_previous_position(Vector2 value);

    QBodyNode *set_body_rotation(float angle_radian,bool with_previous_rotation=true);

    QBodyNode *set_body_rotation_degree(float degree,bool with_previous_rotation=true);

    QBodyNode *add_body_rotation(float angle_radian);

    QBodyNode *set_body_previous_rotation(float angle_radian);

    QBodyNode *add_body_previous_rotation(float angle_radian);

    QBodyNode *set_layers_bit(int value);

    QBodyNode *set_collidable_layers_bit(int value);

    QBodyNode *set_can_sleep(bool value);

    QBodyNode *set_mode(Modes value);

    QBodyNode *set_friction(float value);

    QBodyNode *set_static_friction(float value);

    QBodyNode *set_air_friction(float value);

    QBodyNode *set_mass(float value);

    QBodyNode *set_restitution(float value);

    QBodyNode *set_body_spesific_time_scale_enabled(bool value);

    QBodyNode *set_body_spesific_time_scale(float value);

    QBodyNode *set_enabled(bool value);

    QBodyNode *set_velocity_limit(float value);

    QBodyNode *set_integrated_velocities_enabled(bool value);

    //Sleeping

    QBodyNode *wake_up();

    //Meshes

    QBodyNode *add_mesh_node(Object *mesh_node);
    QBodyNode *remove_mesh_node_at(int index);
    QBodyNode *remove_mesh_node(Object *mesh_node);
    QMeshNode *get_mesh_at(int index);
    int get_mesh_count();

    QMeshNode *get_mesh_node_with_object(QMesh *mesh_object);

    static QBodyNode *type_cast(Object *obj);

    void update_meshes_draw();


    //Events 

    bool on_collision_callback(QBody *body,QBody::CollisionInfo info);

    void pre_step_callback(QBody* body);


    void step_callback(QBody* body);

    
    friend class QWorldNode;
    friend class QJointObject;
    friend class QRenderer;
    friend class QMeshNode;

};

VARIANT_ENUM_CAST(QBodyNode::Modes)


#endif // QBODY_NODE_H
