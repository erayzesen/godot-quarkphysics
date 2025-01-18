#include "qareabody_node.h"
#include "qworld_node.h"

void QAreaBodyNode::_bind_methods() {

    ClassDB::bind_method(D_METHOD("get_gravity_free_enabled"), &QAreaBodyNode::get_gravity_free_enabled );
    ClassDB::bind_method(D_METHOD("get_linear_force_to_apply"), &QAreaBodyNode::get_linear_force_to_apply );

    ClassDB::bind_method(D_METHOD("set_gravity_free_enabled","value"), &QAreaBodyNode::set_gravity_free_enabled );
    ClassDB::bind_method(D_METHOD("set_linear_force_to_apply","value"), &QAreaBodyNode::set_linear_force_to_apply );

    ADD_PROPERTY( PropertyInfo(Variant::BOOL,"gravity_free"), "set_gravity_free_enabled","get_gravity_free_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2,"linear_force_to_apply"), "set_linear_force_to_apply","get_linear_force_to_apply" );


    ADD_SIGNAL(MethodInfo("collision_enter",PropertyInfo(Variant::OBJECT,"area_body_node"),PropertyInfo(Variant::OBJECT,"collided_body_node") ) );
    ADD_SIGNAL(MethodInfo("collision_exit",PropertyInfo(Variant::OBJECT,"area_body_node"),PropertyInfo(Variant::OBJECT,"collided_body_node") ) );
}

void QAreaBodyNode::collision_enter_call_back(QAreaBody *areaBody, QBody *collidingBody) {
    QBodyNode *ab_node=worldNode->get_body_node_with_object(areaBody);
    QBodyNode *cb_node=worldNode->get_body_node_with_object(collidingBody);
    emit_signal("collision_enter",ab_node,cb_node);
}

void QAreaBodyNode::collision_exit_call_back(QAreaBody *areaBody, QBody *collidingBody) {
    QBodyNode *ab_node=worldNode->get_body_node_with_object(areaBody);
    QBodyNode *cb_node=worldNode->get_body_node_with_object(collidingBody);
    emit_signal("collision_exit",ab_node,cb_node);
}

void QAreaBodyNode::set_gravity_free_enabled(bool value)
{
    enableGravityFree=value;
    if (Engine::get_singleton()->is_editor_hint()==false && bodyObject!=nullptr ){
        QAreaBody *areaObject=static_cast<QAreaBody*>(bodyObject);
        if(areaObject!=nullptr){
            areaObject->SetGravityFreeEnabled(true);
        }
    }
}

void QAreaBodyNode::set_linear_force_to_apply(Vector2 value)
{
    linearForceToApply=value;
    if (Engine::get_singleton()->is_editor_hint()==false && bodyObject!=nullptr ){
        QAreaBody *areaObject=static_cast<QAreaBody*>(bodyObject);
        if(areaObject!=nullptr){
            areaObject->SetLinearForceToApply(QVector(linearForceToApply.x,linearForceToApply.y) );
        }
    }
}
