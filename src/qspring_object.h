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

#ifndef QSPRING_OBJECT_H
#define QSPRING_OBJECT_H



#include "QuarkPhysics/qspring.h"
#include "qparticle_object.h"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class QSpringObject: public RefCounted{
    GDCLASS(QSpringObject,RefCounted);
protected:
    static void _bind_methods();
    QSpring *springObject=nullptr;
    Ref<QParticleObject> pA=nullptr;
    Ref<QParticleObject> pB=nullptr;
public:
    QSpringObject(){
        springObject=new QSpring(nullptr,nullptr,0.0f,false);
    };
    QSpringObject(QSpring *obj){
        springObject=obj;
    };
    ~QSpringObject(){
        if(springObject!=nullptr){
            delete springObject;
            springObject=nullptr;
        }
    };
    Ref<QSpringObject> configure_with_current_distance(Ref<QParticleObject> particle_object_a,Ref<QParticleObject> particle_object_b,bool internal=false){
        if(particle_object_a.is_valid()==false || particle_object_b.is_valid()==false ){
            //print_error ("Quark Physics Error: Invalid particle object type! | QSpringObject.create_with_current_distance() ");
            return this;
        }
        
        set_particle_a(particle_object_a);
        set_particle_b(particle_object_b);
        float length=(particle_object_a->get_global_position()-particle_object_b->get_global_position()).length();
        set_length(length);
        set_is_internal(internal);
        return Ref<QSpringObject>(this);
    };

     Ref<QSpringObject> configure(Ref<QParticleObject> particle_object_a,Ref<QParticleObject> particle_object_b,float length, bool internal=false){
        if(particle_object_a.is_valid()==false || particle_object_b.is_valid()==false ){
            //print_error ("Quark Physics Error: Invalid particle object type! | QSpringObject.create_with_current_distance() ");
            return Ref<QSpringObject>(this);
        }
        set_particle_a(particle_object_a);
        set_particle_b(particle_object_b);
        set_length(length);
        set_is_internal(internal);
        return Ref<QSpringObject>(this);
    };
    

    //Get Methods
    Ref<QParticleObject> get_particle_a();
    Ref<QParticleObject> get_particle_b();
    float get_length();
    bool get_is_internal();
    float get_rigidity();
    bool get_enabled();

    //Set Methods
    Ref<QSpringObject> set_particle_a(Ref<QParticleObject> particle_object);
    Ref<QSpringObject> set_particle_b(Ref<QParticleObject> particle_object);
    Ref<QSpringObject> set_length(float value);
    Ref<QSpringObject> set_is_internal(bool value);
    Ref<QSpringObject> set_rigidity(float value);
    Ref<QSpringObject> set_enabled(bool value);

    friend class QMeshNode;
    friend class QWorldNode;
    friend class QRenderer;
    
    
};




#endif // QSPRING_OBJECT_H
