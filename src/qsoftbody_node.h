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

#ifndef QSOFTBODY_NODE_H
#define QSOFTBODY_NODE_H

#include "qbody_node.h"
#include "QuarkPhysics/qsoftbody.h"

class QSoftBodyNode : public QBodyNode {
    GDCLASS(QSoftBodyNode,QBodyNode);
protected:
    static void _bind_methods();
public:
    QSoftBodyNode(){
        bodyObject=new QSoftBody();
        bodyObject->deleteProtected=true;
        bodyNodeType=QBodyNode::bodyNodeTypes::SOFT;
        init_events();
    };
    ~QSoftBodyNode(){};

    

    //Get Methods
    float get_rigidity();
    bool get_area_preserving_enabled();
    float get_area_preserving_rate();
    float get_area_preserving_rigidity();
    float get_target_preservation_area();
    bool get_self_collision_enabled();
    float get_self_collisions_specified_radius();
    bool get_passivation_of_internal_springs_enabled();
    bool get_shape_matching_enabled();
    float get_shape_matching_rate();
    bool get_shape_matching_fixed_transform_enabled();
    Vector2 get_shape_matching_fixed_position();
    float get_shape_matching_fixed_rotation();
    float get_particle_spesific_mass();
    bool get_particle_spesific_mass_enabled();
    
    //Set Methods
    QSoftBodyNode *set_rigidity(float value);
    QSoftBodyNode *set_area_preserving_enabled(bool value);
    QSoftBodyNode *set_area_preserving_rate(float value);
    QSoftBodyNode *set_area_preserving_rigidity(float value);
    QSoftBodyNode *set_target_preservation_area(float value);
    QSoftBodyNode *set_self_collision_enabled(bool value);
    QSoftBodyNode *set_self_collisions_specified_radius(float value);
    QSoftBodyNode *set_passivation_of_internal_springs_enabled(bool value);
    QSoftBodyNode *set_shape_matching_enabled(bool value);
    QSoftBodyNode *set_shape_matching_rate(float value);
    QSoftBodyNode *set_shape_matching_fixed_transform_enabled(bool value);
    QSoftBodyNode *set_shape_matching_fixed_position(Vector2 value);
    QSoftBodyNode *set_shape_matching_fixed_rotation(float value);
    QSoftBodyNode *set_particle_spesific_mass(float value);
    QSoftBodyNode *set_particle_spesific_mass_enabled(bool value);



    

    

};

#endif // QSOFTBODY_NODE_H
