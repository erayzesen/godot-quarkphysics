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

#ifndef QTEXTUREDPBDBODYNODE_H
#define QTEXTUREDPBDBODYNODE_H

#include "qsoftbody_node.h"
#include "qangleconstraint_object.h"
#include "unordered_set"
#include "utility"

#include "godot_cpp/classes/multi_mesh_instance2d.hpp"
#include "godot_cpp/classes/multi_mesh.hpp"
#include <godot_cpp/classes/array_mesh.hpp>
#include <godot_cpp/classes/shader.hpp>
#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/variant/transform2d.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/gradient.hpp>
#include <godot_cpp/classes/gradient_texture2d.hpp>
#include <godot_cpp/classes/sub_viewport.hpp>
#include <godot_cpp/classes/viewport_texture.hpp>
#include <godot_cpp/classes/camera2d.hpp>

struct SpringIndexPairHash {
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ (std::hash<T2>()(pair.second) << 1);
    }
};




class QTexturedPBDBodyNode : public QSoftBodyNode {
    GDCLASS(QTexturedPBDBodyNode,QSoftBodyNode);
    
protected:
    static void _bind_methods();
    //Properties
    Ref<Texture2D> sourceTexture=nullptr;
    bool breakable=false;
    float breakDistance=10.0f;
    float pixelStepSize=4.0f;
    float textureScale=1.0f;


    bool enableSDFRendering=true;
    Ref<Texture2D> SFDTexture=nullptr;
    float SFDTextureScale=2.0f;
    float SFDShaderAlphaThreshold=0.5f;
    Sprite2D *SDFSprite=nullptr;
    Camera2D *SDFCamera=nullptr;
    SubViewport * SDFViewport=nullptr;
    
    //Mesh
    QMeshNode *meshNode=nullptr;

    //Events
    void _notification(int what);

    //Collections
    PackedVector2Array particlePositions;
    PackedColorArray particleColors;

    //MultiMesh Instance
    Ref<MultiMesh> multiMesh;
    MultiMeshInstance2D* multiMeshInstance; 


    //Rendering
    Ref<ArrayMesh> CreateCircleMesh(float radius,int segments);
    Ref<ArrayMesh> CreateQuadMesh(Vector2 size);
    void render();

    void on_physics_process();

    void InitDefaultParticleTexture();

    
public:
    QTexturedPBDBodyNode(){
        bodyObject=new QSoftBody();
        bodyObject->manualDeletion=true;
        bodyNodeType=QBodyNode::bodyNodeTypes::SOFT;
        init_events();
    };
    ~QTexturedPBDBodyNode(){};

    // Texture
    Ref<Texture2D> get_texture(){
        return sourceTexture;
    };
    QTexturedPBDBodyNode * set_texture(Ref<Texture2D> value){
        sourceTexture=value;
        queue_redraw();
        return this;
    };
    //SFD Features

    //Breakable

    bool get_sdf_rendering_enabled(){
        return enableSDFRendering;
    };
    QTexturedPBDBodyNode * set_sdf_rendering_enabled(bool value){
        enableSDFRendering=value;
        return this;
    };


    Ref<Texture2D> get_sfd_texture(){
        return SFDTexture;
    };
    QTexturedPBDBodyNode * set_sfd_texture(Ref<Texture2D> value){
        SFDTexture=value;
        return this;
    };

    float get_sfd_texture_scale(){
        return SFDTextureScale;
    };
    QTexturedPBDBodyNode * set_sfd_texture_scale(float value){
        SFDTextureScale=value;
        return this;
    };

    float get_sfd_shader_alpha_threshold(){
        return SFDShaderAlphaThreshold;
    };
    QTexturedPBDBodyNode * set_sfd_shader_alpha_threshold(float value){
        SFDShaderAlphaThreshold=value;

        if(SDFSprite!=nullptr){
            Ref<ShaderMaterial> material=SDFSprite->get_material();
            material->set_shader_parameter("alpha_threshold",SFDShaderAlphaThreshold);
        }
        return this;
    };

    //Breakable

    bool get_breakable(){
        return breakable;
    };
    QTexturedPBDBodyNode * set_breakable(bool value){
        breakable=value;
        return this;
    };

    //Break Distance
    float get_break_distance(){
        return breakDistance;
    };
    QTexturedPBDBodyNode * set_break_distance(float value){
        breakDistance=value;
        return this;
    };

    //Pixel Step Size
    float get_pixel_step_size(){
        return pixelStepSize;
    };
    QTexturedPBDBodyNode * set_pixel_step_size(float value){
        pixelStepSize=value;
        return this;
    };

    //Texture Scale
    float get_texture_scale(){
        return textureScale;
    };
    QTexturedPBDBodyNode * set_texture_scale(float value){
        textureScale=value;
        queue_redraw();
        return this;
    };

    QTexturedPBDBodyNode *create_mesh_with_texture();

    
    

};




#endif // QTEXTUREDPBDBODYNODE_H
