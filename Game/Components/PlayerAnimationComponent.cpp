
#include "PlayerAnimationComponent.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Texture2DComponent.hpp"
#include <SDL3/SDL.h>
#include <fstream>
#include <glm/glm.hpp>
#include <map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace dae;

struct AnimationStateInfo
{
    glm::ivec2 startPos;
    glm::ivec2 dimensions;
    int FrameCount;
    int FrameDuration;
};

class dae::PlayerAnimationComponent::Impl
{
  public:
    Impl(GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath);
    ~Impl();
    void SetAnimationState(std::string const &animState);
    void Update();
    void InitAnimationFrame();

  private:
    json m_SpriteSheetJson{};
    std::string m_CurrentStateKey{};
    AnimationStateInfo *m_CurrentState{nullptr};
    std::unordered_map<std::string, AnimationStateInfo> m_States;
    GameObject *m_Owner{nullptr};
    int m_CurrentFrameCount{};
    int m_CurrentAnimationFrame{};
    SDL_Surface *m_SpriteSheetSurface;
    Texture2DComponent *m_CurrentPlayerTexture{nullptr};
    bool m_HasState{false};

    void InitStateInfo();
};

PlayerAnimationComponent::PlayerAnimationComponent(
    GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath)
    : Component(owner), m_pImpl{std::make_unique<Impl>(owner, animationDataPath, spriteSheetPath)}
{
}

dae::PlayerAnimationComponent::~PlayerAnimationComponent() = default;

void dae::PlayerAnimationComponent::Update()
{
    m_pImpl->Update();
}

void dae::PlayerAnimationComponent::SetAnimationState(std::string const &animState)
{
    m_pImpl->SetAnimationState(animState);
}

dae::PlayerAnimationComponent::Impl::Impl(
    GameObject &owner, std::string const &animationDataPath, std::string const &spriteSheetPath)
    : m_Owner{&owner}
{
    m_CurrentPlayerTexture = owner.GetComponent<Texture2DComponent>();
    m_SpriteSheetSurface = SDL_LoadPNG(spriteSheetPath.c_str());
    m_SpriteSheetJson = json::parse(std::ifstream(animationDataPath));
    InitStateInfo();
}

void dae::PlayerAnimationComponent::Impl::SetAnimationState(std::string const &state)
{
    m_HasState = true;
    m_CurrentStateKey = state;
    m_CurrentState = &m_States[m_CurrentStateKey];
    m_CurrentAnimationFrame = 0;
    m_CurrentFrameCount = 0;
    InitAnimationFrame();
}

void dae::PlayerAnimationComponent::Impl::Update()
{
    if (!m_HasState || m_CurrentState->FrameDuration == -1)
        return;

    ++m_CurrentFrameCount;
    if (m_CurrentFrameCount >= m_CurrentState->FrameDuration)
    {
        m_CurrentFrameCount = 0;
        ++m_CurrentAnimationFrame;
        if (m_CurrentAnimationFrame >= m_CurrentState->FrameCount)
            m_CurrentAnimationFrame = 0;
        InitAnimationFrame();
    }
}

void dae::PlayerAnimationComponent::Impl::InitAnimationFrame()
{
    SDL_Surface *animationFrame = SDL_CreateSurface(
        m_CurrentState->dimensions.x * 2, m_CurrentState->dimensions.y * 2, m_SpriteSheetSurface->format);

    SDL_Rect frameRect{
        m_CurrentState->startPos.x + (m_CurrentAnimationFrame * m_CurrentState->dimensions.x),
        m_CurrentState->startPos.y, m_CurrentState->dimensions.x, m_CurrentState->dimensions.y};

    SDL_Rect destRect{0, 0, m_CurrentState->dimensions.x * 2, m_CurrentState->dimensions.y * 2};

    SDL_BlitSurfaceScaled(m_SpriteSheetSurface, &frameRect, animationFrame, &destRect, SDL_SCALEMODE_NEAREST);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), animationFrame);
    m_CurrentPlayerTexture->SetSDLTexture(texture);
    SDL_DestroySurface(animationFrame);
}

void dae::PlayerAnimationComponent::Impl::InitStateInfo()
{
    for (auto const &[key, val] : m_SpriteSheetJson.items())
    {
        m_States[key] = AnimationStateInfo{
            glm::vec2{val["startPosX"].get<float>(), val["startPosY"].get<float>()},
            glm::vec2{val["Width"].get<float>(), val["Height"].get<float>()}, val["FrameCount"].get<int>(),
            val["FrameDuration"].get<int>()};
    }
}

dae::PlayerAnimationComponent::Impl::~Impl()
{
    SDL_DestroySurface(m_SpriteSheetSurface);
}
