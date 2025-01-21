#include "register_types.h"

#include "qworld_node.h"
#include "qbody_node.h"
#include "qrigidbody_node.h"
#include "qsoftbody_node.h"
#include "qareabody_node.h"
#include "qmesh_node.h"
#include "qmesh_rect_node.h"
#include "qmesh_polygon_node.h"
#include "qmesh_circle_node.h"
#include "qmesh_advanced_node.h"
#include "qparticle_object.h"
#include "qspring_object.h"
#include "qjoint_object.h"
#include "qraycast_object.h"
#include "qplatformerbody_node.h"


#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>


using namespace godot;

void initialize_quarkphysics_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    GDREGISTER_VIRTUAL_CLASS(QBodyNode);
    GDREGISTER_CLASS( QRigidBodyNode) ;
    GDREGISTER_CLASS( QSoftBodyNode) ;
    GDREGISTER_CLASS( QAreaBodyNode) ;
    GDREGISTER_CLASS( QPlatformerBodyNode) ;
    GDREGISTER_CLASS( QWorldNode) ;
    GDREGISTER_CLASS( QMeshNode) ;
    GDREGISTER_CLASS( QMeshRectNode) ;
    GDREGISTER_CLASS( QMeshPolygonNode) ;
    GDREGISTER_CLASS( QMeshCircleNode) ;
    GDREGISTER_CLASS( QMeshAdvancedNode) ;
    GDREGISTER_CLASS( QParticleObject) ;
    GDREGISTER_CLASS( QSpringObject) ;
    GDREGISTER_CLASS( QJointObject) ;
    GDREGISTER_CLASS( QRaycastObject) ;
}

void uninitialize_quarkphysics_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT quarkphysics_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
    
	init_obj.register_initializer(initialize_quarkphysics_module);
	init_obj.register_terminator(uninitialize_quarkphysics_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SERVERS);

	return init_obj.init();
}
}


