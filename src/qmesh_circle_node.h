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

#ifndef QMESH_CIRCLE_NODE_H
#define QMESH_CIRCLE_NODE_H

#include "qmesh_node.h"

class QMeshCircleNode: public QMeshNode {
    GDCLASS(QMeshCircleNode,QMeshNode);
private:
    float circleRadius=32.0f;
    void set_circle_radius(float value);
    float get_circle_radius();
    
    Vector2 centerPosition=Vector2(0.0f,0.0f);
    void set_center_position(Vector2 value);
    Vector2 get_center_position();
    

protected:
    
    static void _bind_methods();
    void update_mesh_node_data();
public:
    QMeshCircleNode(){
        useMeshData=true;
    };
    ~QMeshCircleNode(){

    };

    

    


};

#endif // QMESH_CIRCLE_NODE_H





