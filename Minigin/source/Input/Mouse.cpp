#include "Mouse.h"
#include <SDL.h>

namespace dae
{
    class Mouse::MouseImpl final
    {
    public:
        void Update();
        bool IsButtonDown(MouseButton button) const;
        bool IsButtonUp(MouseButton button) const;
        bool MovedThisFrame() const;
        glm::vec2 GetMousePos() const;
        glm::vec2 GetMouseDeltaPos() const;

    private:
        glm::vec2 m_Pos{}, m_DeltaPos{};
        MouseFlags m_Flags{ MouseFlags::None };

        bool IsFlagSet(MouseFlags flag) const;
        void SetFlag(MouseFlags flag, bool set);
    };
}
void dae::Mouse::MouseImpl::Update()
{
    int x{}, y{};
    Uint32 mouseState{ SDL_GetMouseState(&x, &y) };
    glm::vec2 mousePos{ static_cast<float>(x), static_cast<float>(y) };
    m_DeltaPos = mousePos - m_Pos;
    m_Pos = mousePos;

    bool leftDown{ static_cast<bool>(mouseState & SDL_BUTTON_LEFT) };
    bool rightDown{ static_cast<bool>(mouseState & SDL_BUTTON_RIGHT) };
    bool middleDown{ static_cast<bool>(mouseState & SDL_BUTTON_MIDDLE) };

    if (leftDown && !IsFlagSet(MouseFlags::LeftButtonDown))
        SetFlag(MouseFlags::LeftButtonDown, true);
    else if (!leftDown && IsFlagSet(MouseFlags::LeftButtonDown))
        SetFlag(MouseFlags::LeftButtonDown, false);

    if (rightDown && !IsFlagSet(MouseFlags::RightButtonDown))
        SetFlag(MouseFlags::RightButtonDown, true);
    else if (!rightDown && IsFlagSet(MouseFlags::RightButtonDown))
        SetFlag(MouseFlags::RightButtonDown, false);

    if (middleDown && !IsFlagSet(MouseFlags::MiddleButtonDown))
        SetFlag(MouseFlags::MiddleButtonDown, true);
    else if (!middleDown && IsFlagSet(MouseFlags::MiddleButtonDown))
        SetFlag(MouseFlags::MiddleButtonDown, false);

    SetFlag(MouseFlags::MovedThisFrame, glm::dot(m_DeltaPos, m_DeltaPos) > 0.001f);
}
bool dae::Mouse::MouseImpl::IsButtonDown(MouseButton button) const
{
    switch (button)
    {
    case MouseButton::Left:
        return IsFlagSet(MouseFlags::LeftButtonDown);
        break;
    case MouseButton::Middle:
        return IsFlagSet(MouseFlags::MiddleButtonDown);
        break;
    case MouseButton::Right:
        return IsFlagSet(MouseFlags::RightButtonDown);
        break;
    }
    return false;
}
bool dae::Mouse::MouseImpl::IsButtonUp(MouseButton button) const
{
    return !IsButtonDown(button);
}

bool dae::Mouse::MouseImpl::MovedThisFrame() const
{
    return IsFlagSet(MouseFlags::MovedThisFrame);
}

glm::vec2 dae::Mouse::MouseImpl::GetMousePos() const
{
    return m_Pos;
}

glm::vec2 dae::Mouse::MouseImpl::GetMouseDeltaPos() const
{
    return m_DeltaPos;
}

bool dae::Mouse::MouseImpl::IsFlagSet(MouseFlags flag) const
{
    return (static_cast<int>(m_Flags) & static_cast<int>(flag));
}

void dae::Mouse::MouseImpl::SetFlag(MouseFlags flag, bool set)
{
    m_Flags = (set)
        ? static_cast<MouseFlags>(static_cast<int>(m_Flags) | static_cast<int>(flag))
        : static_cast<MouseFlags>(static_cast<int>(m_Flags) & ~static_cast<int>(flag));
}

dae::Mouse::Mouse()
    : m_pImpl{std::make_unique<MouseImpl>()}
{
}

dae::Mouse::~Mouse()
{
}

void dae::Mouse::Update()
{
    m_pImpl->Update();
}

bool dae::Mouse::IsButtonDown(MouseButton button) const
{
    return m_pImpl->IsButtonDown(button);
}

bool dae::Mouse::IsButtonUp(MouseButton button) const
{
    return m_pImpl->IsButtonUp(button);
}

bool dae::Mouse::MovedThisFrame() const
{
    return m_pImpl->MovedThisFrame();
}

glm::vec2 dae::Mouse::GetMousePos() const
{
    return m_pImpl->GetMousePos();
}

glm::vec2 dae::Mouse::GetMouseDeltaPos() const
{
    return m_pImpl->GetMouseDeltaPos();
}
