#include "KeyboardInput.h"
#include "Binding.h"
#include "Command.h"
#include <algorithm>

using namespace dae;
dae::KeyboardInput::KeyboardInput() = default;
dae::KeyboardInput::~KeyboardInput() = default;
void KeyboardInput::ProcessInput()
{
	if (m_PreviousState.get() == nullptr) m_PreviousState = std::make_unique<bool[]>(SDL_SCANCODE_COUNT);
	/*if(m_CurrentState != nullptr)
		std::transform(m_CurrentState,
			m_CurrentState + SDL_SCANCODE_COUNT,
			begin(m_PreviousState),
			[&](bool key) { return key != 0; });*/
	if (m_CurrentState != nullptr)
		std::memcpy(m_PreviousState.get(), m_CurrentState, SDL_SCANCODE_COUNT);
	//m_PreviousState = m_CurrentState;
	SDL_PumpEvents();
	m_CurrentState = SDL_GetKeyboardState(nullptr);
	for (auto& binding : m_Bindings)
	{
		if (binding->m_TriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->Execute();
		if (binding->m_TriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->Execute();

		if (binding->m_EndTriggerState == InputState::None) continue;
		if (binding->m_EndTriggerState == InputState::JustPressed && WasPressedThisFrame(binding->m_Keybind)) binding->m_Command->StopExecution();
		else if (binding->m_EndTriggerState == InputState::JustReleased && WasReleasedThisFrame(binding->m_Keybind)) binding->m_Command->StopExecution();
		else if (binding->m_EndTriggerState == InputState::Pressed && IsButtonPressed(binding->m_Keybind)) binding->m_Command->StopExecution();
	}
}

bool KeyboardInput::WasPressedThisFrame(unsigned int button) const
{
	bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];
	
	return buttonChange && m_CurrentState[button];
}

bool KeyboardInput::IsButtonPressed(unsigned int button) const
{
	return m_CurrentState[button];
}

bool KeyboardInput::WasReleasedThisFrame(unsigned int button) const
{
	bool buttonChange = m_CurrentState[button] ^ m_PreviousState[button];

	return buttonChange && !m_CurrentState[button];
}

Binding* KeyboardInput::AddBinding(std::unique_ptr<Command> command, InputKeybinds keybind, InputState triggerState, InputState endTriggerState)
{
	assert(command.get() != nullptr);
	int scancode = ConvertToScancode(keybind);
	m_Bindings.emplace_back(std::make_unique<Binding>(std::move(command), scancode, triggerState, endTriggerState));
	return m_Bindings.back().get();
}

std::unique_ptr<Binding> KeyboardInput::UnBind(Binding* binding)
{
	auto it = std::find_if(begin(m_Bindings), end(m_Bindings), [binding](const auto& ptr) {return ptr.get() == binding; });
	if (it == m_Bindings.end()) return nullptr;
	auto foundBinding = std::move(*it);
	m_Bindings.erase(it);
	return foundBinding;
}

int KeyboardInput::ConvertToScancode(InputKeybinds keybind)
{
	assert(static_cast<int>(keybind) > static_cast<int>(InputKeybinds::KEYBOARD_BEGIN) 
		&& static_cast<int>(keybind) < static_cast<int>(InputKeybinds::KEYBOARD_END) &&
		"Keybind has to be a keyboard keybind");
	//only works for the chars in alphabeth but we don't need more for now
	return 4 + static_cast<int>(keybind) - static_cast<int>(InputKeybinds::A);
}