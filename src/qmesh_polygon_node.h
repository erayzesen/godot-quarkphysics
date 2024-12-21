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

#ifndef QMESH_POLYGON_NODE_H
#define QMESH_POLYGON_NODE_H

#include "qmesh_node.h"

class QMeshPolygonNode: public QMeshNode {
    GDCLASS(QMeshPolygonNode,QMeshNode);
private:
    float polygonRadius=32.0f;
    void set_polygon_radius(float value);
    float get_polygon_radius();
    int sideCount=8;
    void set_side_count(int value);
    int get_side_count();
    Vector2 centerPosition=Vector2(0.0f,0.0f);
    void set_center_position(Vector2 value);
    Vector2 get_center_position();
    int polarGrid=-1;
    void set_polar_grid(int value);
    int get_polar_grid();

    float particleRadius=0.5f;

protected:
    
    static void _bind_methods();
    void update_mesh_node_data();
public:
    QMeshPolygonNode(){
        useMeshData=true;
    };
    ~QMeshPolygonNode(){

    };

    QMeshPolygonNode *set_particle_radius(float value);
    float get_particle_radius();

    


};


#endif // QMESH_POLYGON_NODE_H




