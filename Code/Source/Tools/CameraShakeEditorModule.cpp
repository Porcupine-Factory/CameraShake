
#include <CameraShake/CameraShakeTypeIds.h>
#include <CameraShakeModuleInterface.h>
#include "CameraShakeEditorSystemComponent.h"

namespace CameraShake
{
    class CameraShakeEditorModule
        : public CameraShakeModuleInterface
    {
    public:
        AZ_RTTI(CameraShakeEditorModule, CameraShakeEditorModuleTypeId, CameraShakeModuleInterface);
        AZ_CLASS_ALLOCATOR(CameraShakeEditorModule, AZ::SystemAllocator);

        CameraShakeEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                CameraShakeEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<CameraShakeEditorSystemComponent>(),
            };
        }
    };
}// namespace CameraShake

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), CameraShake::CameraShakeEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_CameraShake_Editor, CameraShake::CameraShakeEditorModule)
#endif
