#include "SdlWindow.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

namespace dae
{

    class SdlWindow::SdlWindowImpl final
    {
    public:
        SdlWindowImpl(const WindowDesc& desc);
        ~SdlWindowImpl();

        void Init();
        void Shutdown();

        void Resize(uint32_t width, uint32_t height);

        uint32_t GetWidth() const { return m_Desc.width; }
        uint32_t GetHeight() const { return m_Desc.height; }

        uint32_t GetID() const;

    private:
        WindowDesc m_Desc;
        SDL_Window* m_pWindow{nullptr};
        static int s_Instances;

        void PrintSDLVersion();
    };
}

int dae::SdlWindow::SdlWindowImpl::s_Instances{0};

dae::SdlWindow::SdlWindowImpl::SdlWindowImpl(const WindowDesc& desc)
    : m_Desc{desc}
{
    ++s_Instances;
}

dae::SdlWindow::SdlWindowImpl::~SdlWindowImpl()
{
    if ((m_Desc.flags & WindowFlags::Initialized) != WindowFlags::None)
    {
        Shutdown();
    }

    --s_Instances;
}

void dae::SdlWindow::SdlWindowImpl::Init()
{
    if ((m_Desc.flags & WindowFlags::Initialized) != WindowFlags::None)
        return;

    if (s_Instances == 1)
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
        }

        PrintSDLVersion();
    }

    m_pWindow = SDL_CreateWindow(
        m_Desc.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_Desc.width,
        m_Desc.height,
        SDL_WINDOW_OPENGL //| SDL_WINDOW_RESIZABLE
    );
    if (m_pWindow == nullptr)
    {
        throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
    }

    m_Desc.flags |= WindowFlags::Initialized;
}

void dae::SdlWindow::SdlWindowImpl::Shutdown()
{

    if ((m_Desc.flags & WindowFlags::Initialized) != WindowFlags::None)
        return;

    SDL_DestroyWindow(m_pWindow);
    m_pWindow = nullptr;

    m_Desc.flags ^= WindowFlags::Initialized;

    if (s_Instances == 1)
    {
        SDL_Quit();
    }
}

uint32_t dae::SdlWindow::SdlWindowImpl::GetID() const
{
    return SDL_GetWindowID(m_pWindow);
}

void dae::SdlWindow::SdlWindowImpl::Resize(uint32_t width, uint32_t height)
{
    m_Desc.width = width;
    m_Desc.height = height;
}

void dae::SdlWindow::SdlWindowImpl::PrintSDLVersion()
{
    SDL_version version{};
    SDL_VERSION(&version);
    printf("We compiled against SDL version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    SDL_GetVersion(&version);
    printf("We are linking against SDL version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_IMAGE_VERSION(&version);
    printf("We compiled against SDL_image version %u.%u.%u ...\n",
        version.major, version.minor, version.patch);

    version = *IMG_Linked_Version();
    printf("We are linking against SDL_image version %u.%u.%u.\n",
        version.major, version.minor, version.patch);

    SDL_TTF_VERSION(&version)
        printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
            version.major, version.minor, version.patch);

    version = *TTF_Linked_Version();
    printf("We are linking against SDL_ttf version %u.%u.%u.\n",
        version.major, version.minor, version.patch);
}

dae::SdlWindow::SdlWindow(const WindowDesc& desc)
    : m_pImpl{std::make_unique<SdlWindowImpl>(desc) }
{
}

dae::SdlWindow::~SdlWindow()
{
}

void dae::SdlWindow::Init()
{
    m_pImpl->Init();
}

void dae::SdlWindow::Shutdown()
{
    m_pImpl->Shutdown();
}

uint32_t dae::SdlWindow::GetID() const
{
    return m_pImpl->GetID();
}

void dae::SdlWindow::Resize(uint32_t width, uint32_t height)
{
    m_pImpl->Resize(width, height);
}

uint32_t dae::SdlWindow::GetWidth() const
{
    return m_pImpl->GetWidth();
}

uint32_t dae::SdlWindow::GetHeight() const
{
    return m_pImpl->GetHeight();
}
