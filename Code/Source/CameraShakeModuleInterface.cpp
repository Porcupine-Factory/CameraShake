
#include "CameraShakeModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <CameraShake/CameraShakeTypeIds.h>

#include <Clients/CameraShakeSystemComponent.h>
#include <Clients/CameraShakeComponent.h>

namespace CameraShake
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(CameraShakeModuleInterface,
        "CameraShakeModuleInterface", CameraShakeModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(CameraShakeModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(CameraShakeModuleInterface, AZ::SystemAllocator);

    CameraShakeModuleInterface::CameraShakeModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            CameraShakeSystemComponent::CreateDescriptor(),
            CameraShakeComponent::CreateDescriptor(),
            });
    }

    AZ::ComponentTypeList CameraShakeModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<CameraShakeSystemComponent>(),
        };
    }
} // namespace CameraShake
