#pragma once

#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzFramework/Physics/Collision/CollisionLayers.h>
#include <CameraShake/CameraShakeTypeIds.h>

namespace CameraShake
{
    class CameraShakeComponentRequests : public AZ::ComponentBus
    {
    public:
        ~CameraShakeComponentRequests() override = default;

        virtual void StartShakeWithDefaults() = 0;
        virtual void StartShake(const AZ::EntityId&, const float&, const float&, const float&, const AZ::Vector3&, const AZ::Vector3&) = 0;
        virtual void AddTrauma(float amount) = 0;
        virtual float GetTrauma() const = 0;
        virtual void SetTrauma(const float&) = 0;
        virtual float GetDecay() const = 0;
        virtual void SetDecay(const float&) = 0;
        virtual float GetSpeed() const = 0;
        virtual void SetSpeed(const float&) = 0;
        virtual AZ::EntityId GetShakeEntityId() const = 0;
        virtual void SetShakeEntityId(const AZ::EntityId&) = 0;
    };

    using CameraShakeComponentRequestBus = AZ::EBus<CameraShakeComponentRequests>;

} // namespace CameraShake
