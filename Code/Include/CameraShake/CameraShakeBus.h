
#pragma once

#include <CameraShake/CameraShakeTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace CameraShake
{
    class CameraShakeRequests
    {
    public:
        AZ_RTTI(CameraShakeRequests, CameraShakeRequestsTypeId);
        virtual ~CameraShakeRequests() = default;
        // Put your public methods here
    };

    class CameraShakeBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using CameraShakeRequestBus = AZ::EBus<CameraShakeRequests, CameraShakeBusTraits>;
    using CameraShakeInterface = AZ::Interface<CameraShakeRequests>;

} // namespace CameraShake
