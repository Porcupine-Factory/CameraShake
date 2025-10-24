
#include "CameraShakeSystemComponent.h"

#include <CameraShake/CameraShakeTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace CameraShake
{
    AZ_COMPONENT_IMPL(CameraShakeSystemComponent, "CameraShakeSystemComponent", CameraShakeSystemComponentTypeId);

    void CameraShakeSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<CameraShakeSystemComponent, AZ::Component>()->Version(0);
        }
    }

    void CameraShakeSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("CameraShakeService"));
    }

    void CameraShakeSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("CameraShakeService"));
    }

    void CameraShakeSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void CameraShakeSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    CameraShakeSystemComponent::CameraShakeSystemComponent()
    {
        if (CameraShakeInterface::Get() == nullptr)
        {
            CameraShakeInterface::Register(this);
        }
    }

    CameraShakeSystemComponent::~CameraShakeSystemComponent()
    {
        if (CameraShakeInterface::Get() == this)
        {
            CameraShakeInterface::Unregister(this);
        }
    }

    void CameraShakeSystemComponent::Init()
    {
    }

    void CameraShakeSystemComponent::Activate()
    {
        CameraShakeRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void CameraShakeSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        CameraShakeRequestBus::Handler::BusDisconnect();
    }

    void CameraShakeSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace CameraShake
