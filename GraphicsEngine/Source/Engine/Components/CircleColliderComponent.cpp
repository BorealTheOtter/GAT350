#include "pch.h"
#include "CircleColliderComponent.h"
#include "Framework/Actor.h"

namespace sr {
    FACTORY_REGISTER(CircleColliderComponent);

    bool CircleColliderComponent::CheckCollision(const ColliderComponent& o)
    {
        const CircleColliderComponent* cir_col = dynamic_cast<const CircleColliderComponent*>(&o);

        if (cir_col == nullptr) { return false; }

            float distance = GetOwner()->GetTransform().pos.Distance(o.GetOwner()->GetTransform().pos);

            return distance <= m_radius + cir_col->GetRadius();
        
        
    }
    void CircleColliderComponent::Read(const json::value_t& value)
    {
        ColliderComponent::Read(value);

        JSON_READ_NAME(value, "radius", m_radius);
    }
}
