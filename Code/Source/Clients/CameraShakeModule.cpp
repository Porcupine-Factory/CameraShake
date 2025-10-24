
#include "CameraShakeSystemComponent.h"
#include <CameraShake/CameraShakeTypeIds.h>
#include <CameraShakeModuleInterface.h>

namespace CameraShake
{
    class CameraShakeModule : public CameraShakeModuleInterface
    {
    public:
        AZ_RTTI(CameraShakeModule, CameraShakeModuleTypeId, CameraShakeModuleInterface);
        AZ_CLASS_ALLOCATOR(CameraShakeModule, AZ::SystemAllocator);
    };
} // namespace CameraShake

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), CameraShake::CameraShakeModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_CameraShake, CameraShake::CameraShakeModule)
#endif
