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

#ifndef QANGLECONSTRAINT_OBJECT_H
#define QANGLECONSTRAINT_OBJECT_H



#include "QuarkPhysics/qangleconstraint.h"
#include "qparticle_object.h"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class QAngleConstraintObject: public RefCounted{
    GDCLASS(QAngleConstraintObject,RefCounted);
protected:
    static void _bind_methods();
    QAngleConstraint *angleConstraintObject=nullptr;
    Ref<QParticleObject> pA=nullptr;
    Ref<QParticleObject> pB=nullptr;
    Ref<QParticleObject> pC=nullptr;
public:
    QAngleConstraintObject(){
        angleConstraintObject=new QAngleConstraint(nullptr,nullptr,nullptr);
        angleConstraintObject->manualDeletion=true;
    };
    QAngleConstraintObject(QAngleConstraint *obj){
        angleConstraintObject=obj;
        angleConstraintObject->manualDeletion=true;
    };
    ~QAngleConstraintObject(){
        if(angleConstraintObject!=nullptr){
            delete angleConstraintObject;
            angleConstraintObject=nullptr;
        }
    };
    Ref<QAngleConstraintObject> configure_with_angle_of_local_positions(Ref<QParticleObject> particle_object_a,Ref<QParticleObject> particle_object_b,Ref<QParticleObject> particle_object_c,float angleRange=0.1f){
        if(particle_object_a.is_valid()==false || particle_object_b.is_valid()==false || particle_object_c.is_valid()==false ){
            //print_error ("Quark Physics Error: Invalid particle object type! | QAngleConstraintObject.configure_with_angle_of_local_positions() ");
            return Ref<QAngleConstraintObject>(this);
        }

        
        set_particle_a(particle_object_a);
        set_particle_b(particle_object_b);
        set_particle_c(particle_object_c);
        float angle=QAngleConstraint::GetAngleOfParticlesWithLocalPositions(particle_object_a->particleObject,particle_object_b->particleObject,particle_object_c->particleObject);
        set_min_angle(angle-angleRange);
        set_max_angle(angle+angleRange);
        
        return Ref<QAngleConstraintObject>(this);
    };

     Ref<QAngleConstraintObject> configure(Ref<QParticleObject> particle_object_a,Ref<QParticleObject> particle_object_b,Ref<QParticleObject> particle_object_c,float minimum_angle,float maximum_angle){
        if(particle_object_a.is_valid()==false || particle_object_b.is_valid()==false ||  particle_object_c.is_valid()==false ){
            //print_error ("Quark Physics Error: Invalid particle object type! | QAngleConstraintObject.configure() ");
            return Ref<QAngleConstraintObject>(this);
        }

        
        set_particle_a(particle_object_a);
        set_particle_b(particle_object_b);
        set_particle_c(particle_object_c);
        set_min_angle(minimum_angle);
        set_max_angle(maximum_angle);
        return Ref<QAngleConstraintObject>(this);
    };
    

    //Get Methods
    Ref<QParticleObject> get_particle_a();
    Ref<QParticleObject> get_particle_b();
    Ref<QParticleObject> get_particle_c();
    float get_min_angle();
    bool get_max_angle();
    float get_rigidity();
    bool get_enabled();
    float get_current_angle();
    static float get_angle_of_particles_with_local_positions(Ref<QParticleObject> particle_object_a,Ref<QParticleObject> particle_object_b,Ref<QParticleObject> particle_object_c);

    //Set Methods
    Ref<QAngleConstraintObject> set_particle_a(Ref<QParticleObject> particle_object);
    Ref<QAngleConstraintObject> set_particle_b(Ref<QParticleObject> particle_object);
    Ref<QAngleConstraintObject> set_particle_c(Ref<QParticleObject> particle_object);
    Ref<QAngleConstraintObject> set_min_angle(float value);
    Ref<QAngleConstraintObject> set_max_angle(float value);
    Ref<QAngleConstraintObject> set_rigidity(float value);
    Ref<QAngleConstraintObject> set_enabled(bool value);

    friend class QMeshNode;
    friend class QWorldNode;
    friend class QRenderer;
    
    
};




#endif // QANGLECONSTRAINT_OBJECT_H
