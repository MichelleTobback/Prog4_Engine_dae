#include "Mouse.h"
#include <SDL.h>
#include "Core/BitFlag.h"

namespace dae
{
    class Mouse::MouseImpl final
    {
    public:
        void Update();
        bool IsButtonDown(MouseButton button) const;
        bool IsButtonUp(MouseButton button) const;
        bool IsButtonDownLastFrame(MouseButton button) const;
        bool IsButtonUpLastFrame(MouseButton button) const;
        bool MovedThisFrame() const;
        glm::vec2 GetMousePos() const;
        glm::vec2 GetMouseDeltaPos() const;

    private:
        glm::vec2 m_Pos{}, m_DeltaPos{};
        MouseFlags m_Flags{ MouseFlags::None };
        MouseFlags m_PrevFlags{ MouseFlags::None };

        bool IsFlagSet(MouseFlags flag) const;
        void SetFlag(MouseFlags flag, bool set);
    };
}
void dae::Mouse::MouseImpl::Update()
{
    m_PrevFlags = m_Flags;

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
    MouseFlags buttonDownFlag{Mouse::GetButtonDownFlag(button)};
    return buttonDownFlag != MouseFlags::None && IsFlagSet(buttonDownFlag);
}
bool dae::Mouse::MouseImpl::IsButtonUp(MouseButton button) const
{
    return !IsButtonDown(button);
}

bool dae::Mouse::MouseImpl::IsButtonDownLastFrame(MouseButton button) const
{
    MouseFlags buttonDownFlag{ Mouse::GetButtonDownFlag(button) };
    return buttonDownFlag != MouseFlags::None && BitFlag::IsSet(m_PrevFlags, buttonDownFlag);
}

bool dae::Mouse::MouseImpl::IsButtonUpLastFrame(MouseButton button) const
{
    return !IsButtonDownLastFrame(button);
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

dae::Mouse::MouseFlags dae::Mouse::GetButtonDownFlag(MouseButton button)
{
    switch (button)
    {
    case MouseButton::Left:
        return MouseFlags::LeftButtonDown;
        break;
    case MouseButton::Middle:
        return MouseFlags::MiddleButtonDown;
        break;
    case MouseButton::Right:
        return MouseFlags::RightButtonDown;
        break;
    }
    return MouseFlags::None;
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

bool dae::Mouse::IsButtonPressed(MouseButton button) const
{
    return IsButtonDown(button) && m_pImpl->IsButtonUpLastFrame(button);
}

bool dae::Mouse::IsButtonReleased(MouseButton button) const
{
    return IsButtonUp(button) && m_pImpl->IsButtonDownLastFrame(button);
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
