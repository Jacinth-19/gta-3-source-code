#include "physics/JoltPhysicsWorld.hpp"

#include <Jolt/Core/Core.h>
JPH_SUPPRESS_WARNINGS

#include <Jolt/Jolt.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyLockInterface.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>

#include <algorithm>
#include <thread>

namespace waypoint::physics {
namespace layers {
constexpr JPH::ObjectLayer NON_MOVING = 0;
constexpr JPH::ObjectLayer MOVING = 1;
constexpr JPH::ObjectLayer NUM_LAYERS = 2;
} // namespace layers

namespace broad_phase_layers {
constexpr JPH::BroadPhaseLayer NON_MOVING(0);
constexpr JPH::BroadPhaseLayer MOVING(1);
constexpr uint NUM_LAYERS = 2;
} // namespace broad_phase_layers

class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface {
public:
    uint GetNumBroadPhaseLayers() const override { return broad_phase_layers::NUM_LAYERS; }
    JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer layer) const override {
        return layer == layers::NON_MOVING ? broad_phase_layers::NON_MOVING : broad_phase_layers::MOVING;
    }
#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
    const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer layer) const override {
        return layer == broad_phase_layers::NON_MOVING ? "NON_MOVING" : "MOVING";
    }
#endif
};

class ObjectVsBroadPhaseLayerFilterImpl final : public JPH::ObjectVsBroadPhaseLayerFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer layer1, JPH::BroadPhaseLayer layer2) const override {
        if (layer1 == layers::NON_MOVING) {
            return layer2 == broad_phase_layers::MOVING;
        }
        return true;
    }
};

class ObjectLayerPairFilterImpl final : public JPH::ObjectLayerPairFilter {
public:
    bool ShouldCollide(JPH::ObjectLayer layer1, JPH::ObjectLayer layer2) const override {
        if (layer1 == layers::NON_MOVING) {
            return layer2 == layers::MOVING;
        }
        return true;
    }
};

struct JoltPhysicsWorld::Impl {
    BPLayerInterfaceImpl broadPhaseLayerInterface;
    ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseLayerFilter;
    ObjectLayerPairFilterImpl objectLayerPairFilter;
    std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
    std::unique_ptr<JPH::JobSystemThreadPool> jobSystem;
    std::unique_ptr<JPH::PhysicsSystem> physicsSystem;
    bool initialized = false;
};

JoltPhysicsWorld::JoltPhysicsWorld() : impl_(std::make_unique<Impl>()) {}
JoltPhysicsWorld::~JoltPhysicsWorld() { Shutdown(); }

bool JoltPhysicsWorld::Init() {
    if (impl_->initialized) {
        return true;
    }
    JPH::RegisterDefaultAllocator();
    JPH::Factory::sInstance = new JPH::Factory();
    JPH::RegisterTypes();

    impl_->tempAllocator = std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);
    const int threadCount = std::max(1, static_cast<int>(std::thread::hardware_concurrency()) - 1);
    impl_->jobSystem = std::make_unique<JPH::JobSystemThreadPool>(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers,
                                                                     threadCount);
    impl_->physicsSystem = std::make_unique<JPH::PhysicsSystem>();
    impl_->physicsSystem->Init(1024, 0, 1024, 1024, impl_->broadPhaseLayerInterface,
                               impl_->objectVsBroadPhaseLayerFilter, impl_->objectLayerPairFilter);
    impl_->initialized = true;
    return true;
}

void JoltPhysicsWorld::Shutdown() {
    if (!impl_->initialized) {
        return;
    }
    impl_->physicsSystem.reset();
    impl_->jobSystem.reset();
    impl_->tempAllocator.reset();
    JPH::UnregisterTypes();
    delete JPH::Factory::sInstance;
    JPH::Factory::sInstance = nullptr;
    impl_->initialized = false;
}

void JoltPhysicsWorld::Step(float deltaTime) {
    if (!impl_->initialized) {
        return;
    }
    constexpr int collisionSteps = 1;
    impl_->physicsSystem->Update(deltaTime, collisionSteps, impl_->tempAllocator.get(), impl_->jobSystem.get());
}

u64 JoltPhysicsWorld::AddBox(const Vec3& position, const Vec3& halfExtent, bool dynamic) {
    if (!impl_->initialized) {
        return 0;
    }
    JPH::BoxShapeSettings shapeSettings(JPH::Vec3(halfExtent.x, halfExtent.y, halfExtent.z));
    shapeSettings.SetEmbedded();
    JPH::ShapeSettings::ShapeResult shapeResult = shapeSettings.Create();
    if (shapeResult.HasError()) {
        return 0;
    }
    JPH::ShapeRefC shape = shapeResult.Get();
    JPH::BodyCreationSettings settings(shape, JPH::RVec3(position.x, position.y, position.z),
                                       JPH::Quat::sIdentity(),
                                       dynamic ? JPH::EMotionType::Dynamic : JPH::EMotionType::Static,
                                       dynamic ? layers::MOVING : layers::NON_MOVING);
    settings.mFriction = 0.5f;
    settings.mRestitution = 0.1f;
    JPH::BodyInterface& bodyInterface = impl_->physicsSystem->GetBodyInterface();
    JPH::Body* body = bodyInterface.CreateBody(settings);
    if (!body) {
        return 0;
    }
    bodyInterface.AddBody(body->GetID(), JPH::EActivation::Activate);
    return body->GetID().GetIndexAndSequenceNumber();
}

Vec3 JoltPhysicsWorld::GetPosition(u64 bodyId) const {
    if (!impl_->initialized) {
        return Vec3{0.0f};
    }
    const JPH::BodyLockInterface& lockInterface = impl_->physicsSystem->GetBodyLockInterface();
    JPH::BodyID id(static_cast<JPH::uint32>(bodyId));
    JPH::BodyLockRead lock(lockInterface, id);
    if (!lock.Succeeded()) {
        return Vec3{0.0f};
    }
    const JPH::RVec3 position = lock.GetBody().GetPosition();
    return Vec3{static_cast<float>(position.GetX()), static_cast<float>(position.GetY()),
                static_cast<float>(position.GetZ())};
}
} // namespace waypoint::physics
