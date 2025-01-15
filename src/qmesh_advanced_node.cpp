#include "qmesh_advanced_node.h"
#include "qbody_node.h"
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/json.hpp>



void QMeshAdvancedNode::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_import_file_path"),&QMeshAdvancedNode::get_import_file_path );
	ClassDB::bind_method(D_METHOD("get_export_file_path"),&QMeshAdvancedNode::get_export_file_path );
	//ClassDB::bind_method(D_METHOD("get_mesh_index"),&QMeshAdvancedNode::get_mesh_index );
    

    ClassDB::bind_method(D_METHOD("set_import_file_path","value"),&QMeshAdvancedNode::set_import_file_path );
    ClassDB::bind_method(D_METHOD("set_export_file_path","value"),&QMeshAdvancedNode::set_export_file_path );
	//ClassDB::bind_method(D_METHOD("set_mesh_index","value"),&QMeshAdvancedNode::set_mesh_index );


    ADD_PROPERTY( PropertyInfo(Variant::STRING, "import_qmesh_file",PROPERTY_HINT_FILE,"*.qmesh"),"set_import_file_path","get_import_file_path" );
    ADD_PROPERTY( PropertyInfo(Variant::STRING, "export_qmesh_file",PROPERTY_HINT_SAVE_FILE,"*.qmesh"),"set_export_file_path","get_export_file_path" );
    //ADD_PROPERTY( PropertyInfo(Variant::INT, "mesh_index",PROPERTY_HINT_RANGE,"0,100"),"set_mesh_index","get_mesh_index" );
    
}

void QMeshAdvancedNode::update_mesh_node_data() {
    
    queue_redraw();
}



void QMeshAdvancedNode::set_import_file_path(String value) {

    if (value==""){
        return;
    }

    QMesh::MeshData meshData;

    importFilePath=value;

    auto file=FileAccess::open(importFilePath,FileAccess::READ);

    if(file==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The file doesn't exist! | importFilePath: " + importFilePath + " | QMeshAdvancedNode::set_import_file_path() ");
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

    importFilePath="";
}

String  QMeshAdvancedNode::get_import_file_path() {
    return importFilePath;
}

void QMeshAdvancedNode::set_export_file_path(String value)
{
    if (value==""){
        return;
    }
    exportFilePath=value;
    auto file=FileAccess::open(exportFilePath,FileAccess::WRITE);

    /* if(file==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The file doesn't exist! | exportFilePath: " + exportFilePath + " | QMeshAdvancedNode::set_export_file_path() ");
        return;
    } */

    //Convert Mesh Data to  Dictionarty  
    Dictionary meshDic;
    Dictionary dataDic;
    Array meshDataList;
    dataDic["name"]="QMeshData";
    Array meshPos;
    meshPos.append(0.0f);
    meshPos.append(0.0f);
    dataDic["position"]=meshPos;
    dataDic["rotation"]=0.0f;
    Array particles;
    for (size_t i=0;i<dataParticlePositions.size();++i ){
        Dictionary particleData;
        Array pos;
        pos.append(dataParticlePositions[i].x);
        pos.append(dataParticlePositions[i].y);
        particleData["position"]=pos;
        particleData["radius"]=dataParticleRadius[i];
        particleData["is_internal"]=dataParticleIsInternal[i]==1;
        particleData["enabled"]=dataParticleIsEnabled[i]==1;
        particleData["lazy"]=dataParticleIsLazy[i]==1;
        particles.append(particleData);
    }
    dataDic["particles"]=particles;
    dataDic["springs"]=dataSprings;
    dataDic["internal_springs"]=dataInternalSprings;
    dataDic["polygon"]=dataPolygon;
    dataDic["uv_maps"]=dataUVMaps;

    meshDataList.append(dataDic);

    meshDic["meshes"]=meshDataList;
    
    file->store_string( JSON::stringify(meshDic,"\t") );
    file->close();

    exportFilePath="";


}

String QMeshAdvancedNode::get_export_file_path()
{
    return exportFilePath;
}

void QMeshAdvancedNode::set_mesh_index(int value) {
    meshIndex=value;
    update_mesh_node_data();
}

int  QMeshAdvancedNode::get_mesh_index() {
    return meshIndex;
}


