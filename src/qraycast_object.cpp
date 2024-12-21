#include "qraycast_object.h"
#include "qworld_node.h"

void QRaycastObject::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_position"),&QRaycastObject::get_position );
	ClassDB::bind_method(D_METHOD("get_rotation"),&QRaycastObject::get_rotation );
	ClassDB::bind_method(D_METHOD("get_ray_vector"),&QRaycastObject::get_ray_vector );
	ClassDB::bind_method(D_METHOD("get_containing_bodies_enabled"),&QRaycastObject::get_containing_bodies_enabled );

	ClassDB::bind_method(D_METHOD("set_position","value"),&QRaycastObject::set_position );
	ClassDB::bind_method(D_METHOD("set_rotation","value"),&QRaycastObject::set_rotation );
	ClassDB::bind_method(D_METHOD("set_ray_vector","value"),&QRaycastObject::set_ray_vector );
	ClassDB::bind_method(D_METHOD("set_containing_bodies_enabled","value"),&QRaycastObject::set_containing_bodies_enabled );
	ClassDB::bind_method(D_METHOD("set_collidable_layers_bit","value"),&QRaycastObject::set_collidable_layers_bit );

	ClassDB::bind_method(D_METHOD("configure","position","ray_vector","enable_containing_bodies","collidable_layers"),&QRaycastObject::configure );
	ClassDB::bind_method(D_METHOD("get_contacts"),&QRaycastObject::get_contacts );


}
//Get
Vector2 QRaycastObject::get_position() {
	QVector res=raycastObject->GetPosition();
	return Vector2(res.x,res.y);
}

float QRaycastObject::get_rotation() {
	return raycastObject->GetRotation();
}

Vector2 QRaycastObject::get_ray_vector() {
	QVector res=raycastObject->GetRayVector();
	return Vector2(res.x,res.y);
}

bool QRaycastObject::get_containing_bodies_enabled() {
	return raycastObject->GetEnabledContainingBodies();
}

int QRaycastObject::get_collidable_layers_bit() {
	return raycastObject->GetCollidableLayersBit();
}

//Set

Ref<QRaycastObject> QRaycastObject::set_position(Vector2 value) {
	raycastObject->SetPosition(QVector(value.x,value.y) );
	return Ref<QRaycastObject>(this);
}

Ref<QRaycastObject> QRaycastObject::set_rotation(float value) {
	raycastObject->SetRotation(value);
	return Ref<QRaycastObject>(this);
}

Ref<QRaycastObject> QRaycastObject::set_ray_vector(Vector2 value) {
	raycastObject->SetRayVector(QVector(value.x,value.y) );
	return Ref<QRaycastObject>(this);
}

Ref<QRaycastObject> QRaycastObject::set_containing_bodies_enabled(bool value) {
	raycastObject->SetEnabledContainingBodies(value);
	return Ref<QRaycastObject>(this);
}

Ref<QRaycastObject> QRaycastObject::set_collidable_layers_bit(int value) {
	raycastObject->SetCollidableLayersBit(value);
	return Ref<QRaycastObject>(this);
}

Array QRaycastObject::get_contacts() {
	Array res;
	vector<QRaycast::Contact> *contactList=raycastObject->GetContacts();
	for(int i=0;i<contactList->size();i++){
		QRaycast::Contact oc=contactList->at(i);
		Dictionary c={};
		c["body"]=worldNode->get_body_node_with_object(oc.body);
		c["position"]=Vector2(oc.position.x,oc.position.y);
		c["normal"]=Vector2(oc.normal.x,oc.normal.y);
		c["distance"]=oc.distance;
		res.append(c);
	}
	return res;

}
