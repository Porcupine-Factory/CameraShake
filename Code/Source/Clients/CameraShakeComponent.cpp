#include "CameraShakeComponent.h"

#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzCore/Component/Entity.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/Serialization/EditContext.h>
#include <AzFramework/Components/CameraBus.h>
#include <AzFramework/Physics/NameConstants.h>

namespace CameraShake
{
    using namespace StartingPointInput;

    void CameraShakeComponent::Reflect(AZ::ReflectContext* rc)
    {
        if (auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<CameraShakeComponent, AZ::Component>()

                // Entity to shake
                ->Field("Shake Entity", &CameraShakeComponent::m_shakeEntityId)

                // Input Binding Key to perform shake
                ->Field("Shake Input Key", &CameraShakeComponent::m_strShake)

                // Main amplitude, frequency, and decay parameters of the shake noise functions.
                ->Field("Trauma", &CameraShakeComponent::m_traumaInitial)
                ->Field("Decay", &CameraShakeComponent::m_traumaDecay)
                ->Field("Speed", &CameraShakeComponent::m_freq)
                ->Attribute(AZ::Edit::Attributes::Suffix, " Hz")

                // Translational Shake
                ->Field("Translation Amplitudes", &CameraShakeComponent::m_translationAmplitudes)
                ->Attribute(AZ::Edit::Attributes::Suffix, " " + Physics::NameConstants::GetLengthUnit())
                // Rotational Shake
                ->Field("Rotation Amplitudes", &CameraShakeComponent::m_rotationAmplitudes)
                ->Attribute(AZ::Edit::Attributes::Suffix, " " + AZStd::string(" rad"))
                ->Version(1);

            if (AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit;
                ec->Class<CameraShakeComponent>("Camera Shake", "[Component that shakes the selected entity or active camera]")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(Attributes::AppearsInAddComponentMenu, AZ_CRC_CE("Game"))

                    ->DataElement(
                        nullptr,
                        &CameraShakeComponent::m_shakeEntityId,
                        "Shake Entity",
                        "Entity to shake (e.g., player camera or primitive). Defaults to active camera if blank.")
                    ->DataElement(nullptr, &CameraShakeComponent::m_strShake, "Shake Input Key", "Key to initiate or test shake.")
                    ->DataElement(nullptr, &CameraShakeComponent::m_traumaInitial, "Trauma", "Total shake strength")
                    ->DataElement(
                        nullptr,
                        &CameraShakeComponent::m_translationAmplitudes,
                        "Translation Amplitudes",
                        "X/Y/Z translational shake strengths")
                    ->DataElement(
                        nullptr, &CameraShakeComponent::m_rotationAmplitudes, "Rotation Amplitudes", "X/Y/Z rotational shake strengths")
                    ->DataElement(
                        nullptr,
                        &CameraShakeComponent::m_traumaDecay,
                        "Decay",
                        "Adjusts the length of the shake. Value of 0 produces infinite length.")
                    ->DataElement(nullptr, &CameraShakeComponent::m_freq, "Speed", "Frequency of the shake");
            }
        }

        if (auto bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->EBus<CameraShakeComponentRequestBus>("CameraShakeComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "camerashake")
                ->Attribute(AZ::Script::Attributes::Category, "Camera Shake")
                ->Event("Start Shake With Defaults", &CameraShakeComponentRequests::StartShakeWithDefaults)
                ->Event(
                    "Start Shake",
                    &CameraShakeComponentRequests::StartShake,
                    AZStd::array<AZ::BehaviorParameterOverrides, 6>{
                        { AZ::BehaviorParameterOverrides("ShakeEntityId", "Entity to shake (valid ID required)"),
                          AZ::BehaviorParameterOverrides("Trauma", "Initial trauma strength"),
                          AZ::BehaviorParameterOverrides("Decay", "Decay rate"),
                          AZ::BehaviorParameterOverrides("Speed", "Shake frequency"),
                          AZ::BehaviorParameterOverrides("TranslationAmplitudes", "X/Y/Z translation strengths"),
                          AZ::BehaviorParameterOverrides("RotationAmplitudes", "X/Y/Z rotation strengths") } })
                ->Event("Get Trauma", &CameraShakeComponentRequests::GetTrauma)
                ->Event("Set Trauma", &CameraShakeComponentRequests::SetTrauma)
                ->Event("Get Decay", &CameraShakeComponentRequests::GetDecay)
                ->Event("Set Decay", &CameraShakeComponentRequests::SetDecay)
                ->Event("Get Speed", &CameraShakeComponentRequests::GetSpeed)
                ->Event("Set Speed", &CameraShakeComponentRequests::SetSpeed)
                ->Event("Get Shake Entity Id", &CameraShakeComponentRequests::GetShakeEntityId)
                ->Event(
                    "Set Shake Entity Id",
                    &CameraShakeComponentRequests::SetShakeEntityId,
                    AZStd::array<AZ::BehaviorParameterOverrides, 1>{ { AZ::BehaviorParameterOverrides(
                        "Shake EntityId", "ID of the entity to shake (invalid falls back to active camera)") } });
        }
    }

    void CameraShakeComponent::Activate()
    {
        m_shakeEventId = StartingPointInput::InputEventNotificationId(m_strShake.c_str());
        InputEventNotificationBus::MultiHandler::BusConnect(m_shakeEventId);

        AZ::TickBus::Handler::BusConnect();

        m_defaultTraumaInitial = m_traumaInitial;
        m_defaultTraumaDecay = m_traumaDecay;
        m_defaultFreq = m_freq;
        m_defaultTranslationAmplitudes = m_translationAmplitudes;
        m_defaultRotationAmplitudes = m_rotationAmplitudes;
        m_defaultShakeEntityId = m_shakeEntityId;

        // When the entity is activated, set our current trauma level (m_trauma) to m_traumaInitial
        m_trauma = m_traumaInitial;

        // Setup shake entity
        if (!m_shakeEntityId.IsValid())
        {
            m_shakeEntityPtr = GetActiveCamera();
            if (m_shakeEntityPtr == nullptr)
            {
                m_needsCameraFallback = true;
                Camera::CameraNotificationBus::Handler::BusConnect();
            }
        }
        else
        {
            AZ::EntityBus::Handler::BusConnect(m_shakeEntityId);
        }

        CameraShakeComponentRequestBus::Handler::BusConnect(GetEntityId());
    }

    void CameraShakeComponent::Deactivate()
    {
        CameraShakeComponentRequestBus::Handler::BusDisconnect();

        AZ::TickBus::Handler::BusDisconnect();
        InputEventNotificationBus::MultiHandler::BusDisconnect();
        if (m_needsCameraFallback)
        {
            Camera::CameraNotificationBus::Handler::BusDisconnect();
        }
        AZ::EntityBus::Handler::BusDisconnect();
    }

    void CameraShakeComponent::OnEntityActivated(const AZ::EntityId& entityId)
    {
        if (entityId == m_shakeEntityId)
        {
            m_shakeEntityPtr = GetEntityPtr(m_shakeEntityId);
            AZ::EntityBus::Handler::BusDisconnect();
        }
    }

    void CameraShakeComponent::OnActiveViewChanged(const AZ::EntityId& activeEntityId)
    {
        if (m_needsCameraFallback)
        {
            m_shakeEntityPtr = GetEntityPtr(activeEntityId);
            if (m_shakeEntityPtr != nullptr)
            {
                m_shakeEntityId = activeEntityId;
                Camera::CameraNotificationBus::Handler::BusDisconnect();
                m_needsCameraFallback = false;
            }
        }
    }

    AZ::Entity* CameraShakeComponent::GetEntityPtr(AZ::EntityId pointer) const
    {
        auto ca = AZ::Interface<AZ::ComponentApplicationRequests>::Get();
        return ca->FindEntity(pointer);
    }

    // Recieve the input event in OnPressed method
    void CameraShakeComponent::OnPressed(float value)
    {
        const InputEventNotificationId* inputId = InputEventNotificationBus::GetCurrentBusId();
        if (inputId == nullptr)
        {
            return;
        }

        if (*inputId == m_shakeEventId && value > 0.f)
        {
            StartShakeWithDefaults();
        }
    }

    // Recieve the input event in OnReleased method
    void CameraShakeComponent::OnReleased(float value)
    {
        const InputEventNotificationId* inputId = InputEventNotificationBus::GetCurrentBusId();
        if (inputId == nullptr)
        {
            return;
        }
        if (*inputId == m_shakeEventId)
        {
            m_ShakeKey = value;
        }
    }

    void CameraShakeComponent::OnTick(float deltaTime, AZ::ScriptTimePoint)
    {
        Shake(deltaTime);
    }

    AZ::Entity* CameraShakeComponent::GetActiveCamera() const
    {
        AZ::EntityId activeCameraId;
        Camera::CameraSystemRequestBus::BroadcastResult(activeCameraId, &Camera::CameraSystemRequestBus::Events::GetActiveCamera);

        auto ca = AZ::Interface<AZ::ComponentApplicationRequests>::Get();
        return ca->FindEntity(activeCameraId);
    }

    void CameraShakeComponent::Shake(const float& deltaTime)
    {
        if (!m_initiateShake || !m_shakeEntityPtr)
        {
            return;
        }

        m_currentTime += deltaTime;

        AZ::SimpleLcgRandom getSeed;
        m_Random = getSeed.GetRandom();

        // Getting our entity's current local translation
        m_currentCameraTranslation = m_shakeEntityPtr->GetTransform()->GetLocalTM().GetTranslation();

        // Subtracting the translation caused by shake to get a "clean" version of our current local translation
        AZ::Vector3 adjustedCameraTranslation = m_currentCameraTranslation - m_shakeTranslation;

        // Getting our entity's current rotation using Euler Radians to make the math a bit easier
        m_currentCameraRotation = m_shakeEntityPtr->GetTransform()->GetLocalTM().GetRotation().GetEulerRadians();

        // Subtracting the rotation caused by shake to get a "clean" version of our current local rotation
        AZ::Vector3 adjustedCameraRotation = m_currentCameraRotation - m_shakeRotation;

        if (m_trauma > 0)
        {
            // Create a Vector3 with Perlin Noise values and amplitude multipliers for each axis. Used for translation.
            m_shakeTranslation = AZ::Vector3(
                                     GenFastNoise(m_Random) * m_translationAmplitudes.GetX(),
                                     GenFastNoise(m_Random + 2) * m_translationAmplitudes.GetY(),
                                     GenFastNoise(m_Random + 3) * m_translationAmplitudes.GetZ()) *
                (m_trauma * m_trauma);

            // Create a Quaternion with Perlin Noise values and amplitude multipliers for each axis. Used for rotation.
            AZ::Quaternion shakeRotation = AZ::Quaternion::CreateFromEulerAnglesRadians(
                AZ::Vector3(
                    GenFastNoise(m_Random + 4) * m_rotationAmplitudes.GetX(),
                    GenFastNoise(m_Random + 5) * m_rotationAmplitudes.GetY(),
                    GenFastNoise(m_Random + 6) * m_rotationAmplitudes.GetZ()) *
                (m_trauma * m_trauma));

            // Converting to Euler Radians to make the math a bit easier
            m_shakeRotation = shakeRotation.GetEulerRadians();

            // Set our entity's translation to the clean translation plus the shake values
            m_shakeEntityPtr->GetTransform()->SetLocalTranslation(adjustedCameraTranslation + m_shakeTranslation);

            // Set our entity's rotation to the clean rotation plus the shake values
            m_shakeEntityPtr->GetTransform()->SetLocalRotation(adjustedCameraRotation + m_shakeRotation);

            // Reducing the trauma amount over time. GetMax() ensures trauma is never less than 0
            m_trauma = AZ::GetMax(m_trauma - m_traumaDecay * deltaTime, 0.f);
        }
        else
        {
            m_initiateShake = false;
            m_trauma = m_traumaInitial;

            // Reset our shake/noise vectors back to 0
            m_shakeTranslation = AZ::Vector3::CreateZero();
            m_shakeRotation = AZ::Vector3::CreateZero();

            // Immediate reset to clean position/rotation
            m_shakeEntityPtr->GetTransform()->SetLocalTranslation(adjustedCameraTranslation);
            m_shakeEntityPtr->GetTransform()->SetLocalRotation(adjustedCameraRotation);
        }
    }

    void CameraShakeComponent::SetShakeEntity(const AZ::EntityId& id)
    {
        // Disconnect existing handlers
        if (m_needsCameraFallback)
        {
            Camera::CameraNotificationBus::Handler::BusDisconnect();
            m_needsCameraFallback = false;
        }
        AZ::EntityBus::Handler::BusDisconnect();

        m_shakeEntityId = id;
        m_shakeEntityPtr = nullptr;

        if (!m_shakeEntityId.IsValid())
        {
            m_shakeEntityPtr = GetActiveCamera();
            if (m_shakeEntityPtr == nullptr)
            {
                m_needsCameraFallback = true;
                Camera::CameraNotificationBus::Handler::BusConnect();
            }
        }
        else
        {
            m_shakeEntityPtr = GetEntityPtr(m_shakeEntityId);
            if (m_shakeEntityPtr == nullptr)
            {
                AZ::EntityBus::Handler::BusConnect(m_shakeEntityId);
            }
        }
    }

    float CameraShakeComponent::GenFastNoise(int Seed)
    {
        FastNoise noiseValues;
        noiseValues.SetNoiseType(FastNoise::Perlin);
        noiseValues.SetSeed(Seed);
        noiseValues.SetFrequency(m_freq);

        m_perlinFastNoise = noiseValues.GetPerlin(m_currentTime, m_currentTime);

        // AZ_Printf("", "Perlin FAST Number = %.10f", m_perlinFastNoise);
        return m_perlinFastNoise;
    }

    // Parameterless: Uses component defaults
    void CameraShakeComponent::StartShakeWithDefaults()
    {
        // Reset runtime state to editor component values (ignores prior overrides)
        m_traumaInitial = m_defaultTraumaInitial;
        m_traumaDecay = m_defaultTraumaDecay;
        m_freq = m_defaultFreq;
        m_translationAmplitudes = m_defaultTranslationAmplitudes;
        m_rotationAmplitudes = m_defaultRotationAmplitudes;
        SetShakeEntity(m_defaultShakeEntityId);

        // Refresh for fresh shake (uses new baseline)
        m_initiateShake = true;
        m_trauma = m_traumaInitial;
        m_currentTime = 0.f;
    }

    // Parameterized: Full override (all parameters required)
    void CameraShakeComponent::StartShake(
        const AZ::EntityId& shakeEntityId,
        const float& new_traumaInitial,
        const float& new_traumaDecay,
        const float& new_freq,
        const AZ::Vector3& new_translationAmplitudes,
        const AZ::Vector3& new_rotationAmplitudes)
    {
        if (shakeEntityId.IsValid())
        {
            SetShakeEntity(shakeEntityId);
        }
        m_traumaInitial = new_traumaInitial;
        m_traumaDecay = new_traumaDecay;
        m_freq = new_freq;
        m_translationAmplitudes = new_translationAmplitudes;
        m_rotationAmplitudes = new_rotationAmplitudes;

        // Start the shake with overridden values
        m_initiateShake = true;
        m_trauma = m_traumaInitial;
        m_currentTime = 0.f;
    }

    float CameraShakeComponent::GetTrauma() const
    {
        return m_traumaInitial;
    }

    void CameraShakeComponent::SetTrauma(const float& new_traumaInitial)
    {
        m_traumaInitial = new_traumaInitial;
    }

    float CameraShakeComponent::GetDecay() const
    {
        return m_traumaDecay;
    }

    void CameraShakeComponent::SetDecay(const float& new_traumaDecay)
    {
        m_traumaDecay = new_traumaDecay;
    }

    float CameraShakeComponent::GetSpeed() const
    {
        return m_freq;
    }

    void CameraShakeComponent::SetSpeed(const float& new_freq)
    {
        m_freq = new_freq;
    }

    AZ::EntityId CameraShakeComponent::GetShakeEntityId() const
    {
        return m_shakeEntityId;
    }

    void CameraShakeComponent::SetShakeEntityId(const AZ::EntityId& entityId)
    {
        SetShakeEntity(entityId);
    }
} // namespace CameraShake