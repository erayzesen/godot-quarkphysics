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

#ifndef QPARTICLE_OBJECT_H
#define QPARTICLE_OBJECT_H

#include "QuarkPhysics/qparticle.h"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
using namespace godot;

class QMeshNode;
class QSpringObject;
class QParticleObject: public RefCounted{
    GDCLASS(QParticleObject,RefCounted)
protected:
    static void _bind_methods();
    QParticle *particleObject;
private:
    QMeshNode *ownerMeshNode;
public:
    QParticleObject(){
        particleObject=new QParticle();
    };
    QParticleObject(QParticle *obj){
        particleObject=obj;
    };
    Ref<QParticleObject> configure(Vector2 position,float radius){
        set_position(position);
        set_radius(radius);
        return Ref<QParticleObject>(this);
    };
    
    ~QParticleObject(){
        if(particleObject!=nullptr){
            delete particleObject;
            particleObject=nullptr;
        }
    };

   

    //Get Methods
    Vector2 get_global_position();
    Vector2 get_previous_global_position();
    Vector2 get_position();
    float get_mass();
    QMeshNode* get_owner_mesh_node();
    float get_radius();
    bool get_is_internal();
    Vector2 get_force();

    //Set Methods
    Ref<QParticleObject> set_global_position(Vector2 value);
    Ref<QParticleObject> add_global_position(Vector2 value);
    Ref<QParticleObject> set_previous_global_position(Vector2 value);
    Ref<QParticleObject> add_previous_global_position(Vector2 value);
    Ref<QParticleObject> set_position(Vector2 value);
    Ref<QParticleObject> add_position(Vector2 value);
    Ref<QParticleObject> set_mass(float value);
    Ref<QParticleObject> set_radius(float value);
    Ref<QParticleObject> set_is_internal(bool value);
    Ref<QParticleObject> apply_force(Vector2 value);
    Ref<QParticleObject> set_force(Vector2 value);
    Ref<QParticleObject> add_force(Vector2 value);

    friend class QSpringObject;
    friend class QMeshNode;
    friend class QRenderer;



};




#endif // QPARTICLE_OBJECT_H
