
#include "CameraShakeEditorSystemComponent.h"
#include <AzCore/Serialization/SerializeContext.h>

#include <CameraShake/CameraShakeTypeIds.h>

namespace CameraShake
{
    AZ_COMPONENT_IMPL(
        CameraShakeEditorSystemComponent, "CameraShakeEditorSystemComponent", CameraShakeEditorSystemComponentTypeId, BaseSystemComponent);

    void CameraShakeEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<CameraShakeEditorSystemComponent, CameraShakeSystemComponent>()->Version(0);
        }
    }

    CameraShakeEditorSystemComponent::CameraShakeEditorSystemComponent() = default;

    CameraShakeEditorSystemComponent::~CameraShakeEditorSystemComponent() = default;

    void CameraShakeEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("CameraShakeEditorService"));
    }

    void CameraShakeEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("CameraShakeEditorService"));
    }

    void CameraShakeEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void CameraShakeEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void CameraShakeEditorSystemComponent::Activate()
    {
        CameraShakeSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void CameraShakeEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        CameraShakeSystemComponent::Deactivate();
    }

} // namespace CameraShake
