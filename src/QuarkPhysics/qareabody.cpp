
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
 *
**************************************************************************************/
#include "qareabody.h"
#include "qbody.h"
#include "qcollision.h"
#include "qworld.h"

QAreaBody::QAreaBody(){
	simulationModel=QBody::SimulationModels::RIGID_BODY;
	bodyType=QBody::BodyTypes::AREA;
}
void QAreaBody::AddCollidedBody(QBody* body){
	bool isBodyNew=bodies.insert(body).second;
	if(isBodyNew){
		OnCollisionEnter(body);
		if(CollisionEnterEventListener!=nullptr)
			CollisionEnterEventListener(this,body);
	}

}
void QAreaBody::CheckBodies(){
	vector<QBody*> blackList;
	for(auto body:bodies){
		bool bodyIsOnBlackList=false;
		if(body->GetEnabled()==false){
			blackList.push_back(body);
			bodyIsOnBlackList=true;
		}
		vector<QCollision::Contact*> collisionContacts=world->GetCollisions(this,body);
		if(body->GetEnabled()==false || collisionContacts.size()==0 ){
			blackList.push_back(body);
			bodyIsOnBlackList=true;
		}
		
		QVector linear_force=ComputeLinearForce(body);
		if (gravityFree==true || linear_force!=QVector::Zero() ){

			

			if(body->GetBodyType()==QBody::BodyTypes::RIGID){
				if(bodyIsOnBlackList){
					if (gravityFree==true){
						body->ignoreGravity=false;
					}
				}else{
					if (gravityFree==true && body->ignoreGravity==false){
						body->ignoreGravity=true;
					}
					if(linear_force!=QVector::Zero() ){
						body->ApplyForce(linear_force);
					}
				}
				

			}else if (body->GetBodyType()==QBody::SOFT ){
				if(bodyIsOnBlackList){
					for (size_t i=0;i<body->GetMeshCount();++i){
						QMesh *mesh=body->GetMeshAt(i);
						vector<bool> particleCollisionChecklist;
						for(size_t j=0;j<mesh->GetParticleCount();++j ){

							QParticle *particle=mesh->GetParticleAt(j);

							if(gravityFree){
								particle->ignoreGravity=false;
							}

						}
					}
				}else{

					for (size_t i=0;i<body->GetMeshCount();++i){
						QMesh *mesh=body->GetMeshAt(i);
						vector<bool> particleCollisionChecklist;
						for(size_t j=0;j<mesh->GetParticleCount();++j ){
							QParticle *particle=mesh->GetParticleAt(j);
							//Checking are particles colliding
							bool particleIsColliding=false;
							for(size_t k=0;k<collisionContacts.size();++k ){
								QCollision::Contact *contact=collisionContacts[k];

								if(contact->particle==particle){
									particleIsColliding=true;
									break;
								}
								
								for(auto rp:contact->referenceParticles){
									if(rp==particle){
										particleIsColliding=true;
										break;
									}
								}
								if(particleIsColliding==true){
									break;
								}
							}
							particleCollisionChecklist.push_back(particleIsColliding);


						}

						for(size_t m=0;m<particleCollisionChecklist.size();++m){
							QParticle *particle=mesh->GetParticleAt(m);
							if(particle->GetEnabled()==false || particle->GetIsLazy()==true  )
								continue;
							if(gravityFree){
								particle->ignoreGravity=particleCollisionChecklist[m];
							}

							if (linear_force!=QVector::Zero() && body->GetMode()!=QBody::STATIC ){
								particle->ApplyForce(linear_force);
							}
						}


					}
				}


				

				
			}
		}

		
	}
	while (blackList.size()!=0) {
		OnCollisionExit(blackList[0]);
		if(CollisionExitEventListener!=nullptr)
			CollisionExitEventListener(this,blackList[0]);
		if (gravityFree==true && blackList[0]->ignoreGravity==true ){
			blackList[0]->ignoreGravity=false;
		}
		bodies.erase(blackList[0]);
		blackList.erase(blackList.begin());
	}
}
