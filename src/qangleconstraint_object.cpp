#include "qspring_object.h"
#include "qangleconstraint_object.h"


void QAngleConstraintObject::_bind_methods() {
    //get
    
    ClassDB::bind_method(D_METHOD("get_particle_a"),&QAngleConstraintObject::get_particle_a );
    ClassDB::bind_method(D_METHOD("get_particle_b"),&QAngleConstraintObject::get_particle_b );
    ClassDB::bind_method(D_METHOD("get_particle_c"),&QAngleConstraintObject::get_particle_c );
    ClassDB::bind_method(D_METHOD("get_min_angle"),&QAngleConstraintObject::get_min_angle );
    ClassDB::bind_method(D_METHOD("get_max_angle"),&QAngleConstraintObject::get_max_angle );
    ClassDB::bind_method(D_METHOD("get_rigidity"),&QAngleConstraintObject::get_rigidity );
    ClassDB::bind_method(D_METHOD("get_enabled"),&QAngleConstraintObject::get_enabled );
    //set
    ClassDB::bind_method(D_METHOD("set_particle_a","particle_object"),&QAngleConstraintObject::set_particle_a );
    ClassDB::bind_method(D_METHOD("set_particle_b","particle_object"),&QAngleConstraintObject::set_particle_b );
    ClassDB::bind_method(D_METHOD("set_particle_c","particle_object"),&QAngleConstraintObject::set_particle_c );
    ClassDB::bind_method(D_METHOD("set_min_angle","value"),&QAngleConstraintObject::set_min_angle );
    ClassDB::bind_method(D_METHOD("set_max_angle","value"),&QAngleConstraintObject::set_max_angle );
    ClassDB::bind_method(D_METHOD("set_rigidity","value"),&QAngleConstraintObject::set_rigidity );
    ClassDB::bind_method(D_METHOD("set_enabled","value"),&QAngleConstraintObject::set_enabled );

    ClassDB::bind_method(D_METHOD("configure","particle_object_a","particle_object_b","particle_object_c","minimum_angle","maximum_angle"),&QAngleConstraintObject::configure );
    ClassDB::bind_method(D_METHOD("configure_with_angle_of_local_positions","particle_object_a","particle_object_b","particle_object_c","angle_range"),&QAngleConstraintObject::configure_with_angle_of_local_positions );
}

Ref<QParticleObject> QAngleConstraintObject::get_particle_a()
{
    return pA;
}

Ref<QParticleObject> QAngleConstraintObject::get_particle_b()
{
    return pB;
}

Ref<QParticleObject> QAngleConstraintObject::get_particle_c()
{
    return pC;
}

float QAngleConstraintObject::get_min_angle()
{
    return angleConstraintObject->GetMinAngle();
}

bool QAngleConstraintObject::get_max_angle()
{
    return angleConstraintObject->GetMaxAngle();
}

float QAngleConstraintObject::get_rigidity()
{
    return angleConstraintObject->GetRigidity();
}

bool QAngleConstraintObject::get_enabled()
{
    return angleConstraintObject->GetEnabled();
}

float QAngleConstraintObject::get_current_angle()
{
    return angleConstraintObject->GetCurrentAngle();
}

float QAngleConstraintObject::get_angle_of_particles_with_local_positions(Ref<QParticleObject> particle_object_a, Ref<QParticleObject> particle_object_b, Ref<QParticleObject> particle_object_c)
{
    
    return QAngleConstraint::GetAngleOfParticlesWithLocalPositions(particle_object_a->particleObject,particle_object_b->particleObject,particle_object_c->particleObject);
}

//SET METHODS
Ref<QAngleConstraintObject> QAngleConstraintObject::set_particle_a(Ref<QParticleObject> particle_object)
{
    pA=particle_object;
    if(particle_object!=nullptr){
        angleConstraintObject->SetParticleA(particle_object->particleObject);
    }else{
        angleConstraintObject->SetParticleA(nullptr);
    }
    return Ref<QAngleConstraintObject>(this);
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_particle_b(Ref<QParticleObject> particle_object)
{
    pB=particle_object;
    if(particle_object!=nullptr){
        angleConstraintObject->SetParticleB(particle_object->particleObject);
    }else{
        angleConstraintObject->SetParticleB(nullptr);
    }
    return Ref<QAngleConstraintObject>(this);
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_particle_c(Ref<QParticleObject> particle_object)
{
    pC=particle_object;
    if(particle_object!=nullptr){
        angleConstraintObject->SetParticleC(particle_object->particleObject);
    }else{
        angleConstraintObject->SetParticleC(nullptr);
    }
    return Ref<QAngleConstraintObject>(this);
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_min_angle(float value)
{
    angleConstraintObject->SetMinAngle(value);
    return Ref<QAngleConstraintObject>(this);
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_max_angle(float value)
{
    angleConstraintObject->SetMaxAngle(value);
    return Ref<QAngleConstraintObject>(this);
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_rigidity(float value)
{
    angleConstraintObject->SetRigidity(value);
    return Ref<QAngleConstraintObject>(this);
    
}

Ref<QAngleConstraintObject> QAngleConstraintObject::set_enabled(bool value)
{
    angleConstraintObject->SetEnabled(value);
    return Ref<QAngleConstraintObject>(this);
}
