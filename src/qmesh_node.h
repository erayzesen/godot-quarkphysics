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

#ifndef QMESH_NODE_H
#define QMESH_NODE_H

#include <godot_cpp/classes/node2d.hpp>
#include "QuarkPhysics/qmesh.h"
#include "qparticle_object.h"
#include "qspring_object.h"

#include <godot_cpp/core/class_db.hpp>
#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/curve2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/rendering_server.hpp>



class QBodyNode;
class QMeshNode: public Node2D{
    GDCLASS(QMeshNode,Node2D);
protected:
    
    bool showSprings=true;
    bool showPolygon=true;
    bool showParticleIndexNumbers=false;
    float particleRadius=0.5f;
    bool disablePolygonForCollisions=false;
    
    QMesh *meshObject=nullptr;
    QBodyNode *ownerBodyNode=nullptr;
    virtual void update_mesh_node_data(){};
    QMesh::MeshData convert_mesh_node_data_to_mesh_data();
    void apply_mesh_data_to_mesh_node_data(QMesh::MeshData data);

    static void _bind_methods();
    bool useMeshData=false;
    bool isConfigured=false;

    //Render Features
    bool enableVectorRendering=false;
    bool enableAntialias=true;
    bool enableTriangulation=false;
    bool enableFill=true;
    Color fillColor=Color::named("white");
    Ref<Texture2D> fillTexture=nullptr;
    bool enableStretchingTextureToPolygon=true;
    float fillTextureScale=1.0f;
    Vector2 fillTextureOffset=Vector2(0,0);
    bool enableStroke=false;
    float strokeWidth=3.0f;
    Color strokeColor=Color::named("black");
    float strokeOffset=0.0f;
    bool enableParticleRendering=true;

    Ref<Curve2D> curvedPolygon=memnew( Curve2D);
    bool enableCurvedCorners=false;
    float curveLength=8.0f;
    


    //Collections
    vector<Ref<QParticleObject>> particleObjects;
    vector<Ref<QSpringObject>> springObjects;
    vector<Ref<QParticleObject>> polygon;

    //Data Collections
    PackedVector2Array dataParticlePositions;
    PackedFloat32Array dataParticleRadius;
    PackedByteArray dataParticleIsInternal;
    PackedByteArray dataParticleIsEnabled;
    PackedByteArray dataParticleIsLazy;
    TypedArray<PackedInt32Array> dataSprings;
    TypedArray<PackedInt32Array> dataInternalSprings;
    PackedInt32Array dataPolygon;
    TypedArray<PackedInt32Array> dataUVMaps;

    //Rendering
    godot::RID debugRenderInstance;
    godot::RID vectorRenderInstance;
    godot::RID textureRenderInstance;
    godot::RID textureMaskRenderInstance;
    RenderingServer *rs;

    void debug_render_in_editor();
    void vector_render_in_editor();
    void debug_render_in_runtime();
    void vector_render_in_runtime();
    PackedVector2Array create_circle_polygon(Vector2 pos, float radius);
    void on_process();
    void on_post_enter_tree();
    void _notification(int what);
public:
    QMeshNode(){
        rs=RenderingServer::get_singleton();

        debugRenderInstance=rs->canvas_item_create();
        vectorRenderInstance=rs->canvas_item_create();
        textureRenderInstance=rs->canvas_item_create();
        textureMaskRenderInstance=rs->canvas_item_create();

        rs->canvas_item_set_parent(textureMaskRenderInstance,get_canvas_item() );
        rs->canvas_item_set_parent(textureRenderInstance,textureMaskRenderInstance );
        rs->canvas_item_set_parent(vectorRenderInstance,get_canvas_item() );
        rs->canvas_item_set_parent(debugRenderInstance,get_canvas_item() );
       

       rs->canvas_item_set_canvas_group_mode(textureMaskRenderInstance,RenderingServer::CanvasGroupMode::CANVAS_GROUP_MODE_CLIP_ONLY);
       
    };
    ~QMeshNode(){
        rs->free_rid(debugRenderInstance);
        rs->free_rid(vectorRenderInstance);
        rs->free_rid(textureRenderInstance);
        rs->free_rid(textureMaskRenderInstance);
        if(meshObject!=nullptr){
            for(size_t i=0;i<particleObjects.size();++i ){
                particleObjects[i]->particleObject=nullptr;
            }
            for(size_t i=0;i<springObjects.size();++i ){
                springObjects[i]->springObject=nullptr;
            }
            delete meshObject;
            meshObject=nullptr;
        }
    }


    

    //Get Methods
    Vector2 get_mesh_position();
    Vector2 get_mesh_global_position();

    float get_mesh_rotation();
    float get_mesh_global_rotation();
    float get_initial_area();
    float get_initial_polygons_area();
    float get_area();
    float get_polygon_area();
    float get_circumference();
    bool get_show_springs_enabled();
    bool get_show_polygon_enabled();
    bool get_show_particle_index_numbers_enabled();
    Array get_average_position_and_rotation(TypedArray<QParticleObject> particle_collection);
    Array get_matching_particle_positions(TypedArray<QParticleObject> particle_collection,Vector2 target_position,float target_rotation);
    float get_min_angle_constraint_of_polygon();

    bool get_polygon_for_collision_disabled();

    QBodyNode *get_owner_body_node();
    

    //Set Methods
    QMeshNode *set_mesh_position(Vector2 value);
    QMeshNode *set_mesh_global_position(Vector2 value);
    QMeshNode *set_mesh_rotation(float value);
    QMeshNode *set_show_springs_enabled(bool value);
    QMeshNode *set_show_polygon_enabled(bool value);
    QMeshNode *set_show_particle_index_numbers_enabled(bool value);
    QMeshNode *set_min_angle_constraint_of_polygon(bool value);
    QMeshNode *set_polygon_for_collision_disabled(bool value);
    

    //Particle Operations
    QMeshNode *add_particle(Ref<QParticleObject> particle_object);
    QMeshNode *remove_particle(Ref<QParticleObject> particle_object);
    QMeshNode *remove_particle_at(int index);
    Ref<QParticleObject> get_particle_at(int index);
    int get_particle_count();
    int get_particle_index(Ref<QParticleObject> particle_object);    

    Ref<QParticleObject> get_particle_object_with_particle(QParticle * particle);


    //Spring Operations
    QMeshNode *add_spring(Ref<QSpringObject> spring_object);
    QMeshNode *remove_spring(Ref<QSpringObject> spring_object);
    QMeshNode *remove_spring_at(int index);
    Ref<QSpringObject> get_spring_at(int index);
    int get_spring_count();
    int get_spring_index(Ref<QSpringObject> spring_object);

    //Polygon Operations
    QMeshNode *set_polygon(TypedArray<QParticleObject> particleCollection);
    QMeshNode *add_particle_to_polygon(Ref<QParticleObject> particleObject);
    QMeshNode *remove_particle_from_polygon(Ref<QParticleObject> particleObject);
    QMeshNode *remove_particle_from_polygon_at(int index);
    QMeshNode *remove_polygon();
    Ref<QParticleObject> get_particle_from_polygon(int index);
    Rect2 get_aabb_of_polygon(PackedVector2Array polygonPoints);

    int get_polygon_particle_count();
    Array get_decomposited_polygon();

    //UV Operations
    int get_uv_map_count();
    PackedInt32Array get_uv_map_at(int index);
    QMeshNode * add_uv_map(PackedInt32Array map);
    QMeshNode * remove_uv_map_at(int index);
    

    //Render helper operations
    PackedVector2Array get_curved_polygon_points(PackedVector2Array polygonPoints,float curve_amount,float margin=0,Vector2 origin=Vector2(0,0),float bake_interval=5.0f);
    PackedVector2Array get_inner_shadow_of_polygon(PackedVector2Array polygon_points,Vector2 offset);
    PackedVector2Array get_specified_side_points_of_polygon(PackedVector2Array polygon_points,Vector2 axis,int points_count_range,float scale=1.0, Vector2 origin=Vector2(0,0));


    //Rendering Features
    bool get_vector_rendering_enabled(){
        return enableVectorRendering;
    }

    bool get_antialias_enabled(){
        return enableAntialias;
    }

    bool get_triangulation_enabled(){
        return enableTriangulation;
    }

    bool get_fill_enabled(){
        return enableFill;
    }

    Color get_fill_color(){
        return fillColor;
    }

    Ref<Texture2D> get_fill_texture() const {
        return fillTexture;
    }

    bool get_stretching_texture_to_polygon_enabled() {
        return enableStretchingTextureToPolygon;
    }

    float get_fill_texture_scale() {
        return fillTextureScale;
    }

    Vector2 get_fill_texture_offset() {
        return fillTextureOffset;
    }

    bool get_stroke_enabled(){
        return enableStroke;
    }

    float get_stroke_width(){
        return strokeWidth;
    }

    Color get_stroke_color(){
        return strokeColor;
    }

    float get_stroke_offset(){
        return strokeOffset;
    }

    bool get_curved_corners_enabled(){
        return enableCurvedCorners;
    }

    float get_curve_length(){
        return curveLength;
    }

    bool get_particle_rendering_enabled(){
        return enableParticleRendering;
    }

    QMeshNode *set_vector_rendering_enabled(bool value){
        enableVectorRendering=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_antialias_enabled(bool value){
        enableAntialias=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_triangulation_enabled(bool value){
        enableTriangulation=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_fill_enabled(bool value){
        enableFill=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_fill_color(Color value){
        fillColor=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_fill_texture(const Ref <Texture2D> &texture){
        fillTexture=texture;
        queue_redraw();
        return this;
    }

    QMeshNode *set_stretching_texture_to_polygon_enabled(bool value){
        enableStretchingTextureToPolygon=value;
        queue_redraw();
        return this;
    }
    QMeshNode * set_fill_texture_scale(float value) {
        fillTextureScale=value;
        queue_redraw();
        return this;
    }

    QMeshNode * set_fill_texture_offset(Vector2 value) {
        fillTextureOffset=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_stroke_width(float value){
        strokeWidth=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_stroke_enabled(bool value){
        enableStroke=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_stroke_color(Color value){
        strokeColor=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_stroke_offset(float value){
        strokeOffset=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_curved_corners_enabled(bool value){
        enableCurvedCorners=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_curve_length(float value){
        curveLength=value;
        queue_redraw();
        return this;
    }

    QMeshNode *set_particle_rendering_enabled(bool value){
        enableParticleRendering=value;
        queue_redraw();
        return this;
    }

    //Data

    void set_data_particle_positions(PackedVector2Array collection){
        dataParticlePositions=collection;
    }
    PackedVector2Array get_data_particle_positions(){
        return dataParticlePositions;
    }

    void set_data_particle_radius(PackedFloat32Array collection){
        dataParticleRadius=collection;
    }
    PackedFloat32Array get_data_particle_radius(){
        return dataParticleRadius;
    }

    void set_data_particle_is_internal(PackedByteArray collection){
        dataParticleIsInternal=collection;
    }
    PackedByteArray get_data_particle_is_internal(){
        return dataParticleIsInternal;
    }

    void set_data_particle_is_enabled(PackedByteArray collection){
        dataParticleIsEnabled=collection;
    }

    PackedByteArray get_data_particle_is_enabled(){
        return dataParticleIsEnabled;
    }

    void set_data_particle_is_lazy(PackedByteArray collection){
        dataParticleIsLazy=collection;
    }

    PackedByteArray get_data_particle_is_lazy(){
        return dataParticleIsLazy;
    }

    void set_data_springs(TypedArray<PackedInt32Array> collection){
        dataSprings=collection;
    }
    TypedArray<PackedInt32Array> get_data_springs(){
        return dataSprings;
    }

    void set_data_internal_springs(TypedArray<PackedInt32Array> collection){
        dataInternalSprings=collection;
    }
    TypedArray<PackedInt32Array> get_data_internal_springs(){
        return dataInternalSprings;
    }

    void set_data_polygon(PackedInt32Array collection){
        dataPolygon=collection;
    }
    PackedInt32Array get_data_polygon(){
        return dataPolygon;
    }

    void set_data_uv_maps(TypedArray<PackedInt32Array> collection){
        dataUVMaps=collection;
    };
    TypedArray<PackedInt32Array>  get_data_uv_maps(){
        return dataUVMaps;
    };



    //Type casting

    static QMeshNode *type_cast(Object *obj);

    


    friend class QBodyNode;
    friend class QWorldNode;

};

class QDebugColors{
    public:
        const Color COLLIDER_DYNAMIC=Color::named("limegreen"); //green
        const Color COLLIDER_PARTICLE=Color::named("limegreen");
        const Color COLLIDER_PARTICLE_DISABLED=Color::named("lightseagreen");
        const Color COLLIDER_PARTICLE_LAZY=Color::named("sandybrown");
        const Color COLLIDER_DYNAMIC_SLEEPING=Color::named("seagreen"); //green
        const Color COLLIDER_STATIC=Color::named("deepskyblue"); //blue
        const Color SPRING=Color::named("darkgray"); // gray
        const Color SPRING_INTERNAL=Color::named("dimgray"); // dark gray
        const Color SPRING_EXTERNAL=Color::named("burlywood"); // burlywood
        const Color JOINT=Color::named("goldenrod"); // goldenrod
        const Color RAYCAST=Color::named("darksalmon"); // darksalmon
};


#endif // QMESH_NODE_H
