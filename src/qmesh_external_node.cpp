#include "qmesh_external_node.h"
#include "qbody_node.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>



void QMeshExternalNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_file_path"),&QMeshExternalNode::get_file_path );
	ClassDB::bind_method(D_METHOD("get_mesh_index"),&QMeshExternalNode::get_mesh_index );
    

    ClassDB::bind_method(D_METHOD("set_file_path","value"),&QMeshExternalNode::set_file_path );
	ClassDB::bind_method(D_METHOD("set_mesh_index","value"),&QMeshExternalNode::set_mesh_index );


    ADD_PROPERTY( PropertyInfo(Variant::STRING, "file_path",PROPERTY_HINT_FILE,"*.qmesh"),"set_file_path","get_file_path" );
    ADD_PROPERTY( PropertyInfo(Variant::INT, "mesh_index",PROPERTY_HINT_RANGE,"0,100"),"set_mesh_index","get_mesh_index" );
    
}

void QMeshExternalNode::update_mesh_node_data() {
    QMesh::MeshData meshData;
    if(filePath==""){
        queue_redraw();
        return;
    }

    
    auto file=FileAccess::open(filePath,FileAccess::READ);

    if(file==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The file doesn't exist! | filePath: " + filePath + " | QMeshExternalNode::update_mesh_node_data() ");
        return;
    }
    
    string data=file->get_as_text().utf8().get_data();   
    
    
    auto meshDataList=QMesh::GetMeshDatasFromJsonData( data );
    if(meshDataList.size()==0 || meshIndex>=meshDataList.size() ){
        queue_redraw();
        return;
    }
    meshData=meshDataList.at(meshIndex);

    apply_mesh_data_to_mesh_node_data(meshData);
    
    queue_redraw();
}



void QMeshExternalNode::set_file_path(String value) {
    filePath=value;
    update_mesh_node_data();
}

String  QMeshExternalNode::get_file_path() {
    return filePath;
}


void QMeshExternalNode::set_mesh_index(int value) {
    meshIndex=value;
    update_mesh_node_data();
}

int  QMeshExternalNode::get_mesh_index() {
    return meshIndex;
}