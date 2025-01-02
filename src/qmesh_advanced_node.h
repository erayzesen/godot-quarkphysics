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

#ifndef QMESH_ADVANVED_NODE_H
#define QMESH_ADVANVED_NODE_H

#include "qmesh_node.h"

class QMeshAdvancedNode: public QMeshNode {
    GDCLASS(QMeshAdvancedNode,QMeshNode);
private:
    String importFilePath="";
    void set_import_file_path(String value);
    String get_import_file_path();

    String exportFilePath="";
    void set_export_file_path(String value);
    String get_export_file_path();

    int meshIndex=0;
    void set_mesh_index(int value);
    int get_mesh_index();

    
    

protected:
    
    static void _bind_methods();
    void update_mesh_node_data();
public:
    QMeshAdvancedNode(){
        useMeshData=false;
    };
    ~QMeshAdvancedNode(){

    };

};

#endif // QMESH_ADVANVED_NODE_H










