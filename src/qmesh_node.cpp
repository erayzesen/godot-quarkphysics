#include "qmesh_node.h"
#include "qbody_node.h"
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/classes/geometry2d.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/theme_db.hpp>
#include <godot_cpp/classes/font.hpp>
#include "string"
#include "qworld_node.h"



void QMeshNode::_notification(int what) {
    switch (what){
    case NOTIFICATION_POST_ENTER_TREE:
        if(Engine::get_singleton()->is_editor_hint()==false){
            on_post_enter_tree();
        }else{
            if(useMeshData==true){
                update_mesh_node_data();
            }
        }
        
        
        break;
    case NOTIFICATION_DRAW:
        if(Engine::get_singleton()->is_editor_hint()==true){
            vector_render_in_editor();
            debug_render_in_editor();
        }else{
            vector_render_in_runtime();
            debug_render_in_runtime();
        }
        break;

    case NOTIFICATION_PROCESS:
        if(Engine::get_singleton()->is_editor_hint()==false)
            on_process();
        break;
    case NOTIFICATION_PARENTED:
        if(Engine::get_singleton()->is_editor_hint()==false && meshObject!=nullptr){
            QBodyNode *bodyNode=QBodyNode::type_cast(get_parent() );
            if(bodyNode!=nullptr){
                bodyNode->add_mesh_node(this);
                ownerBodyNode=bodyNode;
            }else{
                ownerBodyNode=nullptr;
            }
        }
        break;
    case NOTIFICATION_UNPARENTED:
        if(Engine::get_singleton()->is_editor_hint()==false && meshObject!=nullptr){
            if(ownerBodyNode!=nullptr){
                ownerBodyNode->remove_mesh_node(this);
                ownerBodyNode=nullptr;
            } 
        }
        break;
    case NOTIFICATION_PREDELETE:
        if(Engine::get_singleton()->is_editor_hint()==false && meshObject!=nullptr){
            if(ownerBodyNode!=nullptr){
                ownerBodyNode->remove_mesh_node(this);
                ownerBodyNode=nullptr;
            } 
            
        }
        break;

    
    default:
        break;
    }
}



void QMeshNode::debug_render_in_editor() {

    rs->canvas_item_clear(debugRenderInstance);

    
    QDebugColors colors;
    Color colliderColor=colors.COLLIDER_DYNAMIC;
   
    QBodyNode *parent_body=QBodyNode::type_cast(get_parent() );
    if(parent_body!=nullptr){
        colliderColor=parent_body->get_mode()==QBodyNode::Modes::STATIC ? colors.COLLIDER_STATIC:colliderColor;
    }

    //Draw springs
    if(showSprings){
        //default springs
        for(int i=0;i<dataSprings.size();i++){
            PackedInt32Array spring=dataSprings[i];
            Vector2 pA=dataParticlePositions[spring[0] ];
            Vector2 pB=dataParticlePositions[spring[1] ];
            //draw_line( pA,pB,colors.SPRING);
            rs->canvas_item_add_line(debugRenderInstance,pA,pB,colors.SPRING);
        }
        //internal springs
        for(int i=0;i<dataInternalSprings.size();i++){
            PackedInt32Array spring=dataInternalSprings[i];
            Vector2 pA=dataParticlePositions[spring[0] ];
            Vector2 pB=dataParticlePositions[ spring[1] ];
            //draw_line(pA,pB,colors.SPRING_INTERNAL);
            rs->canvas_item_add_line(debugRenderInstance,pA,pB,colors.SPRING_INTERNAL);
        }
    }

    //Draw polygons
    if(showPolygon){

        
        for(int n=0;n<dataPolygon.size();n++){
            Vector2 p=dataParticlePositions[ dataPolygon[n] ];
            Vector2 pn=dataParticlePositions[dataPolygon[(n+1)%dataPolygon.size()] ];
            //draw_line(p,pn,colliderColor);
            rs->canvas_item_add_line(debugRenderInstance,p,pn,colliderColor);
        }
    }
    //Draw particles
    for(int i=0;i<dataParticlePositions.size();i++){
        Vector2 pos=dataParticlePositions[i];
        float radius=dataParticleRadius[i];
        Color particleColor=colors.COLLIDER_PARTICLE;
        if (i<dataParticleIsLazy.size() ){
            if (dataParticleIsLazy[i]==1){
                particleColor=colors.COLLIDER_PARTICLE_LAZY;
            }
        }
        if (i<dataParticleIsEnabled.size() ){
            if (dataParticleIsEnabled[i]==0){
                particleColor=colors.COLLIDER_PARTICLE_DISABLED;
            }
        }
        if (colliderColor==colors.COLLIDER_STATIC){
            particleColor=colliderColor;
        }
        if(radius==0.5f){
            //draw_circle(pos,radius,colliderColor);
            rs->canvas_item_add_circle(debugRenderInstance,pos,radius,particleColor);
        }else{
            //draw_arc(pos,radius,0,M_PI*2,particleCount,colliderColor,1.0f);
            rs->canvas_item_add_polyline(debugRenderInstance,create_circle_polygon(pos,radius),{particleColor} );
            
            
            if(dataParticlePositions.size()==1 ){
                //draw_line(pos,pos+Vector2(radius,0),colliderColor,1.0f );
                rs->canvas_item_add_line(debugRenderInstance,pos,pos+Vector2(radius,0),particleColor);
            }
        }

        if (showParticleIndexNumbers==true){
            Ref<Font> font=ThemeDB::get_singleton()->get_fallback_font();
            
            Vector2 textPos=pos;
            textPos+=Vector2(-12,5);
            font->draw_string_outline(debugRenderInstance,textPos,String::num_int64(i),godot::HORIZONTAL_ALIGNMENT_CENTER,24,10,3.0f,Color::named("black"));
            font->draw_string(debugRenderInstance,textPos, String::num_int64(i) ,godot::HORIZONTAL_ALIGNMENT_CENTER,24,10,particleColor);
            
        }
    }

}

void QMeshNode::vector_render_in_editor() {

    rs->canvas_item_clear(vectorRenderInstance);
    rs->canvas_item_clear(textureRenderInstance);
    rs->canvas_item_clear(textureMaskRenderInstance);

    if(enableVectorRendering==false){
        return;
    }
    
    
    PackedColorArray fillColors;
    fillColors.push_back(fillColor);
    PackedColorArray strokeColors;
    strokeColors.push_back(strokeColor);
    
    PackedVector2Array polygonPoints;
    for(int i=0;i<dataPolygon.size();i++){
        int pi=dataPolygon[i];
        Vector2 pos=dataParticlePositions[pi];
        polygonPoints.push_back( pos );
    }
    

    PackedVector2Array strokePoints;

    if(polygonPoints.size()>2){

        if (enableCurvedCorners){
            strokePoints=get_curved_polygon_points(polygonPoints, curveLength,strokeOffset);
        }else{
            if(strokeOffset==0.0){
                strokePoints=polygonPoints;
                if(strokePoints.size()>0 )
                    strokePoints.push_back(strokePoints[0]);
            }else{
                int polygonSize=polygonPoints.size();
                for(size_t i=0;i<polygonSize;++i ){
                    Vector2 pp=polygonPoints[ (i-1+polygonSize) % polygonSize ];
                    Vector2 cp=polygonPoints[i];
                    Vector2 np=polygonPoints[ (i+1) % polygonSize ];

                    Vector2 curveUnit=(np-pp).normalized();
                    Vector2 curveNormal=curveUnit.orthogonal();

                    Vector2 finalPoint=cp-curveNormal*strokeOffset;
                    strokePoints.push_back(finalPoint); 
                }
                if(strokePoints.size()>0 )
                    strokePoints.push_back(strokePoints[0]);

            }
        }

        //draw polygons

        if(enableFill ){
            
            //Rendering Textures
            if(fillTexture.is_valid() ){
                Rect2 textureRect;
                if(enableStretchingTextureToPolygon){
                    textureRect=get_aabb_of_polygon(polygonPoints);
                }else{
                    Vector2 tSize=fillTexture->get_size();
                    textureRect=Rect2(-tSize*0.5,tSize);
                }
                Vector2 zeroDistance=textureRect.position;
                for(size_t i=0;i<dataUVMaps.size();++i ){
                    PackedInt32Array map=dataUVMaps[i];
                    PackedVector2Array points;
                    for(size_t n=0;n<map.size();++n ){
                        points.push_back( dataParticlePositions[ map[n] ] );
                    }
                    PackedVector2Array uvPoints;
                    for(size_t n=0;n<points.size();++n ){
                        Vector2 p=points[n] ;
                        p/=fillTextureScale;
                        p-=zeroDistance+fillTextureOffset;
                        Vector2 uvp=Vector2(p.x/textureRect.size.x,p.y/textureRect.size.y);
                        uvPoints.push_back(uvp);
                    }
                    if(Geometry2D::get_singleton()->triangulate_polygon(points).is_empty()==false  ){
                        rs->canvas_item_add_polygon(textureRenderInstance,points,{fillColor},uvPoints,fillTexture->get_rid() );
                    }

                }
                
            }
            //Rendering Polygons
            RID polygonDrawInstance=fillTexture.is_valid() ? textureMaskRenderInstance:vectorRenderInstance;
            if(enableTriangulation){
                if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).size()==0 ){
                    for(size_t s=0;s<meshObject->GetSubConvexPolygonCount();++s){
                        vector<QParticle*> subPolygonParticles=meshObject->GetSubConvexPolygonAt(s);
                        PackedVector2Array subPolygonPoints;
                        for(size_t p=0;p<subPolygonParticles.size();++p ){
                            QVector pPos=subPolygonParticles[p]->GetGlobalPosition();
                            subPolygonPoints.push_back( Vector2(pPos.x,pPos.y) );
                        }
                        if(subPolygonPoints.size()>2 ){
                            TypedArray<PackedVector2Array> partedPolygons=Geometry2D::get_singleton()->intersect_polygons(subPolygonPoints,subPolygonPoints);
                            for(size_t j=0;j<partedPolygons.size();++j ){                
                                rs->canvas_item_add_polygon(polygonDrawInstance,partedPolygons[j],{fillColors} );
                            }
                        }

                            
                    }
                }else{ 
                    if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).is_empty()==false  ){
                        rs->canvas_item_add_polygon(polygonDrawInstance,polygonPoints,{fillColors} );
                    }
                }
            }else{
                if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).is_empty()==false  ){
                    rs->canvas_item_add_polygon(polygonDrawInstance,polygonPoints,{fillColors} );
                }
            }
        }
        
    }
    
    if(enableStroke && strokePoints.size()>2 ){
        rs->canvas_item_add_polyline(vectorRenderInstance,strokePoints,strokeColors,strokeWidth,enableAntialias);
    }
    
    

    //draw particles
    if (enableParticleRendering){
        for(int i=0;i<dataParticlePositions.size();i++){
            float r=dataParticleRadius[i];
            if(r<=0.5)
                continue;
            Vector2 pos=dataParticlePositions[i];
            
            int particleCount=round( (2*M_PI*r)/2 );
            particleCount=max(particleCount,8);
            if(enableFill)
                rs->canvas_item_add_circle(vectorRenderInstance,pos,r,fillColor,true);
            if(enableStroke)
                rs->canvas_item_add_polyline(vectorRenderInstance,create_circle_polygon(pos,r+strokeOffset),{strokeColor},strokeWidth,enableAntialias);
            
        }
    }

}

void QMeshNode::debug_render_in_runtime()
{

    if(get_owner_body_node()!=nullptr){
        if(get_owner_body_node()->get_world_node()!=nullptr ){
            if(get_owner_body_node()->get_world_node()->get_debug_renderer_enabled()==false ){
                return;
            }
        }else{
            return;
        }
    }else{
        return;
    }

    rs->canvas_item_clear(debugRenderInstance);
    QDebugColors colors;

    bool isOwnerBodyRigid=false;
    Vector2 node_scale_inverse_factor=Vector2(1/get_global_scale().x,1/get_global_scale().y);
    //Drawing springs
    if(ownerBodyNode!=nullptr){
        if(ownerBodyNode->bodyObject->GetBodyType()!=QBody::BodyTypes::RIGID){

            for(int i=0;i<meshObject->GetSpringCount();i++){
                QSpring* spring=meshObject->GetSpringAt(i);
                Color springColor=spring->GetIsInternal() ? colors.SPRING_INTERNAL:colors.SPRING;
                QVector qpa=spring->GetParticleA()->GetGlobalPosition();
                QVector qpb=spring->GetParticleB()->GetGlobalPosition();
                Vector2 pA=( Vector2(qpa.x,qpa.y)-get_global_position() ).rotated(-get_global_rotation() );
                Vector2 pB=( Vector2(qpb.x,qpb.y)-get_global_position() ).rotated(-get_global_rotation() );
                pA*=node_scale_inverse_factor;
                pB*=node_scale_inverse_factor;
                rs->canvas_item_add_line(debugRenderInstance,pA,pB,springColor);
                
            }
        }else{
            isOwnerBodyRigid=true;
        }
    }

    Color colliderColor=colors.COLLIDER_DYNAMIC;
    if(ownerBodyNode!=nullptr){
        
        if (ownerBodyNode->get_mode()==QBody::Modes::STATIC){
            colliderColor=colors.COLLIDER_STATIC;
        }  

    }
    //Drawing polygons
    if(meshObject->GetPolygonParticleCount()>2 ){
        PackedVector2Array polygonPoints;
        for(int n=0;n<meshObject->GetPolygonParticleCount();n++){
            
            QVector qp=meshObject->GetParticleFromPolygon(n)->GetGlobalPosition();
            Vector2 p=(Vector2(qp.x,qp.y)-get_global_position() ).rotated(-get_global_rotation() ) ;
            p*=node_scale_inverse_factor;
            polygonPoints.push_back(p);
            
        }
        polygonPoints.append(polygonPoints[0]);
        if(polygonPoints.size()!=0){
            PackedColorArray colors;
            colors.append(colliderColor);
            rs->canvas_item_add_polyline(debugRenderInstance,polygonPoints,colors);
        }
        //Sub Convex Polygons Rendering
        for(int n=0;n<meshObject->GetSubConvexPolygonCount();++n){
            vector<QParticle*> subPolygon=meshObject->GetSubConvexPolygonAt(n);
            PackedVector2Array points;
            for(int m=0;m<subPolygon.size();m++){
                
                QVector qp=subPolygon[m]->GetGlobalPosition();
                Vector2 p=(Vector2(qp.x,qp.y)-get_global_position() ).rotated(-get_global_rotation() ) ;
                p*=node_scale_inverse_factor;
                points.push_back(p);
                
            }
            points.append(points[0]);
            Color subPolygonColor=colliderColor;
            subPolygonColor.a=0.2f;
            PackedColorArray colors;
            colors.append(subPolygonColor);
            rs->canvas_item_add_polyline(debugRenderInstance,points,colors);
            
        }
    }
    //Drawing particles
    for(int i=0;i<meshObject->GetParticleCount();i++){
        QParticle *particle=meshObject->GetParticleAt(i);
        Vector2 pos=Vector2( particle->GetGlobalPosition().x, particle->GetGlobalPosition().y )-get_global_position();
        pos=pos.rotated(-get_global_rotation() );
        pos*=node_scale_inverse_factor;
        float radius=particle->GetRadius();
        Color particleColor=colors.COLLIDER_PARTICLE;
        if (particle->GetIsLazy()==true){
            particleColor=colors.COLLIDER_PARTICLE_LAZY;
        }
        if(particle->GetEnabled()==false ){
            particleColor=colors.COLLIDER_PARTICLE_DISABLED;
        }
        if (colliderColor==colors.COLLIDER_STATIC){
            particleColor=colliderColor;
        }
        
        if(radius==0.5f){
            rs->canvas_item_add_circle(debugRenderInstance,pos,radius,particleColor);
        }else{
            int particleCount=round( (2*M_PI*radius)/2 );
            particleCount=max(particleCount,8);
            rs->canvas_item_add_polyline(debugRenderInstance,create_circle_polygon(pos,radius),{particleColor} );
            
            if( isOwnerBodyRigid==true && meshObject->GetParticleCount()==1){
                
                rs->canvas_item_add_line(debugRenderInstance,pos,( pos+Vector2(radius,0) ) ,particleColor);
            }

        }

        
        

    }

}

void QMeshNode::vector_render_in_runtime()
{

    rs->canvas_item_clear(vectorRenderInstance);
    rs->canvas_item_clear(textureRenderInstance);
    rs->canvas_item_clear(textureMaskRenderInstance);
    
    if(enableVectorRendering==false)
        return;

    Vector2 node_scale_inverse_factor=Vector2(1/get_global_scale().x,1/get_global_scale().y);

    PackedColorArray fillColors;
    fillColors.push_back(fillColor);	
    PackedColorArray strokeColors;
    strokeColors.push_back(strokeColor);
    //Drawing polygons
    if(meshObject->GetPolygonParticleCount()>2){
        int polygonSize=meshObject->GetPolygonParticleCount();
        PackedVector2Array polygonPoints;
        PackedVector2Array polygonLocalPoints;
        for(int n=0;n<polygonSize;n++){
            QVector qpg=meshObject->GetParticleFromPolygon(n)->GetGlobalPosition();
            QVector qpl=meshObject->GetParticleFromPolygon(n)->GetPosition();
            Vector2 pg=(Vector2(qpg.x,qpg.y)-get_global_position() ).rotated(-get_global_rotation() );
            pg*=node_scale_inverse_factor; 
            Vector2 pl=Vector2(qpl.x,qpl.y);
            polygonPoints.push_back(pg );
            polygonLocalPoints.push_back(pl );
            
        }
        /* if(polygonPoints.size()>0){
            polygonPoints.push_back(polygonPoints[0] );
        } */

        PackedVector2Array strokePoints;
        if (enableCurvedCorners){
            strokePoints=get_curved_polygon_points(polygonPoints, curveLength,strokeOffset);
        }else{
            if(strokeOffset==0.0){
                strokePoints=polygonPoints;
                if(strokePoints.size()>0 )
                    strokePoints.push_back(strokePoints[0]);
            }else{
                int polygonSize=polygonPoints.size();
                for(size_t i=0;i<polygonSize;++i ){
                    Vector2 pp=polygonPoints[ (i-1+polygonSize) % polygonSize ];
                    Vector2 cp=polygonPoints[i];
                    Vector2 np=polygonPoints[ (i+1) % polygonSize ];

                    Vector2 curveUnit=(np-pp).normalized();
                    Vector2 curveNormal=curveUnit.orthogonal();

                    Vector2 finalPoint=cp-curveNormal*strokeOffset;
                    strokePoints.push_back(finalPoint); 
                }
                if(strokePoints.size()>0 )
                    strokePoints.push_back(strokePoints[0]);

            }
        }
        
        
        if(enableFill){

            if (fillTexture.is_valid() && get_uv_map_count()>0 ){
                
                Rect2 textureRect;
                if(enableStretchingTextureToPolygon){
                    textureRect=get_aabb_of_polygon(polygonLocalPoints);
                }else{
                    Vector2 tSize=fillTexture->get_size();
                    textureRect=Rect2(-tSize*0.5,tSize);
                }
                Vector2 zeroDistance=textureRect.position;
                for(size_t i=0;i<get_uv_map_count() ;++i ){
                    PackedInt32Array map=get_uv_map_at(i);
                    
                    vector <Ref<QParticleObject>> polygonParticles;
                    for(size_t j=0;j<map.size();++j ){
                        polygonParticles.push_back( get_particle_at( map[j] ) );
                    }
                    PackedVector2Array points;
                    PackedVector2Array uvPoints;
                    for(size_t j=0;j<polygonParticles.size();++j ){
                        Ref<QParticleObject> p=polygonParticles[j] ;
                        Vector2 localPosition=p->get_position();
                        localPosition/=fillTextureScale;
                        localPosition-=zeroDistance+fillTextureOffset;
                        Vector2 uvp=Vector2(localPosition.x/textureRect.size.x,localPosition.y/textureRect.size.y);
                        uvPoints.push_back(uvp);
                        points.push_back( (p->get_global_position()-get_global_position()).rotated(-get_global_rotation() ) );
                    }
                    if(Geometry2D::get_singleton()->is_polygon_clockwise(points)==true ){
                        continue;
                    }
                    if(Geometry2D::get_singleton()->triangulate_polygon(points).is_empty()==false  ){
                        rs->canvas_item_add_polygon(textureRenderInstance,points,{fillColor},uvPoints,fillTexture->get_rid() );
                    }
                    
                }

                
            }

            RID polygonDrawInstance=fillTexture.is_valid() ? textureMaskRenderInstance:vectorRenderInstance;
            if(enableTriangulation){
                if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).size()==0 ){
                    for(size_t s=0;s<meshObject->GetSubConvexPolygonCount();++s){
                        vector<QParticle*> subPolygonParticles=meshObject->GetSubConvexPolygonAt(s);
                        PackedVector2Array subPolygonPoints;
                        
                        for(size_t p=0;p<subPolygonParticles.size();++p ){
                            QVector pPos=subPolygonParticles[p]->GetGlobalPosition();
                            subPolygonPoints.push_back( (Vector2(pPos.x,pPos.y)-get_global_position()).rotated(-get_global_rotation() ) );
                        }

                        if(subPolygonPoints.size()>2 ){
                            TypedArray<PackedVector2Array> partedPolygons=Geometry2D::get_singleton()->intersect_polygons(subPolygonPoints,subPolygonPoints);
                            for(size_t j=0;j<partedPolygons.size();++j ){

                                rs->canvas_item_add_polygon(polygonDrawInstance,partedPolygons[j],{fillColors} );
                                
                            }
                        }
     
                    }
                    
                }else{
                    if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).is_empty()==false  ){
                        rs->canvas_item_add_polygon(polygonDrawInstance,polygonPoints,{fillColors} );
                    }
                }
            }else{
                if(Geometry2D::get_singleton()->triangulate_polygon(polygonPoints).is_empty()==false  ){
                    rs->canvas_item_add_polygon(polygonDrawInstance,polygonPoints,{fillColors} );
                }
            }

            

        }

        if(enableStroke){
            rs->canvas_item_add_polyline(vectorRenderInstance,strokePoints,strokeColors,strokeWidth,enableAntialias);
            
        }
            
    }
    //Drawing particles
    if (enableParticleRendering){
        
        for(int i=0;i<meshObject->GetParticleCount();i++){
            QParticle *particle=meshObject->GetParticleAt(i);
            float r=particle->GetRadius();
            if(r<=0.5f){
                continue;
            }
            Vector2 pos=Vector2( particle->GetGlobalPosition().x, particle->GetGlobalPosition().y );
            pos-=get_global_position();
            pos=pos.rotated(-get_global_rotation() );
            if(enableFill){
                rs->canvas_item_add_circle(vectorRenderInstance,pos,r,fillColor,true);
            }
            if( enableStroke){
                int particleCount=round( (2*M_PI*r)/2 );
                particleCount=max(particleCount,8);
                rs->canvas_item_add_polyline(vectorRenderInstance,create_circle_polygon(pos,r+strokeOffset),{strokeColor},strokeWidth,enableAntialias);
            }
        }

    }
             
}

PackedVector2Array QMeshNode::create_circle_polygon(Vector2 pos, float radius)
{
    PackedVector2Array res;
    int pointCount=120;
    float anglePart=(M_PI*2)/pointCount;
    for(size_t i=0;i<pointCount;++i){
        float curAngle=anglePart*i;
        Vector2 point=pos+( Vector2(cos(curAngle),sin(curAngle) )*radius );
        res.push_back(point);
    }
    res.push_back(res[0]);

    return res;

}

void QMeshNode::on_process() {
	queue_redraw();
}

void QMeshNode::on_post_enter_tree() {
    if(isConfigured==false){
        if (meshObject==nullptr){
            
            if(useMeshData==true){
                update_mesh_node_data();
            }
            QMesh::MeshData qData=convert_mesh_node_data_to_mesh_data();
            meshObject=QMesh::CreateWithMeshData(qData ) ;
            meshObject->manualDeletion=true;
                
            
            //Creating object and node versions of the mesh
            for(int i=0;i<meshObject->GetParticleCount();i++ ){ //particles
                Ref<QParticleObject> p=memnew( QParticleObject(meshObject->GetParticleAt(i)) );
                particleObjects.push_back(p);
            }
            for(int i=0;i<meshObject->GetSpringCount();i++ ){ //springs
                Ref<QSpringObject> s=memnew( QSpringObject(meshObject->GetSpringAt(i)) );
                Ref<QParticleObject> pA=get_particle_object_with_particle(meshObject->GetSpringAt(i)->GetParticleA() );
                Ref<QParticleObject> pB=get_particle_object_with_particle(meshObject->GetSpringAt(i)->GetParticleB() );
                s->pA=pA;
                s->pB=pB;
                
                springObjects.push_back(s);
            }
            
            int polygonParticleCount=meshObject->GetPolygonParticleCount(); //polygon
            for(int n=0;n<polygonParticleCount;n++ ){
                Ref<QParticleObject> p=get_particle_object_with_particle(meshObject->GetParticleFromPolygon(n));
                polygon.push_back(p);
            } 
        }
    }

    set_mesh_position(get_position());
    set_mesh_rotation(get_rotation());

    //Re-Set Properties of Mesh Object
    meshObject->SetPolygonForCollisionsDisabled(disablePolygonForCollisions);
    
    if(ownerBodyNode==nullptr){
        QBodyNode *bodyNode=QBodyNode::type_cast(get_parent());
        if(bodyNode!=nullptr){
            bodyNode->add_mesh_node(this);
            ownerBodyNode=bodyNode;
        }
    }

    set_process(true);

    

    isConfigured=true;

    //cout<<"on post enter tree finished!"<<endl;
    
    //godot::UtilityFunctions::print("Quark Physics Status: QMeshNode Configured! | QMeshNode::on_post_enter_tree() ");
        
    
    
}

QMesh::MeshData QMeshNode::convert_mesh_node_data_to_mesh_data()
{

    QMesh::MeshData qData;
    for(size_t i=0;i<dataParticlePositions.size();++i){
        Vector2 pos=dataParticlePositions[i];
        pos*=get_global_scale();
        qData.particlePositions.push_back( QVector( pos.x, pos.y) );
    }
    
    for(size_t i=0;i<dataParticlePositions.size();++i){
        if (i>=dataParticleRadius.size())
            qData.particleRadValues.push_back( 0.5 );
        else 
            qData.particleRadValues.push_back( dataParticleRadius[i] );
    }

    

    for(size_t i=0;i<dataParticlePositions.size();++i){
        if (i>=dataParticleIsInternal.size())
            qData.particleInternalValues.push_back( false );
        else
            qData.particleInternalValues.push_back( dataParticleIsInternal[i] );
    }

    for(size_t i=0;i<dataParticlePositions.size();++i){
        if (i>=dataParticleIsEnabled.size())
            qData.particleEnabledValues.push_back( true );
        else
            qData.particleEnabledValues.push_back( dataParticleIsEnabled[i] );
    }

    for(size_t i=0;i<dataParticlePositions.size();++i){
        if (i>=dataParticleIsLazy.size())
            qData.particleLazyValues.push_back( false );
        else
            qData.particleLazyValues.push_back( dataParticleIsLazy[i] );
    }

    for(size_t i=0;i<dataSprings.size();++i){
        PackedInt32Array intPair=dataSprings[i];
        int sA=intPair[0];
        int sB=intPair[1];
        qData.springList.push_back( pair<int,int> ( sA, sB ) );
    }

    for(size_t i=0;i<dataInternalSprings.size();++i){
        PackedInt32Array intPair=dataInternalSprings[i];
        int sA=intPair[0];
        int sB=intPair[1];
        qData.internalSpringList.push_back( pair<int,int> ( sA, sB ) );
    }

    for(size_t i=0;i<dataPolygon.size();++i){
        qData.polygon.push_back( dataPolygon[i] );
    }

    for(size_t i=0;i<dataUVMaps.size();++i){
        PackedInt32Array map=dataUVMaps[i];
        vector<int> qmap;
        for(size_t j=0;j<map.size();++j){
            qmap.push_back( map[j] );
        }
        qData.UVMaps.push_back(qmap);
    }

    return qData;
}

void QMeshNode::apply_mesh_data_to_mesh_node_data(QMesh::MeshData data)
{
    dataParticlePositions.clear();
    dataParticleRadius.clear();
    dataParticleIsInternal.clear();
    dataParticleIsEnabled.clear();
    dataParticleIsLazy.clear();
    dataSprings.clear();
    dataInternalSprings.clear();
    dataPolygon.clear();
    dataUVMaps.clear();

    for(size_t i=0;i<data.particlePositions.size();++i ){
        dataParticlePositions.push_back(Vector2(data.particlePositions[i].x,data.particlePositions[i].y ) );
    }

    for(size_t i=0;i<data.particleRadValues.size();++i ){
        dataParticleRadius.push_back( data.particleRadValues[i] );
    }

    for(size_t i=0;i<data.particleInternalValues.size();++i ){
        dataParticleIsInternal.push_back(  data.particleInternalValues[i] );
    }

    for(size_t i=0;i<data.particleEnabledValues.size();++i ){
        dataParticleIsEnabled.push_back(data.particleEnabledValues[i] );
    }

    for(size_t i=0;i<data.particleLazyValues.size();++i ){
        dataParticleIsLazy.push_back(data.particleLazyValues[i] );
    }

    for(size_t i=0;i<data.springList.size();++i ){
        PackedInt32Array intPair={data.springList[i].first,data.springList[i].second};
        dataSprings.push_back( intPair );
    }

    for(size_t i=0;i<data.internalSpringList.size();++i ){
        PackedInt32Array intPair={data.internalSpringList[i].first,data.internalSpringList[i].second};
        dataInternalSprings.push_back( intPair );
    }

    for(size_t i=0;i<data.polygon.size();++i ){
        dataPolygon.push_back(  data.polygon[i] );
    }

    for(size_t i=0;i<data.UVMaps.size();++i ){
        vector<int> qmap=data.UVMaps[i];
        PackedInt32Array map;
        for(size_t n=0;n<qmap.size();++n ){
            map.push_back(qmap[n] );
        }
        
        
        dataUVMaps.push_back( map );
    }

}

Rect2 QMeshNode::get_aabb_of_polygon(PackedVector2Array polygonPoints)
{
    float minX=MAXFLOAT;
	float minY=MAXFLOAT;
	float maxX=-MAXFLOAT;
	float maxY=-MAXFLOAT;
    for(int n=0;n<polygonPoints.size();n++){
        Vector2 p=polygonPoints[n];

        if(p.x<minX)
            minX=p.x;
        if(p.y<minY)
            minY=p.y;
        if(p.x>maxX)
            maxX=p.x;
        if(p.y>maxY)
            maxY=p.y;
    }
    Vector2 minPos=Vector2(minX,minY);
    Vector2 maxPos=Vector2(maxX,maxY);
    Vector2 rectPos=minPos;
    Vector2 rectSize=maxPos-minPos;
    return Rect2(rectPos,rectSize);
}

void QMeshNode::_bind_methods()
{
    //Get
    ClassDB::bind_method(D_METHOD("get_mesh_position"),&QMeshNode::get_mesh_position );
    ClassDB::bind_method(D_METHOD("get_mesh_global_position"),&QMeshNode::get_mesh_global_position );
    ClassDB::bind_method(D_METHOD("get_mesh_rotation"),&QMeshNode::get_mesh_rotation );
    ClassDB::bind_method(D_METHOD("get_mesh_global_rotation"),&QMeshNode::get_mesh_global_rotation );
    ClassDB::bind_method(D_METHOD("get_initial_area"),&QMeshNode::get_initial_area );
    ClassDB::bind_method(D_METHOD("get_initial_polygons_area"),&QMeshNode::get_initial_polygons_area );
    ClassDB::bind_method(D_METHOD("get_area"),&QMeshNode::get_area );
    ClassDB::bind_method(D_METHOD("get_polygon_area"),&QMeshNode::get_polygon_area );
    ClassDB::bind_method(D_METHOD("get_circumference"),&QMeshNode::get_circumference );
    ClassDB::bind_method(D_METHOD("get_owner_body_node"),&QMeshNode::get_owner_body_node );
    ClassDB::bind_method(D_METHOD("get_show_springs_enabled"),&QMeshNode::get_show_springs_enabled );
    ClassDB::bind_method(D_METHOD("get_show_polygon_enabled"),&QMeshNode::get_show_polygon_enabled );
    ClassDB::bind_method(D_METHOD("get_show_particle_index_numbers_enabled"),&QMeshNode::get_show_particle_index_numbers_enabled );
    ClassDB::bind_method(D_METHOD("get_polygon_for_collision_disabled"),&QMeshNode::get_polygon_for_collision_disabled );
    //Set
    ClassDB::bind_method(D_METHOD("set_mesh_position"),&QMeshNode::set_mesh_position );
    ClassDB::bind_method(D_METHOD("set_mesh_global_position"),&QMeshNode::set_mesh_global_position );
    ClassDB::bind_method(D_METHOD("set_mesh_rotation"),&QMeshNode::set_mesh_rotation );
    ClassDB::bind_method(D_METHOD("set_show_springs_enabled","value"),&QMeshNode::set_show_springs_enabled );
    ClassDB::bind_method(D_METHOD("set_show_polygon_enabled","value"),&QMeshNode::set_show_polygon_enabled );
    ClassDB::bind_method(D_METHOD("set_show_particle_index_numbers_enabled","value"),&QMeshNode::set_show_particle_index_numbers_enabled );
    ClassDB::bind_method(D_METHOD("set_polygon_for_collision_disabled","value"),&QMeshNode::set_polygon_for_collision_disabled );
    //Particle Operations
    ClassDB::bind_method(D_METHOD("add_particle","particle_object"),&QMeshNode::add_particle );
    ClassDB::bind_method(D_METHOD("remove_particle","particle_object"),&QMeshNode::remove_particle );
    ClassDB::bind_method(D_METHOD("remove_particle_at","index"),&QMeshNode::remove_particle_at );
    ClassDB::bind_method(D_METHOD("get_particle_at","index"),&QMeshNode::get_particle_at );
    ClassDB::bind_method(D_METHOD("get_particle_count"),&QMeshNode::get_particle_count );
    ClassDB::bind_method(D_METHOD("get_particle_index","particle_object"),&QMeshNode::get_particle_index );
    ClassDB::bind_method(D_METHOD("get_average_position_and_rotation","particle_collection"),&QMeshNode::get_average_position_and_rotation );
    ClassDB::bind_method(D_METHOD("get_matching_particle_positions","particle_collection","target_position","target_rotation"),&QMeshNode::get_matching_particle_positions );

    //Spring Operations
    ClassDB::bind_method(D_METHOD("add_spring","spring_object"),&QMeshNode::add_spring );
    ClassDB::bind_method(D_METHOD("remove_spring","spring_object"),&QMeshNode::remove_spring );
    ClassDB::bind_method(D_METHOD("remove_spring_at","index"),&QMeshNode::remove_spring_at );
    ClassDB::bind_method(D_METHOD("get_spring_at","index"),&QMeshNode::get_spring_at );
    ClassDB::bind_method(D_METHOD("get_spring_count"),&QMeshNode::get_spring_count );
    ClassDB::bind_method(D_METHOD("get_spring_index","spring_object"),&QMeshNode::get_spring_index );

    //Polygon Operations
    ClassDB::bind_method(D_METHOD("set_polygon","particleCollection"),&QMeshNode::set_polygon );
    ClassDB::bind_method(D_METHOD("add_particle_to_polygon","particleObject"),&QMeshNode::add_particle_to_polygon );
    ClassDB::bind_method(D_METHOD("remove_particle_from_polygon","particleObject"),&QMeshNode::remove_particle_from_polygon );
    ClassDB::bind_method(D_METHOD("remove_particle_from_polygon_at","index"),&QMeshNode::remove_particle_from_polygon_at );
    ClassDB::bind_method(D_METHOD("remove_polygon"),&QMeshNode::remove_polygon );
    ClassDB::bind_method(D_METHOD("get_particle_from_polygon","index"),&QMeshNode::get_particle_from_polygon );
    ClassDB::bind_method(D_METHOD("get_polygon_particle_count"),&QMeshNode::get_polygon_particle_count );
    ClassDB::bind_method(D_METHOD("get_min_angle_constraint_of_polygon"),&QMeshNode::get_min_angle_constraint_of_polygon );
    ClassDB::bind_method(D_METHOD("set_min_angle_constraint_of_polygon","value"),&QMeshNode::set_min_angle_constraint_of_polygon );
    ClassDB::bind_method(D_METHOD("get_decomposited_polygon"),&QMeshNode::get_decomposited_polygon );
    ClassDB::bind_method(D_METHOD("get_aabb_of_polygon","polygonPoints"),&QMeshNode::get_aabb_of_polygon );


    //UV Operations
    ClassDB::bind_method(D_METHOD("get_uv_map_at","index"),&QMeshNode::get_uv_map_at );
    ClassDB::bind_method(D_METHOD("get_uv_map_count"),&QMeshNode::get_uv_map_count );
    ClassDB::bind_method(D_METHOD("add_uv_map","map"),&QMeshNode::add_uv_map );
    ClassDB::bind_method(D_METHOD("remove_uv_map_at","index"),&QMeshNode::remove_uv_map_at );


    //Angle Constraint Operations
    ClassDB::bind_method(D_METHOD("add_angle_constraint","angle_constraint_object"),&QMeshNode::add_angle_constraint );
    ClassDB::bind_method(D_METHOD("remove_angle_constraint","angle_constraint_object"),&QMeshNode::remove_angle_constraint );
    ClassDB::bind_method(D_METHOD("remove_angle_constraint_at","index"),&QMeshNode::remove_angle_constraint_at );
    ClassDB::bind_method(D_METHOD("get_angle_constraint_at","index"),&QMeshNode::get_angle_constraint_at );
    ClassDB::bind_method(D_METHOD("get_angle_constraint_count"),&QMeshNode::get_angle_constraint_count );
    ClassDB::bind_method(D_METHOD("get_angle_constraint_index","angle_constraint_object"),&QMeshNode::get_angle_constraint_index );
    

    

    //Rendering features
     ClassDB::bind_method(D_METHOD("get_vector_rendering_enabled"),&QMeshNode::get_vector_rendering_enabled );
     ClassDB::bind_method(D_METHOD("get_antialias_enabled"),&QMeshNode::get_antialias_enabled );
     ClassDB::bind_method(D_METHOD("get_triangulation_enabled"),&QMeshNode::get_triangulation_enabled );
     ClassDB::bind_method(D_METHOD("get_fill_enabled"),&QMeshNode::get_fill_enabled );
     ClassDB::bind_method(D_METHOD("get_fill_color"),&QMeshNode::get_fill_color );
     ClassDB::bind_method(D_METHOD("get_fill_texture"),&QMeshNode::get_fill_texture );
     ClassDB::bind_method(D_METHOD("get_stretching_texture_to_polygon_enabled"),&QMeshNode::get_stretching_texture_to_polygon_enabled );
     ClassDB::bind_method(D_METHOD("get_fill_texture_scale"),&QMeshNode::get_fill_texture_scale );
     ClassDB::bind_method(D_METHOD("get_fill_texture_offset"),&QMeshNode::get_fill_texture_offset );
     ClassDB::bind_method(D_METHOD("get_stroke_enabled"),&QMeshNode::get_stroke_enabled );
     ClassDB::bind_method(D_METHOD("get_stroke_width"),&QMeshNode::get_stroke_width );
     ClassDB::bind_method(D_METHOD("get_stroke_color"),&QMeshNode::get_stroke_color );
     ClassDB::bind_method(D_METHOD("get_stroke_offset"),&QMeshNode::get_stroke_offset );
     ClassDB::bind_method(D_METHOD("get_curved_corners_enabled"),&QMeshNode::get_curved_corners_enabled );
     ClassDB::bind_method(D_METHOD("get_curve_length"),&QMeshNode::get_curve_length );
     ClassDB::bind_method(D_METHOD("get_particle_rendering_enabled"),&QMeshNode::get_particle_rendering_enabled );

     //Render helper operations
     ClassDB::bind_method(D_METHOD("get_curved_polygon_points","polygonPoints","curve_amount","margin","origin","bake_interval"),&QMeshNode::get_curved_polygon_points );
     ClassDB::bind_method(D_METHOD("get_inner_shadow_of_polygon","polygon_points","offset"),&QMeshNode::get_inner_shadow_of_polygon );
     ClassDB::bind_method(D_METHOD("get_specified_side_points_of_polygon","polygon_points","axis","points_count_range","scale", "origin"),&QMeshNode::get_specified_side_points_of_polygon );

     

     ClassDB::bind_method(D_METHOD("set_vector_rendering_enabled","value"),&QMeshNode::set_vector_rendering_enabled );
     ClassDB::bind_method(D_METHOD("set_antialias_enabled","value"),&QMeshNode::set_antialias_enabled );
     ClassDB::bind_method(D_METHOD("set_triangulation_enabled","value"),&QMeshNode::set_triangulation_enabled );
     ClassDB::bind_method(D_METHOD("set_fill_enabled","value"),&QMeshNode::set_fill_enabled );
     ClassDB::bind_method(D_METHOD("set_fill_color","value"),&QMeshNode::set_fill_color );
     ClassDB::bind_method(D_METHOD("set_fill_texture","texture"),&QMeshNode::set_fill_texture );
     ClassDB::bind_method(D_METHOD("set_stretching_texture_to_polygon_enabled","value"),&QMeshNode::set_stretching_texture_to_polygon_enabled );
     ClassDB::bind_method(D_METHOD("set_fill_texture_scale","value"),&QMeshNode::set_fill_texture_scale );
     ClassDB::bind_method(D_METHOD("set_fill_texture_offset","value"),&QMeshNode::set_fill_texture_offset );
     ClassDB::bind_method(D_METHOD("set_stroke_enabled","value"),&QMeshNode::set_stroke_enabled );
     ClassDB::bind_method(D_METHOD("set_stroke_width","value"),&QMeshNode::set_stroke_width );
     ClassDB::bind_method(D_METHOD("set_stroke_color","value"),&QMeshNode::set_stroke_color );
     ClassDB::bind_method(D_METHOD("set_stroke_offset","value"),&QMeshNode::set_stroke_offset );
     ClassDB::bind_method(D_METHOD("set_curved_corners_enabled","value"),&QMeshNode::set_curved_corners_enabled );
     ClassDB::bind_method(D_METHOD("set_curve_length","value"),&QMeshNode::set_curve_length );
     ClassDB::bind_method(D_METHOD("set_particle_rendering_enabled","value"),&QMeshNode::set_particle_rendering_enabled );


    //Data
    ClassDB::bind_method(D_METHOD("set_data_particle_positions","collection"),&QMeshNode::set_data_particle_positions );
    ClassDB::bind_method(D_METHOD("get_data_particle_positions"),&QMeshNode::get_data_particle_positions);

    ClassDB::bind_method(D_METHOD("set_data_particle_radius","collection"),&QMeshNode::set_data_particle_radius );
    ClassDB::bind_method(D_METHOD("get_data_particle_radius"),&QMeshNode::get_data_particle_radius);

    ClassDB::bind_method(D_METHOD("set_data_particle_is_internal","collection"),&QMeshNode::set_data_particle_is_internal );
    ClassDB::bind_method(D_METHOD("get_data_particle_is_internal"),&QMeshNode::get_data_particle_is_internal);

    ClassDB::bind_method(D_METHOD("set_data_particle_is_enabled","collection"),&QMeshNode::set_data_particle_is_enabled );
    ClassDB::bind_method(D_METHOD("get_data_particle_is_enabled"),&QMeshNode::get_data_particle_is_enabled);

    ClassDB::bind_method(D_METHOD("set_data_particle_is_lazy","collection"),&QMeshNode::set_data_particle_is_lazy );
    ClassDB::bind_method(D_METHOD("get_data_particle_is_lazy"),&QMeshNode::get_data_particle_is_lazy);

    ClassDB::bind_method(D_METHOD("set_data_springs","collection"),&QMeshNode::set_data_springs );
    ClassDB::bind_method(D_METHOD("get_data_springs"),&QMeshNode::get_data_springs);

    ClassDB::bind_method(D_METHOD("set_data_internal_springs","collection"),&QMeshNode::set_data_internal_springs );
    ClassDB::bind_method(D_METHOD("get_data_internal_springs"),&QMeshNode::get_data_internal_springs);

    ClassDB::bind_method(D_METHOD("set_data_polygon","collection"),&QMeshNode::set_data_polygon );
    ClassDB::bind_method(D_METHOD("get_data_polygon"),&QMeshNode::get_data_polygon);

    ClassDB::bind_method(D_METHOD("set_data_uv_maps","collection"),&QMeshNode::set_data_uv_maps );
    ClassDB::bind_method(D_METHOD("get_data_uv_maps"),&QMeshNode::get_data_uv_maps);
     
     

    //Props
    ADD_PROPERTY( PropertyInfo(Variant::BOOL , "disable_polygon_for_collisions"), "set_polygon_for_collision_disabled","get_polygon_for_collision_disabled" );

    ADD_GROUP("Debug Rendering","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL , "show_springs"), "set_show_springs_enabled","get_show_springs_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL , "show_polygons"), "set_show_polygon_enabled","get_show_polygon_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL , "show_particle_index_numbers"), "set_show_particle_index_numbers_enabled","get_show_particle_index_numbers_enabled" );

    ADD_GROUP("Vector Rendering","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_vector_rendering"),"set_vector_rendering_enabled","get_vector_rendering_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "antialias"),"set_antialias_enabled","get_antialias_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "triangulation"),"set_triangulation_enabled","get_triangulation_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_fill"),"set_fill_enabled","get_fill_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::COLOR, "fill_color"),"set_fill_color","get_fill_color" );
    ADD_PROPERTY( PropertyInfo(Variant::OBJECT, "fill_texture",PROPERTY_HINT_RESOURCE_TYPE,"Texture2D"),"set_fill_texture","get_fill_texture" );
    ADD_SUBGROUP("Texture Properties","");
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_stretching_texture_to_polygon"),"set_stretching_texture_to_polygon_enabled","get_stretching_texture_to_polygon_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "fill_texture_scale",PROPERTY_HINT_RANGE,"-10.0,10.0,0.1"),"set_fill_texture_scale","get_fill_texture_scale" );
    ADD_PROPERTY( PropertyInfo(Variant::VECTOR2, "fill_texture_offset"),"set_fill_texture_offset","get_fill_texture_offset" );
    ADD_SUBGROUP("","");
    
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_stroke"),"set_stroke_enabled","get_stroke_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "stroke_width",PROPERTY_HINT_RANGE,"0,9999,0.5"),"set_stroke_width","get_stroke_width" );
    ADD_PROPERTY( PropertyInfo(Variant::COLOR, "stroke_color"),"set_stroke_color","get_stroke_color" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "stroke_offset",PROPERTY_HINT_RANGE,"-256.0,256.0,0.25"),"set_stroke_offset","get_stroke_offset" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_curved_corners"),"set_curved_corners_enabled","get_curved_corners_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "curve_length"),"set_curve_length","get_curve_length" );
    ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_particle_rendering"),"set_particle_rendering_enabled","get_particle_rendering_enabled" );

    ADD_GROUP("Data","");
    ADD_PROPERTY( PropertyInfo(Variant::PACKED_VECTOR2_ARRAY, "data_particle_positions"),"set_data_particle_positions","get_data_particle_positions" );
    ADD_PROPERTY( PropertyInfo(Variant::PACKED_FLOAT32_ARRAY, "data_particle_radius"),"set_data_particle_radius","get_data_particle_radius" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_particle_is_internal", PROPERTY_HINT_ARRAY_TYPE,"bool"),"set_data_particle_is_internal","get_data_particle_is_internal" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_particle_is_enabled", PROPERTY_HINT_ARRAY_TYPE,"bool"),"set_data_particle_is_enabled","get_data_particle_is_enabled" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_particle_is_lazy", PROPERTY_HINT_ARRAY_TYPE,"bool"),"set_data_particle_is_lazy","get_data_particle_is_lazy" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_springs", PROPERTY_HINT_ARRAY_TYPE,"PackedInt32Array"),"set_data_springs","get_data_springs" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_internal_springs", PROPERTY_HINT_ARRAY_TYPE,"PackedInt32Array"),"set_data_internal_springs","get_data_internal_springs" );
    ADD_PROPERTY( PropertyInfo(Variant::PACKED_INT32_ARRAY, "data_polygon"),"set_data_polygon","get_data_polygon" );
    ADD_PROPERTY( PropertyInfo(Variant::ARRAY, "data_uv_maps", PROPERTY_HINT_ARRAY_TYPE,"PackedInt32Array"),"set_data_uv_maps","get_data_uv_maps" );

    
}
//GET

Vector2 QMeshNode::get_mesh_position() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_mesh_position() ");
        return Vector2(0,0);
    }
    QVector value=meshObject->GetPosition();
	return Vector2(value.x,value.y);
}

Vector2 QMeshNode::get_mesh_global_position() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_mesh_global_position() ");
        return Vector2(0,0);
    }
    QVector value=meshObject->GetGlobalPosition();
	return Vector2(value.x,value.y);
}

float QMeshNode::get_mesh_rotation() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_mesh_rotation() ");
        return 0.0;
    }
	return meshObject->GetRotation();
}

float QMeshNode::get_mesh_global_rotation() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_mesh_global_rotation() ");
        return 0.0;
    }
	return meshObject->GetGlobalRotation();
}

float QMeshNode::get_initial_area() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_initial_area() ");
        return 0.0;
    }
	return meshObject->GetInitialArea();
}

float QMeshNode::get_initial_polygons_area() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_initial_polygons_area() ");
        return 0.0;
    }
	return meshObject->GetInitialPolygonsArea();
}

float QMeshNode::get_area() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_area() ");
        return 0.0;
    }
	return meshObject->GetArea();
}

float QMeshNode::get_polygon_area() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_polygon_area() ");
        return 0.0;
    }
	return meshObject->GetPolygonsArea();
}

float QMeshNode::get_circumference(){
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_circumference() ");
        return 0.0;
    }
    return meshObject->GetCircumference();
}

bool QMeshNode::get_show_springs_enabled() {
	return showSprings;
}

bool QMeshNode::get_show_polygon_enabled() {
	return showPolygon;
}

bool QMeshNode::get_show_particle_index_numbers_enabled()
{
    return showParticleIndexNumbers;
}

Array QMeshNode::get_average_position_and_rotation(TypedArray<QParticleObject> particle_collection) {
    
    Array result;

    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_average_position_and_rotation() ");
        return result;
    }
    vector<QParticle*> particleObjects;
    
    for(int i=0;i<particle_collection.size();i++){
        Ref<QParticleObject> pObj=particle_collection[i];
        if(pObj.is_valid()==false || pObj==nullptr){
            godot::UtilityFunctions::printerr("Quark Physics Error: There are an invalid object type in the particle collection! | QMeshNode.get_average_position_and_rotation() ");
            return result;
        }
        particleObjects.push_back(pObj->particleObject);
    }
    pair<QVector,float> resultRaw=meshObject->GetAveragePositionAndRotation(particleObjects);
    result.append(Vector2(resultRaw.first.x,resultRaw.first.y ));
    result.append(resultRaw.second);
    
	return result;
}

Array QMeshNode::get_matching_particle_positions(TypedArray<QParticleObject> particle_collection, Vector2 target_position, float target_rotation) {
    
	Array result;
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_matching_particle_positions() ");
        return result;
    }
    vector<QParticle*> particleObjects;
    for(int i=0;i<particle_collection.size();i++){
        Ref<QParticleObject> pObj=particle_collection[i];
        if(pObj.is_valid()==false || pObj==nullptr){
            godot::UtilityFunctions::printerr("Quark Physics Error: The element of the particle_collection array isn't a valid particle object type! | QMeshNode.get_matching_particle_positions() ");
            return result;
        }

        particleObjects.push_back(pObj->particleObject);
    }
    vector<QVector> resultRaw=meshObject->GetMatchingParticlePositions(particleObjects,QVector(target_position.x,target_position.y),target_rotation );
    for(int i=0;i<resultRaw.size();++i ){
        result.append(Vector2(resultRaw[i].x,resultRaw[i].y) );
    }
    return result;
}

float QMeshNode::get_min_angle_constraint_of_polygon() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_min_angle_constraint_of_polygon() ");
        return 0.0f;
    }
	return meshObject->GetMinAngleConstraintOfPolygon();
}

bool QMeshNode::get_polygon_for_collision_disabled()
{
    return disablePolygonForCollisions;
}

QBodyNode * QMeshNode::get_owner_body_node(){
    return ownerBodyNode;
}

//SET

QMeshNode *QMeshNode::set_mesh_position(Vector2 value) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.set_mesh_position() ");
        return this;
    }
    meshObject->SetPosition(QVector(value.x,value.y) );
	return this;
}
QMeshNode *QMeshNode::set_mesh_global_position(Vector2 value) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.set_mesh_global_position() ");
        return this;
    }
    meshObject->SetGlobalPosition(QVector(value.x,value.y) );
	return this;
}

QMeshNode *QMeshNode::set_mesh_rotation(float value) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.set_mesh_rotation() ");
        return this;
    }
    meshObject->SetRotation(value);
	return this;
}

QMeshNode *QMeshNode::set_show_springs_enabled(bool value) {
    showSprings=value;
    queue_redraw();
	return this;
}

QMeshNode *QMeshNode::set_show_polygon_enabled(bool value) {
	showPolygon=value;
    queue_redraw();
	return this;
}

QMeshNode *QMeshNode::set_show_particle_index_numbers_enabled(bool value)
{
    showParticleIndexNumbers=value;
    queue_redraw();
    return this;
}

QMeshNode *QMeshNode::set_min_angle_constraint_of_polygon(bool value) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.set_min_angle_constraint_of_polygon() ");
        return this;
    }
	meshObject->SetMinAngleConstraintOfPolygon(value);
    return this;
}

QMeshNode *QMeshNode::set_polygon_for_collision_disabled(bool value)
{
    
    disablePolygonForCollisions=value;
    if ( Engine::get_singleton()->is_editor_hint()==false && meshObject!=nullptr ){
        meshObject->SetPolygonForCollisionsDisabled(value);
    }
    return this;
}

//Particle Operations
QMeshNode *QMeshNode::add_particle(Ref<QParticleObject> particle_object) {
    if(particle_object.is_valid()==false ){
        godot::UtilityFunctions::printerr("Quark Physics Error: Not a valid particle object type! | QMeshNode.add_particle() ");
        return this;
    }
    if(meshObject==nullptr){
        meshObject=new QMesh();
        meshObject->manualDeletion=true;
    }
    particleObjects.push_back(particle_object);
    meshObject->AddParticle(particle_object->particleObject);
    
	return this;
}

QMeshNode *QMeshNode::remove_particle(Ref<QParticleObject> particle_object) {
    if(particle_object.is_valid()==false){
        godot::UtilityFunctions::printerr("Quark Physics Error: Not a valid particle object type! | QMeshNode.remove_particle() ");
        return this;
    }

    auto findedIt=find(particleObjects.begin(),particleObjects.end(),particle_object );
    if(findedIt!=particleObjects.end()){
        int index=findedIt-particleObjects.begin();
        remove_particle_at(index);
    }
	return this;
}

QMeshNode *QMeshNode::remove_particle_at(int index) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.remove_particle_at() ");
        return this;
    }

    Ref<QParticleObject> particle_object=particleObjects[index];
    //Remove linked springs
    int i=0;
    while(i<springObjects.size()){
        Ref<QSpringObject> sp=springObjects[i];
        if(sp->get_particle_a()==particle_object || sp->get_particle_b()==particle_object){
            remove_spring_at(i);
        }else{
            ++i;
        }
       
    }

    //Remove linked angle constraints
    i=0;
    while(i<angleConstraintObjects.size()){
        Ref<QAngleConstraintObject> ac=angleConstraintObjects[i];
        if(ac->get_particle_a()==particle_object || ac->get_particle_b()==particle_object || ac->get_particle_c()==particle_object){
            remove_angle_constraint_at(i);
        }else{
            ++i;
        }
    }

    //Remove linked polygons
    
    
    vector<Ref<QParticleObject> > &pol=polygon;
    bool matched=false;
    int n=0;
    while(n<pol.size() ){
        if(pol[n]==particle_object){
            remove_particle_from_polygon_at(n);
            matched=true;
        }else{
            ++n;
        }
    }
    if(matched==true && pol.size()<3){
        while(get_polygon_particle_count()>0 ){
            remove_particle_from_polygon_at(0);
        }
    }
    

    particleObjects.erase(particleObjects.begin()+index);
    meshObject->RemoveParticleAt(index);
	return this;
}

Ref<QParticleObject> QMeshNode::get_particle_at(int index) {
	return particleObjects[index];
}

int QMeshNode::get_particle_count() {
	return particleObjects.size();
}

int QMeshNode::get_particle_index(Ref<QParticleObject> particle_object)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_particle_index() ");
        return -1;
    }
    return meshObject->GetParticleIndex(particle_object->particleObject);
}

Ref<QParticleObject> QMeshNode::get_particle_object_with_particle(QParticle *particle) {
    for(auto particleObject:particleObjects){
        if(particleObject->particleObject==particle){
            return particleObject;
        }
    }
	return nullptr;
}

//Spring Operations
QMeshNode *QMeshNode::add_spring(Ref<QSpringObject> spring_object) {
	if(spring_object.is_valid()==false){
        godot::UtilityFunctions::printerr("Quark Physics Error: Not a valid spring object type! | QMeshNode.add_spring() ");
        return this;
    }
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.add_spring() ");
        return this;
    }else{
        if (meshObject->GetParticleCount()==0){
            godot::UtilityFunctions::printerr("Quark Physics Error: There aren't particle objects to create a new spring! | QMeshNode.add_spring() ");
            return this;
        }
    }
    
    springObjects.push_back(spring_object);
    meshObject->AddSpring(spring_object->springObject);
	return this;
}

QMeshNode *QMeshNode::remove_spring(Ref<QSpringObject> spring_object) {
	if(spring_object->get_class()!="QSpringObject"){
        godot::UtilityFunctions::printerr("Quark Physics Error: Not a valid particle object type! | QMeshNode.remove_spring() ");
        return this;
    }
    
    auto findedIt=find(springObjects.begin(),springObjects.end(),spring_object );
    if(findedIt!=springObjects.end()){
        int index=findedIt-springObjects.begin();
        remove_spring_at(index);
    }
	return this;
}

QMeshNode *QMeshNode::remove_spring_at(int index) {
    if(index>=springObjects.size() ){
        godot::UtilityFunctions::printerr("Quark Physics Error: The index value is out of the range!! | QMeshNode.remove_spring_at() ");
        return this;
    }
	springObjects.erase(springObjects.begin()+index);
    meshObject->RemoveSpringAt(index);
	return this;
}

Ref<QSpringObject> QMeshNode::get_spring_at(int index) {
	return springObjects[index];
}

int QMeshNode::get_spring_count() {
	return springObjects.size();
}

int QMeshNode::get_spring_index(Ref<QSpringObject> spring_object)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_spring_index() ");
        return -1;
    }
    return meshObject->GetSpringIndex(spring_object->springObject);
}

//Polygon Operations

QMeshNode *QMeshNode::set_polygon(TypedArray<QParticleObject> particleCollection) {

    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.set_polygon() ");
        return this;
    }else{
        if (meshObject->GetParticleCount()==0){
            godot::UtilityFunctions::printerr("Quark Physics Error: There aren't particle objects to create a new polygon! | QMeshNode.set_polygon() ");
            return this;
        }
    }

	vector<Ref<QParticleObject>> polygonParticleObjects;
    vector<QParticle*> polygonParticles;

    for(int i=0;i<particleCollection.size();i++){
        
        Ref<QParticleObject> element=particleCollection[i];
        if(element.is_valid()==false || element==nullptr ){
            godot::UtilityFunctions::printerr("Quark Physics Error: The element of the polygon array isn't a valid particle object type! | QMeshNode.set_polygon() ");
            return this;
        }
        polygonParticleObjects.push_back(element);
        polygonParticles.push_back(element->particleObject);
    }

    polygon=polygonParticleObjects;
    meshObject->SetPolygon(polygonParticles);

    return  this;
}

QMeshNode *QMeshNode::add_particle_to_polygon(Ref<QParticleObject> particleObject) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.add_particle_to_polygon() ");
        return this;
    }else{
        if (meshObject->GetParticleCount()==0){
            godot::UtilityFunctions::printerr("Quark Physics Error: There aren't particle objects to add a new particle to the polygon! | QMeshNode.add_particle_to_polygon() ");
            return this;
        }
    }
    
    if(particleObject.is_valid()==false){
        godot::UtilityFunctions::printerr("Quark Physics Error: There is no a valid particle object type! | QMeshNode.add_particle_to_polygon() ");
        return this;
    }

    
    
    polygon.push_back(particleObject);
    meshObject->AddParticleToPolygon(particleObject->particleObject);

	return this;
}

QMeshNode *QMeshNode::remove_particle_from_polygon(Ref<QParticleObject> particleObject) {
       

    auto findedIt=find(polygon.begin(),polygon.end(),particleObject );
    if(findedIt!=polygon.end() ){
        int index=findedIt-polygon.begin();
        remove_particle_from_polygon_at(index);
    }


	return this;
}

QMeshNode *QMeshNode::remove_particle_from_polygon_at(int index) {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.remove_particle_from_polygon_at() ");
        return this;
    }
    if(index>=polygon.size() ){
        godot::UtilityFunctions::printerr("Quark Physics Error: The index value is out of the range! | QMeshNode.remove_particle_from_polygon_at() ");
        return this;
    }
    
    Ref<QParticleObject> po=polygon[index];
    meshObject->RemoveParticleFromPolygonAt(index);
    polygon.erase(polygon.begin()+index );
    
	return this;
}

QMeshNode *QMeshNode::remove_polygon() {
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.remove_polygon() ");
        return this;
    }
    meshObject->RemovePolygon();
    polygon.clear();
	return this;
}

Ref<QParticleObject> QMeshNode::get_particle_from_polygon(int index) {
    if(index>=polygon.size() ){
        godot::UtilityFunctions::printerr("Quark Physics Error: The index value is out of the range! | QMeshNode.get_particle_from_polygon() ");
        return nullptr;
    }
	return polygon[index];
}

int QMeshNode::get_polygon_particle_count() {
	return polygon.size() ;
}



Array QMeshNode::get_decomposited_polygon() {
    Array result;
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_decomposited_polygon() ");
        return result;
    }
    PackedVector2Array polygonPoints;
    for (size_t i=0;i<meshObject->GetPolygonParticleCount();++i ){
        QVector p=meshObject->GetParticleFromPolygon(i)->GetGlobalPosition();
        polygonPoints.push_back( Vector2(p.x,p.y) );

    }

    Array decompositedPolygon=Geometry2D::get_singleton()->decompose_polygon_in_convex(polygonPoints);

    

    for(int i=0;i<decompositedPolygon.size();++i ){
        PackedVector2Array polyOrg=decompositedPolygon[i];
        Array poly;
        for(size_t n=0;n<polyOrg.size();++n ){
            poly.append(polyOrg[n]);
        }
        result.append(poly);
    }

	return result;
}


//UV Operations

int QMeshNode::get_uv_map_count(){
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_uv_map_count() ");
        return 0;
    }
    return meshObject->GetUVMapCount();
    
}

PackedInt32Array QMeshNode::get_uv_map_at(int index){
    if(meshObject==nullptr){
        
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_uv_map_at() ");
     
        return PackedInt32Array();
    }
    PackedInt32Array res;
    vector<int> map=meshObject->GetUVMapAt(index);
    for(size_t i=0;i<map.size();++i){
        res.append(map[i]);
    }
    
    return res;
}

QMeshNode *QMeshNode::add_uv_map(PackedInt32Array map)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.add_uv_map() ");
        return this;
    }
    vector<int> vmap;
    for(size_t i=0;i<map.size();++i){
        vmap.push_back(map[i]);
    }
    meshObject->AddUVMap(vmap);
    return this;
}

QMeshNode *QMeshNode::remove_uv_map_at(int index)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.remove_uv_map_at() ");
        return this;
    }
    meshObject->RemoveUVMapAt(index);
    
    return this;
}

int QMeshNode::get_angle_constraint_count()
{
    return angleConstraintObjects.size();
}

Ref<QAngleConstraintObject> QMeshNode::get_angle_constraint_at(int index)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_angle_constraint_at ");
        return nullptr;
    }
    return angleConstraintObjects[index];
}

QMeshNode *QMeshNode::add_angle_constraint(Ref<QAngleConstraintObject> angleConstraint)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.add_angle_constraint() ");
        return this;
    }
    angleConstraintObjects.push_back(angleConstraint);
    meshObject->AddAngleConstraint(angleConstraint->angleConstraintObject);
    return this;
}

int QMeshNode::get_angle_constraint_index(Ref<QAngleConstraintObject> angleConstraint)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.get_angle_constraint_index() ");
        return -1;
    }
    return meshObject->GetAngleConstraintIndex(angleConstraint->angleConstraintObject);
}

QMeshNode *QMeshNode::remove_angle_constraint(Ref<QAngleConstraintObject> angleConstraint)
{
    if(meshObject==nullptr){
        godot::UtilityFunctions::printerr("Quark Physics Error: The mesh object not configured yet! | QMeshNode.remove_angle_constraint() ");
        return this;
    }

    auto findedIt=find(angleConstraintObjects.begin(),angleConstraintObjects.end(),angleConstraint );
    if(findedIt!=angleConstraintObjects.end()){
        int index=findedIt-angleConstraintObjects.begin();
        remove_angle_constraint_at(index);
    }

    return this;
}

QMeshNode *QMeshNode::remove_angle_constraint_at(int index)
{
    if(index>=angleConstraintObjects.size() ){
        godot::UtilityFunctions::printerr("Quark Physics Error: The index value is out of the range!! | QMeshNode.remove_angle_constraint_at() ");
        return this;
    }

    angleConstraintObjects.erase(angleConstraintObjects.begin()+index);
    meshObject->RemoveAngleConstraintAt(index);

    return this;
}

//Render Helper Operations

PackedVector2Array QMeshNode::get_curved_polygon_points(PackedVector2Array polygonPoints, float curve_amount,float margin,Vector2 origin,float bake_interval) {
    PackedVector2Array res;
    size_t polygonSize=polygonPoints.size();
    if(polygonSize<3 ){
        return res;
    }
    curvedPolygon->clear_points();
    Vector2 firstPoint;
    Vector2 firstCurveVec;
    curvedPolygon->set_bake_interval(bake_interval);
	for (size_t i=0;i<polygonSize;++i ){
        Vector2 pp=polygonPoints[ (i-1+polygonSize) % polygonSize ];
        Vector2 cp=polygonPoints[i];
        Vector2 np=polygonPoints[ (i+1) % polygonSize ];
        Vector2 curveUnit=(np-pp).normalized();
        Vector2 curveNormal=curveUnit.orthogonal();
        Vector2 curveVec=curveUnit*curve_amount;

        Vector2 finalPoint=(cp-origin)-curveNormal*margin; 

        if(i==0){
            firstPoint=finalPoint;
            firstCurveVec=curveVec;
        }

        curvedPolygon->add_point(finalPoint,-curveVec,curveVec);
    }
    curvedPolygon->add_point(firstPoint,-firstCurveVec,firstCurveVec);
    
    return curvedPolygon->get_baked_points();
}

PackedVector2Array QMeshNode::get_inner_shadow_of_polygon(PackedVector2Array polygon_points,Vector2 offset) {
    PackedVector2Array clipVertices;
    for (size_t i=0;i<polygon_points.size();++i ){
        clipVertices.push_back(polygon_points[i]+offset);
    }
    
    Array clippedPolygons=Geometry2D::get_singleton()->clip_polygons(polygon_points,clipVertices);
    PackedVector2Array shadowVertices;
    if (clippedPolygons.size()>0 ){
        shadowVertices=clippedPolygons[0];
    }

	return shadowVertices;
}

PackedVector2Array QMeshNode::get_specified_side_points_of_polygon(PackedVector2Array polygon_points, Vector2 axis, int points_count_range, float scale,Vector2 origin) {
    PackedVector2Array res;
    size_t p_size=polygon_points.size();
    if (p_size<3 ){
        return res;
    }
    size_t extreme_point_index;
    float max_dist=-INFINITY;
    for (size_t i=0;i<p_size;++i ){
        Vector2 p=polygon_points[i];
        float dist=p.dot(axis);
        if(dist>max_dist){
            max_dist=dist;
            extreme_point_index=i;
        }
    }
    size_t current_index=(extreme_point_index-points_count_range+p_size)%p_size;
    size_t end_index=(extreme_point_index+points_count_range)%p_size;
    while (current_index!=end_index){
        Vector2 p=( polygon_points[current_index]-origin )*scale+origin;
        res.push_back(p);
        current_index=(current_index+1)%p_size;
    }

	return res;
}





QMeshNode *QMeshNode::type_cast(Object *obj) {
	Node2D *node=Object::cast_to<Node2D>(obj);
    if(node!=nullptr){
        if(node->get_class()=="QMeshNode" || node->get_class()=="QMeshRectNode" || node->get_class()=="QMeshPolygonNode" || node->get_class()=="QMeshCircleNode" || node->get_class()=="QMeshAdvancedNode"){
            return static_cast<QMeshNode*>(node);
        }
    }
    return nullptr;
}

//PARTICLES





