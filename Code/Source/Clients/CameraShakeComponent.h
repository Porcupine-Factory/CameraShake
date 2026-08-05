#pragma once
#include "CameraShake/CameraShakeComponentBus.h"
#include <AzCore/Component/Component.h>
#include <AzCore/Component/EntityBus.h>
#include <AzCore/Component/TickBus.h>
#include <AzCore/Math/Quaternion.h>
#include <AzCore/Math/Random.h>
#include <AzCore/Math/Vector3.h>
#include <AzFramework/Components/CameraBus.h>
#include <CameraShake/CameraShakeTypeIds.h>
#include <External/FastNoise/FastNoise.h>
#include <StartingPointInput/InputEventNotificationBus.h>

namespace CameraShake
{

    class CameraShakeComponent
        : public AZ::Component
        , public AZ::TickBus::Handler
        , public StartingPointInput::InputEventNotificationBus::MultiHandler
        , public AZ::EntityBus::Handler
        , public Camera::CameraNotificationBus::Handler
        , public CameraShakeComponentRequestBus::Handler
    {
    public:
        AZ_COMPONENT(CameraShakeComponent, CameraShakeComponentTypeId);

        // Provide runtime reflection
        static void Reflect(AZ::ReflectContext* rc);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        // AZ::InputEventNotificationBus interface. Overrides OnPressed and OnReleased virtual methods.
        void OnPressed(float value) override;
        void OnReleased(float value) override;

        void OnTick(float deltaTime, AZ::ScriptTimePoint) override;

        AZ::Entity* GetEntityPtr(AZ::EntityId pointer) const;

        // AZ::EntityBus::Handler overrides
        void OnEntityActivated(const AZ::EntityId& entityId) override;

        // Camera::CameraNotificationBus::Handler overrides
        void OnActiveViewChanged(const AZ::EntityId& activeEntityId) override;

        // CameraShakeComponentRequestBus
        void StartShakeWithDefaults() override;
        void StartShake(
            const AZ::EntityId& shakeEntityId,
            const float& new_traumaInitial,
            const float& new_traumaDecay,
            const float& new_freq,
            const AZ::Vector3& new_translationAmplitudes,
            const AZ::Vector3& new_rotationAmplitudes) override;
        float GetTrauma() const override;
        void SetTrauma(const float& new_traumaInitial) override;
        float GetDecay() const override;
        void SetDecay(const float& new_traumaDecay) override;
        float GetSpeed() const override;
        void SetSpeed(const float& new_freq) override;
        AZ::EntityId GetShakeEntityId() const override;
        void SetShakeEntityId(const AZ::EntityId& new_entityId) override;

    private:
        AZ::Entity* GetActiveCamera() const;

        AZ::EntityId m_shakeEntityId;
        AZ::EntityId m_defaultShakeEntityId;
        AZ::Entity* m_shakeEntityPtr = nullptr;
        bool m_needsCameraFallback = false;

        StartingPointInput::InputEventNotificationId m_shakeEventId;
        AZStd::string m_strShake = "Camera Shake";

        void Shake(const float& deltaTime);
        float GenFastNoise(int genSeed);
        void SetShakeEntity(const AZ::EntityId& id);
        void RemoveShakeOffsets();

        bool m_initiateShake = false;

        AZ::Vector3 m_currentCameraTranslation = AZ::Vector3::CreateZero();
        AZ::Vector3 m_shakeTranslation = AZ::Vector3::CreateZero();
        AZ::Quaternion m_shakeRotation = AZ::Quaternion::CreateIdentity();

        // Component defaults
        float m_defaultTraumaInitial = 1.3f;
        float m_defaultTraumaDecay = 2.f;
        float m_defaultFreq = 19.f;
        AZ::Vector3 m_defaultTranslationAmplitudes = AZ::Vector3(0.02f, 0.f, 0.02f);
        AZ::Vector3 m_defaultRotationAmplitudes = AZ::Vector3(0.04f, 0.01f, 0.05f);

        // Runtime copies
        float m_traumaInitial = m_defaultTraumaInitial;
        float m_traumaDecay = m_defaultTraumaDecay;
        float m_freq = m_defaultFreq;
        AZ::Vector3 m_translationAmplitudes = m_defaultTranslationAmplitudes;
        AZ::Vector3 m_rotationAmplitudes = m_defaultRotationAmplitudes;

        float m_trauma = 0.f;
        float m_perlinFastNoise = 0.f;
        float m_currentTime = 0.f;
        float m_perlinNoise = 0.f;
        float m_ShakeKey = 0.f;
        int m_Random = 0;
    };
} // namespace CameraShake
