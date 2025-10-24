
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/CameraShakeSystemComponent.h>

namespace CameraShake
{
    /// System component for CameraShake editor
    class CameraShakeEditorSystemComponent
        : public CameraShakeSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = CameraShakeSystemComponent;

    public:
        AZ_COMPONENT_DECL(CameraShakeEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        CameraShakeEditorSystemComponent();
        ~CameraShakeEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace CameraShake
