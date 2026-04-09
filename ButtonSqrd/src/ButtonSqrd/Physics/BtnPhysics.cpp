#include "BtnPhysics.h"
#include"ButtonSqrd/Core/GameObject.h"

namespace BtnSqd {

	std::unordered_map<physx::PxActor*, std::shared_ptr<GameObject>> BtnPhysics::physicsGameObjects;

	BtnPhysics::~BtnPhysics() {
		delete sceneDesc;
	}
	bool BtnPhysics::Init() {
		foundation = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocator, errorCallback);
		if (!foundation) {
			BTN_CORE_LOG_ERROR("Failed to initialize physics engine");
		}
		bool recordMemoryAllocations = true;

		toleranceScale.length = 1.0f;
		toleranceScale.speed = 10.0f;

		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, toleranceScale, recordMemoryAllocations);
		if (!physics) {
			BTN_CORE_LOG_ERROR("Failed to create physics engine");
		}
		sceneDesc = new physx::PxSceneDesc(physics->getTolerancesScale());
		sceneDesc->gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		defaultCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc->cpuDispatcher = defaultCpuDispatcher;
		sceneDesc->filterShader = physx::PxDefaultSimulationFilterShader;
		sceneDesc->simulationEventCallback = new ColliderTriggerCallback();
		pScene = physics->createScene(*sceneDesc);
		return true;
	}

	bool CheckPos(physx::PxTransform pose) {
		return pose.p.isFinite() && pose.q.isFinite() && physx::PxAbs(pose.q.magnitude() - 1.0f) < 1e-4f;
	}

	void BtnPhysics::Update() {
		for (auto& [id,transform, physicsComp] : currentScene->GetRegister().GetAllOfID<TransformComponent, PhysicsComponet>()) {
			glm::vec3 btnPos = transform.transform.GetPosition();
			glm::quat btnRot = transform.transform.GetRotationQ();
			physx::PxVec3 position = glmToPxVec3(btnPos);
			physx::PxQuat rotation = glmToPxQuat(btnRot);

			physx::PxTransform pose=physx::PxTransform(position,rotation);

			if (!physicsComp.rigidBody) {

				currentScene->GetgameObjects()[id]->GetComponent<PhysicsComponet>().resetComponent = false;
				AddPhysicsObject(*currentScene->GetgameObjects()[id]);
				continue;
			}
			if (physicsComp.resetComponent) {
				currentScene->GetgameObjects()[id]->GetComponent<PhysicsComponet>().resetComponent = false;
				AddPhysicsObject(*currentScene->GetgameObjects()[id]);
			}
			physx::PxRigidActor* actor = physicsComp.rigidBody;
			if(CheckPos(pose))
				actor->setGlobalPose(pose);
		}

		pScene->simulate(BtnTime::GetDeltaTime());
		pScene->fetchResults(true);
		for (auto& [transformRef,physicsObjsRef]:currentScene->GetRegister().GetAllOfRef<TransformComponent,PhysicsComponet>()) {
			TransformComponent& transform= transformRef.get();
			PhysicsComponet& physicsObjs = physicsObjsRef.get();
			
			physx::PxRigidActor* actor = physicsObjs.rigidBody;
			physx::PxTransform pTransform = actor->getGlobalPose();
			physx::PxVec3 position = pTransform.p;
			physx::PxQuat rotation = pTransform.q;
			transform.transform.MoveTo(pxToGlmVec3(position));
			transform.transform.RotateTo(pxToGlmQuat(rotation));
		}
	}

	bool BtnPhysics::ShutDown() {
		physics->release();
		foundation->release();
		return false;
	}

	void BtnPhysics::OnSceneLoad() {
		for (auto [id, collider] : currentScene->GetRegister().GetAllOfID<ColliderComponent>()) {
			GameObject gameObject = { (entt::entity)id,currentScene.get() };
			CreateCollider(gameObject, gameObject.GetComponent<ColliderComponent>());
		}

		for (auto[id,transform,physics]:currentScene->GetRegister().GetAllOfID<TransformComponent,PhysicsComponet>()) {
			GameObject gameObject = { (entt::entity)id,currentScene.get() };
			AddPhysicsObject(gameObject);
		}
	}
	void BtnPhysics::AddPhysicsObject(GameObject& gameObject)	{
		PhysicsComponet &physComp = gameObject.GetComponent<PhysicsComponet>();
		TransformComponent& transformComp = gameObject.GetComponent<TransformComponent>();
		ColliderComponent& collider = gameObject.GetComponent<ColliderComponent>();
		
		if (physComp.rigidBody) {
			if(physComp.rigidBody->getScene() == pScene)
				pScene->removeActor(*physComp.rigidBody);
		}
		
		if (!collider.pColliderShape) {
			CreateCollider(gameObject, collider);
		}

		physx::PxTransform pose = physx::PxTransform(glmToPxVec3(transformComp.transform.GetPosition()),glmToPxQuat(transformComp.transform.GetRotationQ()) );
		physx::PxShape* shape = collider.pColliderShape;

		if (physComp.isDynamic) {
			physComp.rigidBody = physics->createRigidDynamic(pose);
			physx::PxRigidDynamic* dynamic = static_cast<physx::PxRigidDynamic*>(physComp.rigidBody);
			float halfExtent = 0.5f;

			physx::PxRigidDynamicLockFlags lockFlags;

			if (physComp.lockX) lockFlags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X;
			if (physComp.lockY) lockFlags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y;
			if (physComp.lockZ) lockFlags |= physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z;

			dynamic->setRigidDynamicLockFlags(lockFlags);


			if (physComp.isKinematic) {
				dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);
			}
			else {
				dynamic->setAngularDamping(0.5f);
				dynamic->setLinearVelocity(physx::PxVec3(0));
				physx::PxRigidBodyExt::updateMassAndInertia(*static_cast<physx::PxRigidBody*>(physComp.rigidBody), 10.0f);
			}
			physComp.rigidBody->attachShape(*shape);
		}
		else {
			physComp.rigidBody = physics->createRigidStatic(pose);
			physComp.rigidBody->attachShape(*shape);
		}
		if (pScene->addActor(*physComp.rigidBody)) {
			physicsGameObjects.insert({ physComp.rigidBody,currentScene->GetgameObjects()[gameObject.GetId()] });
		}
	}
	void BtnPhysics::CreateCollider(GameObject& gameObject,ColliderComponent& collider) {
		if (collider.dActor) {
			collider.dActor->release();
		}
		if (collider.physicsMat) {
			collider.physicsMat->release();
		}
		if (collider.pColliderShape) {
			collider.pColliderShape->release();
		}
		collider.physicsMat = physics->createMaterial(collider.staticFriction,collider.dynamicFriction,collider.resitution);
		physx::PxVec3 halfScale = physx::PxVec3(collider.scale.x, collider.scale.y, collider.scale.z);
		switch (collider.colliderType) {
		case ColliderType::BoxCollider: {
			collider.pColliderShape = physics->createShape(physx::PxBoxGeometry(halfScale), *collider.physicsMat);
			break;
		}
		case ColliderType::CapsuleCollider: {				
				physx::PxCapsuleGeometry capsule(collider.radius, collider.halfHeight);
				physx::PxQuat rotation = physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.0f, 0.0f, 1.0f));
				physx::PxTransform pose = physx::PxTransform(physx::PxVec3(0.0f), rotation);

				collider.pColliderShape = physics->createShape(physx::PxCapsuleGeometry(collider.radius,collider.halfHeight), *collider.physicsMat);
				collider.pColliderShape->setLocalPose(pose);
				break;
			}
			case ColliderType::CircleCollider:
				collider.pColliderShape = physics->createShape(physx::PxSphereGeometry(collider.radius), *collider.physicsMat);
				break;
			case ColliderType::PlaneCollider: {
				physx::PxVec3 halfExtents(collider.scale.x, 0.01f, collider.scale.z);
				physx::PxBoxGeometry box(halfExtents);
				collider.pColliderShape = physics->createShape(box, *collider.physicsMat);
				break;
			}
			case ColliderType::CustomCollider: {
				
				collider.pColliderShape = CreateCustomCollider(collider,gameObject);
				break;
			}
		}
		if (collider.isTrigger || !gameObject.CheckObjectForComponent<PhysicsComponet>()) {
			if (collider.pColliderShape) {
				collider.pColliderShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
				collider.pColliderShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

				if (!gameObject.CheckObjectForComponent<PhysicsComponet>()) {
					collider.dActor = physics->createRigidStatic(physx::PxTransform(glmToPxVec3(gameObject.GetComponent<TransformComponent>().transform.GetPosition())));
					collider.dActor->attachShape(*collider.pColliderShape);
					if (pScene->addActor(*collider.dActor)) {
						physicsGameObjects.insert({ collider.dActor,currentScene->GetgameObjects()[gameObject.GetId()] });
					}
				}
			}
			else {
				collider.dActor = nullptr;
				collider.physicsMat = nullptr;
			}
		}
	}
	void BtnPhysics::Reset() {
		physx::PxU32 actorCount = pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
		std::vector<physx::PxActor*> actors(actorCount);
		pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, actors.data(), actorCount);

		for (physx::PxActor* actor : actors) {
			pScene->removeActor(*actor);
		}

		pScene->release();
		physicsGameObjects.clear();
	}
	void BtnPhysics::ReInit() {
		for (auto& [physicsComp] : currentScene->GetRegister().GetAllOf<PhysicsComponet>()) {
			physicsComp.rigidBody = nullptr;
		}
		pScene = physics->createScene(*sceneDesc);
		for (auto [id,collider] : currentScene->GetRegister().GetAllOfID<ColliderComponent>()) {
			GameObject current = *currentScene->GetgameObjects()[id];
			CreateCollider(current, collider);

			if (current.CheckObjectForComponent<PhysicsComponet>()) {
				AddPhysicsObject(current);
			}
		}
	}
	void BtnPhysics::DisplayColliders()	{

	}
	bool BtnPhysics::RemoveObject(GameObject& gameObject) {
		PhysicsComponet& physicsComp = gameObject.GetComponent<PhysicsComponet>();
		pScene->removeActor(*physicsComp.rigidBody);
		return true;
	}
	physx::PxShape* BtnPhysics::CreateCustomCollider(ColliderComponent& collider,GameObject gameObject) {
		if (!gameObject.CheckObjectForComponent<PhysicsComponet>()) {
			BTNLOG_ERROR("Custom Colliders Need A Physics Component")
			return nullptr;
		}
		if (gameObject.GetComponent<PhysicsComponet>().isKinematic) {
			if (gameObject.CheckObjectForComponent<ModelComponent>()) {
				Mesh mesh = gameObject.GetComponent<ModelComponent>().currentModel.GetMeshes()->at(0);
				std::vector<physx::PxU32> indices = mesh.GetIndices();
				std::vector<physx::PxVec3> vertices;
				std::vector<Vertices> verts = mesh.GetVerticies();
				for (auto& vert:verts) {
					vertices.push_back(glmToPxVec3(vert.position));
				}

				physx::PxTriangleMeshDesc nMesh;
				nMesh.points.count = static_cast<physx::PxU32>(vertices.size());
				nMesh.points.stride = sizeof(physx::PxVec3);
				nMesh.points.data = vertices.data();

				nMesh.triangles.count = static_cast<physx::PxU32>(indices.size()/3);
				nMesh.triangles.stride = 3*sizeof(physx::PxU32);
				nMesh.triangles.data = indices.data();
				
				physx::PxCookingParams cookingParams(physics->getTolerancesScale());
				physx::PxDefaultMemoryOutputStream outputStream;

				if (!PxCookTriangleMesh(cookingParams,nMesh,outputStream)) {
					BTNLOG_ERROR("Failed to create custom collider mesh");
					return nullptr;
				}
				physx::PxDefaultMemoryInputData inputStream(outputStream.getData(), outputStream.getSize());
				physx::PxTriangleMesh* triangleMesh = physics->createTriangleMesh(inputStream);

				TransformComponent transform = gameObject.GetComponent<TransformComponent>();
				physx::PxMeshScale mScale(glmToPxVec3(transform.transform.scale), glmToPxQuat(transform.transform.GetRotationQ()));

				return physics->createShape(physx::PxTriangleMeshGeometry(triangleMesh,mScale),*collider.physicsMat);
			}
		}
		return physics->createShape(physx::PxPlaneGeometry(),*collider.physicsMat);
	}
	void BtnPhysics::ColliderTriggerCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
		for (physx::PxU32 i = 0; i < count; ++i) {
			const physx::PxTriggerPair& pair = pairs[i];
			GameObject triggerCollider = *BtnPhysics::physicsGameObjects[pair.triggerActor];
			GameObject otherCollider = *BtnPhysics::physicsGameObjects[pair.otherActor];

			if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
				if (triggerCollider.CheckObjectForComponent<ScriptComponent>()) {
					ScriptComponent& scriptComp = triggerCollider.GetComponent<ScriptComponent>();
					for (auto& script:scriptComp.scripts) {
						script.script->OnColliderEnter(BtnPhysics::physicsGameObjects[pair.otherActor].get());
					}
				}
			}
			else if (pair.status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {
				if (triggerCollider.CheckObjectForComponent<ScriptComponent>()) {
					ScriptComponent& scriptComp = triggerCollider.GetComponent<ScriptComponent>();
					for (auto& script : scriptComp.scripts) {
						script.script->OnColliderLeave(BtnPhysics::physicsGameObjects[pair.otherActor].get());
					}
				}
			}
		}
	}
	void BtnPhysics::ColliderTriggerCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 count) {
		GameObject thisObj = *BtnPhysics::physicsGameObjects[pairHeader.actors[0]];
		GameObject otherObj = *BtnPhysics::physicsGameObjects[pairHeader.actors[1]];
		
		for (physx::PxU32 i = 0; i < count; ++i) {
			const physx::PxContactPair& cp = pairs[i];
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) {
				BTNLOG_TRACE("There was a collision");
			}
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS) {

			}
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST) {

			}
		}

	}
}
