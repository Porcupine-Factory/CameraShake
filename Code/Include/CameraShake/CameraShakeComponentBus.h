#pragma once

#include <CameraShake/CameraShakeTypeIds.h>
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/Math/Vector3.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzFramework/Physics/Collision/CollisionLayers.h>

namespace CameraShake
{
    class CameraShakeComponentRequests : public AZ::ComponentBus
    {
    public:
        ~CameraShakeComponentRequests() override = default;

        virtual void StartShake() = 0;
        virtual float GetTrauma() const = 0;
        virtual void SetTrauma(const float&) = 0;
        virtual float GetDecay() const = 0;
        virtual void SetDecay(const float&) = 0;
        virtual float GetSpeed() const = 0;
        virtual void SetSpeed(const float&) = 0;
    };

    using CameraShakeComponentRequestBus = AZ::EBus<CameraShakeComponentRequests>;

} // namespace CameraShake
