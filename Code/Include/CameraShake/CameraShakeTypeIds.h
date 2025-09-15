
#pragma once

namespace CameraShake
{
    // System Component TypeIds
    inline constexpr const char* CameraShakeSystemComponentTypeId = "{F7625F3C-CB7F-4CDD-900D-53119ADE6C9B}";
    inline constexpr const char* CameraShakeEditorSystemComponentTypeId = "{A990D54A-E846-465B-A89E-6177D28A02F8}";

    // Component TypeId from CameraShakeComponent.h
    inline constexpr const char* CameraShakeComponentTypeId = "{CE4A6EDA-BC0D-4524-AAD5-994370E67F67}";

    // EBus-related IDs from CameraShakeComponentBus.h
    inline constexpr const char* CameraShakeNotificationHandlerTypeId = "{2F5A85D9-94C0-47EA-8CCE-5CFD1FAE8A7E}";

    // Module derived classes TypeIds
    inline constexpr const char* CameraShakeModuleInterfaceTypeId = "{78783B7C-3D46-440E-BEA6-2C97985724A8}";
    inline constexpr const char* CameraShakeModuleTypeId = "{D8CD2568-0964-4AA0-A42D-7BB53D1E6D9A}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* CameraShakeEditorModuleTypeId = CameraShakeModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* CameraShakeRequestsTypeId = "{97E63F86-71DE-4BCA-8196-05899E5281DD}";
} // namespace CameraShake
