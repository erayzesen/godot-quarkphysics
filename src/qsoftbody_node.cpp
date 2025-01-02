#include "qsoftbody_node.h"



void QSoftBodyNode::_bind_methods() {
	//Get
    ClassDB::bind_method(D_METHOD("get_rigidity"), &QSoftBodyNode::get_rigidity );   
    ClassDB::bind_method(D_METHOD("get_area_preserving_enabled"), &QSoftBodyNode::get_area_preserving_enabled );   
    ClassDB::bind_method(D_METHOD("get_area_preserving_rate"), &QSoftBodyNode::get_area_preserving_rate );   
    ClassDB::bind_method(D_METHOD("get_area_preserving_rigidity"), &QSoftBodyNode::get_area_preserving_rigidity );   
    ClassDB::bind_method(D_METHOD("get_target_preservation_area"), &QSoftBodyNode::get_target_preservation_area );   
    ClassDB::bind_method(D_METHOD("get_self_collision_enabled"), &QSoftBodyNode::get_self_collision_enabled ); 
    ClassDB::bind_method(D_METHOD("get_self_collisions_specified_radius"), &QSoftBodyNode::get_self_collisions_specified_radius );     
    ClassDB::bind_method(D_METHOD("get_passivation_of_internal_springs_enabled"), &QSoftBodyNode::get_passivation_of_internal_springs_enabled );   
    ClassDB::bind_method(D_METHOD("get_shape_matching_enabled"), &QSoftBodyNode::get_shape_matching_enabled );   
    ClassDB::bind_method(D_METHOD("get_shape_matching_rate"), &QSoftBodyNode::get_shape_matching_rate );   
    ClassDB::bind_method(D_METHOD("get_shape_matching_fixed_transform_enabled"), &QSoftBodyNode::get_shape_matching_fixed_transform_enabled );
    ClassDB::bind_method(D_METHOD("get_shape_matching_fixed_position"), &QSoftBodyNode::get_shape_matching_fixed_position );
    ClassDB::bind_method(D_METHOD("get_shape_matching_fixed_rotation"), &QSoftBodyNode::get_shape_matching_fixed_rotation );
    ClassDB::bind_method(D_METHOD("get_particle_spesific_mass"), &QSoftBodyNode::get_particle_spesific_mass );   
    ClassDB::bind_method(D_METHOD("get_particle_spesific_mass_enabled"), &QSoftBodyNode::get_particle_spesific_mass_enabled );   

    //Set
    ClassDB::bind_method(D_METHOD("set_rigidity","value"), &QSoftBodyNode::set_rigidity );   
    ClassDB::bind_method(D_METHOD("set_area_preserving_enabled","value"), &QSoftBodyNode::set_area_preserving_enabled );   
    ClassDB::bind_method(D_METHOD("set_area_preserving_rate","value"), &QSoftBodyNode::set_area_preserving_rate );   
    ClassDB::bind_method(D_METHOD("set_area_preserving_rigidity","value"), &QSoftBodyNode::set_area_preserving_rigidity );   
    ClassDB::bind_method(D_METHOD("set_target_preservation_area","value"), &QSoftBodyNode::set_target_preservation_area );   
    ClassDB::bind_method(D_METHOD("set_self_collision_enabled","value"), &QSoftBodyNode::set_self_collision_enabled );
    ClassDB::bind_method(D_METHOD("set_self_collisions_specified_radius","value"), &QSoftBodyNode::set_self_collisions_specified_radius );   
    ClassDB::bind_method(D_METHOD("set_passivation_of_internal_springs_enabled","value"), &QSoftBodyNode::set_passivation_of_internal_springs_enabled );   
    ClassDB::bind_method(D_METHOD("set_shape_matching_enabled","value"), &QSoftBodyNode::set_shape_matching_enabled );   
    ClassDB::bind_method(D_METHOD("set_shape_matching_rate","value"), &QSoftBodyNode::set_shape_matching_rate );   
    ClassDB::bind_method(D_METHOD("set_shape_matching_fixed_transform_enabled","value"), &QSoftBodyNode::set_shape_matching_fixed_transform_enabled );   
    ClassDB::bind_method(D_METHOD("set_shape_matching_fixed_position","value"), &QSoftBodyNode::set_shape_matching_fixed_position );   
    ClassDB::bind_method(D_METHOD("set_shape_matching_fixed_rotation","value"), &QSoftBodyNode::set_shape_matching_fixed_rotation );   
    ClassDB::bind_method(D_METHOD("set_particle_spesific_mass","value"), &QSoftBodyNode::set_particle_spesific_mass );   
    ClassDB::bind_method(D_METHOD("set_particle_spesific_mass_enabled","value"), &QSoftBodyNode::set_particle_spesific_mass_enabled );   

    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"rigidity"), "set_rigidity","get_rigidity" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"passivation_of_internal_springs"), "set_passivation_of_internal_springs_enabled","get_passivation_of_internal_springs_enabled" );
    ADD_GROUP("Self Collisions","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"self_collision"), "set_self_collision_enabled","get_self_collision_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"self_collision_specified_radius"), "set_self_collisions_specified_radius","get_self_collisions_specified_radius" );
    ADD_GROUP("Area Preserving","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"area_preserving"), "set_area_preserving_enabled","get_area_preserving_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"area_preserving_rate"), "set_area_preserving_rate","get_area_preserving_rate" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"area_preserving_rigidity"), "set_area_preserving_rigidity","get_area_preserving_rigidity" );
    ADD_GROUP("Shape Matching","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"shape_matching"), "set_shape_matching_enabled","get_shape_matching_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"shape_matching_rate"), "set_shape_matching_rate","get_shape_matching_rate" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"shape_matching_fixed_transform"), "set_shape_matching_fixed_transform_enabled","get_shape_matching_fixed_transform_enabled" );
    ADD_GROUP("Particle-Specific Mass","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"particle_spesific_mass"), "set_particle_spesific_mass_enabled","get_particle_spesific_mass_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT,"particle_spesific_mass_value"), "set_particle_spesific_mass","get_particle_spesific_mass" );
}

//GET

float QSoftBodyNode::get_rigidity() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetRigidity();
}

bool QSoftBodyNode::get_area_preserving_enabled() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetAreaPreservingEnabled();
}

float QSoftBodyNode::get_area_preserving_rate() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetAreaPreservingRate();
}

float QSoftBodyNode::get_area_preserving_rigidity() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetAreaPreservingRigidity();
}

float QSoftBodyNode::get_target_preservation_area() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetTargetPreservationArea();
}

bool QSoftBodyNode::get_self_collision_enabled() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetSelfCollisionsEnabled();
}

float QSoftBodyNode::get_self_collisions_specified_radius() {
    QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
	return sb->GetSelfCollisionsSpecifiedRadius();
}

bool QSoftBodyNode::get_passivation_of_internal_springs_enabled() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetPassivationOfInternalSpringsEnabled();
}

bool QSoftBodyNode::get_shape_matching_enabled() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetShapeMatchingEnabled();
}

float QSoftBodyNode::get_shape_matching_rate() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetShapeMatchingRate();
}

bool QSoftBodyNode::get_shape_matching_fixed_transform_enabled() {
    QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
	return sb->GetShapeMatchingFixedTransformEnabled();
}

Vector2 QSoftBodyNode::get_shape_matching_fixed_position() {
    QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
	QVector qvalue=sb->GetShapeMatchingFixedPosition();
	return Vector2(qvalue.x,qvalue.y);
}

float QSoftBodyNode::get_shape_matching_fixed_rotation() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
	return sb->GetShapeMatchingFixedRotation();
}

float QSoftBodyNode::get_particle_spesific_mass() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetParticleSpesificMass();
}

bool QSoftBodyNode::get_particle_spesific_mass_enabled() {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    return sb->GetParticleSpesificMassEnabled();
}

//SET

QSoftBodyNode *QSoftBodyNode::set_rigidity(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetRigidity(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_area_preserving_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetAreaPreservingEnabled(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_area_preserving_rate(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetAreaPreservingRate(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_area_preserving_rigidity(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetAreaPreservingRigidity(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_target_preservation_area(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetTargetPreservationArea(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_self_collision_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetSelfCollisionsEnabled(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_self_collisions_specified_radius(float value) {
    QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetSelfCollisionsSpecifiedRadius(value);
	return this;
}

QSoftBodyNode *QSoftBodyNode::set_passivation_of_internal_springs_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetPassivationOfInternalSpringsEnabled(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_shape_matching_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetShapeMatchingEnabled(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_shape_matching_rate(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetShapeMatchingRate(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_shape_matching_fixed_transform_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetShapeMatchingFixedTransformEnabled(value);
    set_shape_matching_fixed_position(get_global_position() );
    set_shape_matching_fixed_rotation(get_global_rotation() );
    cout<<"shape matching fixed position:"<<get_global_position().x<<","<<get_global_position().y<<endl;
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_shape_matching_fixed_position(Vector2 value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetShapeMatchingFixedPosition(QVector(value.x,value.y) );
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_shape_matching_fixed_rotation(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetShapeMatchingFixedRotation(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_particle_spesific_mass(float value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetParticleSpesificMass(value);
    return this;
}

QSoftBodyNode *QSoftBodyNode::set_particle_spesific_mass_enabled(bool value) {
	QSoftBody *sb=static_cast<QSoftBody*>(bodyObject);
    sb->SetParticleSpesificMassEnabled(value);
    return this;
}
