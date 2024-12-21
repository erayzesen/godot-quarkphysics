#include "qareabody_node.h"
#include "qworld_node.h"

void QAreaBodyNode::_bind_methods() {
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
