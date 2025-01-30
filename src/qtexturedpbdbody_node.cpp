#include "qtexturedpbdbody_node.h"
#include <godot_cpp/variant/utility_functions.hpp>



void QTexturedPBDBodyNode::_notification(int what)
{
    switch (what){
    case NOTIFICATION_POST_ENTER_TREE:
        if(Engine::get_singleton()->is_editor_hint()==false ){
            create_mesh_with_texture();
            set_physics_process(true);
        }else{
            InitDefaultParticleTexture();
        }
        
        break;
    case NOTIFICATION_PHYSICS_PROCESS:
        if(Engine::get_singleton()->is_editor_hint()==false ){
            on_physics_process();
        }
        break;
    case NOTIFICATION_PROCESS :
        if(Engine::get_singleton()->is_editor_hint()==false ){
            queue_redraw();
        }
        break;
    case NOTIFICATION_DRAW :
        render();
        break;
    default:
        break;
    }
}

Ref<ArrayMesh> QTexturedPBDBodyNode::CreateCircleMesh(float radius, int segments)
{

    Ref<ArrayMesh> mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
    

    // Verileri tutacak array
    Array mesh_arrays;
    mesh_arrays.resize(Mesh::ARRAY_MAX);

    // Tepe noktalarını hesapla (vertices)
    PackedVector3Array vertices;
    vertices.resize(segments + 1); // Merkez + çevre noktaları
    vertices[0] = Vector3(0, 0, 0); // Merkez noktası (orijin)

    // Çevre noktalarını hesapla
    for (int i = 0; i < segments; ++i) {
        float angle = Math_TAU * i / segments; // Açı hesaplama
        float x = radius * Math::cos(angle);
        float y = radius * Math::sin(angle);
        vertices[i + 1] = Vector3(x, y, 0); // Çevre noktası
    }

    // Indices (üçgenler için)
    PackedInt32Array indices;
    indices.resize(segments * 3);
    for (int i = 0; i < segments; ++i) {
        indices[i * 3 + 0] = 0;               // Merkez
        indices[i * 3 + 1] = i + 1;          // Çevredeki nokta
        indices[i * 3 + 2] = (i + 1) % segments + 1; // Sonraki nokta
    }

    // Mesh arraylerini doldur
    mesh_arrays[Mesh::ARRAY_VERTEX] = vertices;
    mesh_arrays[Mesh::ARRAY_INDEX] = indices;

    // Mesh'i oluştur
    mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, mesh_arrays);

    return mesh;
}

Ref<ArrayMesh> QTexturedPBDBodyNode::CreateQuadMesh(Vector2 size)
{
    Ref<ArrayMesh> mesh = Ref<ArrayMesh>(memnew(ArrayMesh));
    Array arrays;
    arrays.resize(Mesh::ARRAY_MAX);
    PackedVector3Array vertices;
    vertices.push_back(Vector3(-size.x, -size.y, 0));
    vertices.push_back(Vector3(size.x, -size.y, 0));
    vertices.push_back(Vector3(size.x, size.y, 0));
    vertices.push_back(Vector3(-size.x, size.y, 0));
    arrays[Mesh::ARRAY_VERTEX] = vertices;

    PackedVector2Array uvs;
    uvs.push_back(Vector2(0, 1)); 
    uvs.push_back(Vector2(1, 1));
    uvs.push_back(Vector2(1, 0));
    uvs.push_back(Vector2(0, 0));
    arrays[Mesh::ARRAY_TEX_UV] = uvs;

    PackedInt32Array indices;
    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(0);
    arrays[Mesh::ARRAY_INDEX] = indices;

    mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);

    return mesh;
}

void QTexturedPBDBodyNode::render()
{
    if(Engine::get_singleton()->is_editor_hint()){
        //In Editor
        if (sourceTexture!=nullptr){
            Vector2 scaledSize=sourceTexture->get_size()*textureScale;
            draw_texture_rect(sourceTexture,Rect2(-scaledSize*0.5,scaledSize),false);
            return;
        }
        
    }
}

void QTexturedPBDBodyNode::on_physics_process()
{
    if(multiMeshInstance==nullptr){
        return;
    }

    //breaking
    if(breakable){
        for (size_t i=0;i<meshNode->get_spring_count();++i){
            Ref<QSpringObject > s=meshNode->get_spring_at(i);
            Ref<QParticleObject> pA=s->get_particle_a();
            Ref<QParticleObject> pB=s->get_particle_b();

            float len=(pB->get_global_position()-pA->get_global_position() ).length();
            float t_len=s->get_length();
            float diff=abs(t_len-len);
            if(diff>breakDistance){
                meshNode->remove_spring_at(i);
            }

        } 
    }

    for (int i = 0; i < meshNode->get_particle_count(); ++i) {
        Ref<QParticleObject> particle = meshNode->get_particle_at(i);
        Vector2 pos = particle->get_global_position()-multiMeshInstance->get_global_position();
        Color color = particleColors[i];

        // Her partikülün konumunu ve rengini custom data'ya aktaralım
        multiMesh->set_instance_transform_2d(i, Transform2D(0, pos)); // Konum aktarımı

    }

    if(enableSDFRendering){
        float textureGAP=(pixelStepSize*SFDTextureScale)+32.0f;
        Rect2 viewportRect=get_aabb().grow( textureGAP );
        if(SDFViewport!=nullptr ){
            SDFViewport->set_size(viewportRect.size );
        }

        if(SDFCamera!=nullptr ){
            SDFCamera->set_global_position(viewportRect.position );
        }

        if(SDFSprite!=nullptr ){
            SDFSprite->set_global_position(viewportRect.position );
        }

    }

    
}

void QTexturedPBDBodyNode::InitDefaultParticleTexture()
{
    if(SFDTexture==nullptr){
        Ref<Gradient> gradient = Ref<Gradient>(memnew(Gradient));

        Color inColor=Color::named("white");
        inColor.a=0.0;
        Color outColor=Color::named("white");
        outColor.a=1.0f;

        gradient->set_color(0,inColor);
        gradient->set_color(1,outColor);

        gradient->reverse();
        
        
        

        gradient->set_interpolation_mode(Gradient::InterpolationMode::GRADIENT_INTERPOLATE_CUBIC);

        Ref<GradientTexture2D> gradient_texture = Ref<GradientTexture2D>(memnew(GradientTexture2D) );
        gradient_texture->set_width(256.0f);
        gradient_texture->set_height(256.0f);
        gradient_texture->set_fill_from(Vector2(0.5f,0.5f));
        gradient_texture->set_fill_to(Vector2(1.0f,1.0f));
        gradient_texture->set_fill(GradientTexture2D::Fill::FILL_RADIAL);
        gradient_texture->set_repeat(GradientTexture2D::REPEAT_NONE);
        
        gradient_texture->set_gradient(gradient);

        SFDTexture=gradient_texture;

    }


}

void QTexturedPBDBodyNode::_bind_methods()
{
    //Texture
     ClassDB::bind_method(D_METHOD("get_texture"),&QTexturedPBDBodyNode::get_texture );
     ClassDB::bind_method(D_METHOD("set_texture","value"),&QTexturedPBDBodyNode::set_texture );
     
     ADD_PROPERTY( PropertyInfo(Variant::OBJECT, "texture",PROPERTY_HINT_RESOURCE_TYPE,"Texture2D"),"set_texture","get_texture" );

     //Texture Scale
     ClassDB::bind_method(D_METHOD("get_texture_scale"),&QTexturedPBDBodyNode::get_texture_scale );
     ClassDB::bind_method(D_METHOD("set_texture_scale","value"),&QTexturedPBDBodyNode::set_texture_scale );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "texture_scale"),"set_texture_scale","get_texture_scale" );

     //SDF Rendering
     ClassDB::bind_method(D_METHOD("get_sdf_rendering_enabled"),&QTexturedPBDBodyNode::get_sdf_rendering_enabled );
     ClassDB::bind_method(D_METHOD("set_sdf_rendering_enabled","value"),&QTexturedPBDBodyNode::set_sdf_rendering_enabled );

     ADD_PROPERTY( PropertyInfo(Variant::BOOL, "enable_sdf_rendering"),"set_sdf_rendering_enabled","get_sdf_rendering_enabled" );

     //SFD Texture
     ClassDB::bind_method(D_METHOD("get_sfd_texture"),&QTexturedPBDBodyNode::get_sfd_texture );
     ClassDB::bind_method(D_METHOD("set_sfd_texture","value"),&QTexturedPBDBodyNode::set_sfd_texture );
     
     ADD_PROPERTY( PropertyInfo(Variant::OBJECT, "sfd_texture",PROPERTY_HINT_RESOURCE_TYPE,"Texture2D"),"set_sfd_texture","get_sfd_texture" );

     ClassDB::bind_method(D_METHOD("get_sfd_texture_scale"),&QTexturedPBDBodyNode::get_sfd_texture_scale );
     ClassDB::bind_method(D_METHOD("set_sfd_texture_scale","value"),&QTexturedPBDBodyNode::set_sfd_texture_scale );
     
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "sfd_texture_scale"),"set_sfd_texture_scale","get_sfd_texture_scale" );

     ClassDB::bind_method(D_METHOD("get_sfd_shader_alpha_threshold"),&QTexturedPBDBodyNode::get_sfd_shader_alpha_threshold );
     ClassDB::bind_method(D_METHOD("set_sfd_shader_alpha_threshold","value"),&QTexturedPBDBodyNode::set_sfd_shader_alpha_threshold );
     
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "sfd_shader_alpha_threshold"),"set_sfd_shader_alpha_threshold","get_sfd_shader_alpha_threshold" );

     //Breakable

     ClassDB::bind_method(D_METHOD("get_breakable"),&QTexturedPBDBodyNode::get_breakable );
     ClassDB::bind_method(D_METHOD("set_breakable","value"),&QTexturedPBDBodyNode::set_breakable );

     ADD_PROPERTY( PropertyInfo(Variant::BOOL, "breakable"),"set_breakable","get_breakable" );

     //Break Distance

     ClassDB::bind_method(D_METHOD("get_break_distance"),&QTexturedPBDBodyNode::get_break_distance );
     ClassDB::bind_method(D_METHOD("set_break_distance","value"),&QTexturedPBDBodyNode::set_break_distance );

     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "break_distance"),"set_break_distance","get_break_distance" );

     //Pixel Step Size

     ClassDB::bind_method(D_METHOD("get_pixel_step_size"),&QTexturedPBDBodyNode::get_pixel_step_size );
     ClassDB::bind_method(D_METHOD("set_pixel_step_size","value"),&QTexturedPBDBodyNode::set_pixel_step_size );
     ADD_PROPERTY( PropertyInfo(Variant::FLOAT, "pixel_step_size"),"set_pixel_step_size","get_pixel_step_size" );

     

     


}


QTexturedPBDBodyNode *QTexturedPBDBodyNode::create_mesh_with_texture()
{
    if(meshNode==nullptr){
        meshNode=memnew(QMeshNode);
    }

    Ref<Image> image=sourceTexture->get_image();

    //Clear Mesh Node
    while(meshNode->get_particle_count()>0){
        meshNode->remove_particle_at(0);
    }

    while(meshNode->get_spring_count()>0){
        meshNode->remove_spring_at(0);
    }

    particlePositions.clear();
    particleColors.clear();

    
    float halfPixelStepSize=pixelStepSize*0.5f;
    float particleRadius=halfPixelStepSize*textureScale;
    size_t horizontalStepCount=round(image->get_size().x/pixelStepSize);
    size_t verticalStepCount=round(image->get_size().y/pixelStepSize);

    float hStepSize=pixelStepSize;
    float vStepSize=pixelStepSize;

    Vector2 halfSize=sourceTexture->get_size()*0.5f;
    

    for(size_t v=0;v<verticalStepCount;++v){
        for(size_t h=0;h<horizontalStepCount;++h){
            Vector2 pixelPos=Vector2(hStepSize*h,vStepSize*v);
            Color pixel=image->get_pixel(pixelPos.x,pixelPos.y);
            
            Ref<QParticleObject> particle=Ref<QParticleObject>( memnew(QParticleObject) );

            Vector2 particlePos=(pixelPos-halfSize)*textureScale;

            particle->configure(particlePos,particleRadius);

            

            meshNode->add_particle(particle);
            particlePositions.push_back(pixelPos);
            particleColors.push_back(pixel);
        }
    }

    unordered_set<pair<int,int>, SpringIndexPairHash> springPairs;
    for(size_t v=0;v<verticalStepCount-1;++v){
        for(size_t h=0;h<horizontalStepCount-1;++h){
            /* A*****B
               ***x***
               D*****C
            */

            int iA=v*horizontalStepCount+h;
            int iB=v*horizontalStepCount+h+1;
            int iC=(v+1)*horizontalStepCount+h+1;
            int iD=(v+1)*horizontalStepCount+h;

            //Adding Spring Pairs
            pair<int,int> pair1(iA,iB);
            springPairs.emplace(pair1);

            pair<int,int> pair2(iB,iC);
            springPairs.emplace(pair2);

            pair<int,int> pair3(iC,iD);
            springPairs.emplace(pair3);

            pair<int,int> pair4(iD,iA);
            springPairs.emplace(pair4);

            pair<int,int> pair5(iA,iC);
            springPairs.emplace(pair5);

            pair<int,int> pair6(iB,iD);
            springPairs.emplace(pair6);


            /* Ref<QParticleObject> pA=meshNode->get_particle_at(iA);
            Ref<QParticleObject> pB=meshNode->get_particle_at(iB);
            Ref<QParticleObject> pC=meshNode->get_particle_at(iC);
            Ref<QParticleObject> pD=meshNode->get_particle_at(iD);

            vector<Ref<QParticleObject>> particleRectangle={pA,pB,pC,pD};


            for(size_t i=0;i<particleRectangle.size();++i ){
                Ref<QParticleObject> pp=particleRectangle[(i-1+particleRectangle.size())%particleRectangle.size() ];
                Ref<QParticleObject> p=particleRectangle[i];
                Ref<QParticleObject> np=particleRectangle[ (i+1)%particleRectangle.size() ];

                Ref<QAngleConstraintObject> constraint=Ref<QAngleConstraintObject>( memnew(QAngleConstraintObject) );
                constraint->configure_with_angle_of_local_positions(pp,p,np);
                constraint->set_rigidity(0.001f);
                meshNode->add_angle_constraint(constraint);
                

            } */

            



        }
    }
    
    //PackedInt32Array tris=Geometry2D::get_singleton()->triangulate_delaunay(particlePositions);
    

    // Add Springs 
    for (auto pair: springPairs){
        Ref<QSpringObject> spring=Ref<QSpringObject>( memnew(QSpringObject) );
        Ref<QParticleObject> pA=meshNode->get_particle_at(pair.first);
        Ref<QParticleObject> pB=meshNode->get_particle_at(pair.second);
        spring->configure_with_current_distance(pA,pB,false);
        meshNode->add_spring(spring);
    }

    //Removing Zero Alpha Particles
    size_t i=0;
    while (i<meshNode->get_particle_count() ){
        if(particleColors[i].a==0.0f){
            meshNode->remove_particle_at(i);
            particleColors.remove_at(i);
        }else{
            ++i;
        }
    }


    add_child(meshNode);

    //Inıtıalize Particle Texture

   
    
    
    
    

    String sfd_shader_code = R"(
        shader_type canvas_item;

        render_mode blend_mix;

        uniform float radius; 
        varying vec4 p_color; 

        void vertex() {
            p_color = INSTANCE_CUSTOM;
        }

        void fragment() {
            
            vec4 tex_color = texture(TEXTURE, UV);

            
            vec4 custom_color = p_color;
            custom_color.a = tex_color.a; 

            
            COLOR = custom_color;
        }

    )";

    
    Ref<Shader> shader = Ref<Shader>(memnew(Shader) );
    shader->set_code(sfd_shader_code);

    Ref<ShaderMaterial>  shader_material =Ref<ShaderMaterial>( memnew(ShaderMaterial) );
    shader_material->set_shader(shader);

    

    shader_material->set_shader_parameter("radius",particleRadius);
    
    

    multiMesh = Ref<MultiMesh>(memnew(MultiMesh) );
    multiMesh->set_use_custom_data(true);
    multiMesh->set_instance_count(meshNode->get_particle_count());



    // Ref<ArrayMesh> mesh = CreateCircleMesh(particleRadius,36); //Circle Mesh
    float textureScale=SFDTexture==nullptr ? particleRadius:particleRadius*SFDTextureScale;
    Ref<ArrayMesh> mesh = CreateQuadMesh(Vector2(textureScale,textureScale) ); // Quad Mesh
    
    multiMesh->set_mesh(mesh);

    multiMeshInstance = memnew(MultiMeshInstance2D);
    multiMeshInstance->set_multimesh(multiMesh);

    for (int i = 0; i < meshNode->get_particle_count(); ++i) {
        Ref<QParticleObject> particle = meshNode->get_particle_at(i);
        Vector2 pos=particle->get_global_position()-multiMeshInstance->get_global_position();
        Color color=particleColors[i]; 
        
        // Her partikülün konumunu ve rengini shader parametrelerine aktaralım
        multiMesh->set_instance_transform_2d(i, Transform2D(0, pos));
        multiMesh->set_instance_custom_data(i, color );
        
    }

    multiMeshInstance->set_material(shader_material);

    if(enableSDFRendering==false){
        add_child(multiMeshInstance);
        return this;
    }
    

    if(SFDTexture!=nullptr){
        multiMeshInstance->set_texture(SFDTexture);
    }
    

    


    //Create viewport to apply shader
    float textureGAP=(pixelStepSize*SFDTextureScale)+32.0f;
    Rect2 viewportRect=get_aabb().grow( textureGAP );
    SDFViewport=memnew(SubViewport);
    SDFViewport->set_size( viewportRect.size );
    SDFViewport->set_update_mode(SubViewport::UpdateMode::UPDATE_WHEN_VISIBLE);
    SDFViewport->set_clear_mode(SubViewport::CLEAR_MODE_ALWAYS);
    SDFViewport->set_disable_input(true);
    SDFViewport->set_transparent_background(true);
    SDFViewport->set_disable_3d(true);

    SDFCamera=memnew(Camera2D);
    SDFViewport->add_child(SDFCamera);
    SDFCamera->set_anchor_mode(Camera2D::AnchorMode::ANCHOR_MODE_FIXED_TOP_LEFT);
    SDFCamera->set_global_position(viewportRect.position );
    


    add_child(SDFViewport);

    SDFViewport->add_child(multiMeshInstance);

    SDFSprite=memnew(Sprite2D);
    add_child(SDFSprite);
    SDFSprite->set_texture(SDFViewport->get_texture());
    SDFSprite->set_centered(false);
    SDFSprite->set_global_position(viewportRect.position );

    



    String sfd_sprite_shader_code = R"(
        shader_type canvas_item;

        render_mode blend_mix;

        uniform float alpha_threshold=0.5;

        void fragment() {
            
            vec4 tex_color = texture(TEXTURE, UV);

            if(tex_color.a<alpha_threshold){
                discard;
            }

            if (tex_color.a > 0.0) {
                tex_color.rgb /= tex_color.a;
                tex_color.a=1.0f;
            }

        
            COLOR = tex_color;
        }

    )";

     Ref<Shader> sfd_sprite_shader = Ref<Shader>(memnew(Shader) );
     sfd_sprite_shader->set_code(sfd_sprite_shader_code);
    

    Ref<ShaderMaterial>  sfd_sprite_shader_material =Ref<ShaderMaterial>( memnew(ShaderMaterial) );
    sfd_sprite_shader_material->set_shader(sfd_sprite_shader);

    sfd_sprite_shader_material->set_shader_parameter("alpha_threshold",SFDShaderAlphaThreshold);

    

    SDFSprite->set_material(sfd_sprite_shader_material);
    

    

    



    return this;
}
