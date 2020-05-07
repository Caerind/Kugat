#include <Enlivengine/Application/ActionSystem.hpp>

#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/Profiler.hpp>

namespace en
{

ActionInput::ActionInput(const std::string& name)
    : mName(name)
    , mID(Hash::CRC32(name.c_str()))
    , mActive(false) 
{
}

bool ActionInput::IsLogicalOperator() const
{
    return false;
}

const std::string& ActionInput::GetName() const
{
    return mName;
}

U32 ActionInput::GetID() const
{
    return mID;
}

bool ActionInput::IsActive() const
{
    return mActive;
}

void ActionInput::SetActive(bool active)
{
    mActive = active;
}

U32 ActionInput::GetPriorityLevel() const
{
    return 0;
}

ActionInputVariable::ActionInputVariable(const std::string& name, bool* variable)
	: ActionInput(name)
	, mVariable(variable)
{
}

ActionInputType ActionInputVariable::GetInputType() const
{
	return ActionInputType::Variable;
}

bool ActionInputVariable::IsCurrentlyActive(ActionSystem* system) const
{
	ENLIVE_UNUSED(system);
	if (mVariable != nullptr)
	{
		return *mVariable;
	}
	return false;
}

ActionInputFunction::ActionInputFunction(const std::string& name, FuncType function)
	: ActionInput(name)
	, mFunction(function)
{
}

ActionInputType ActionInputFunction::GetInputType() const
{
	return ActionInputType::Function;
}

bool ActionInputFunction::IsCurrentlyActive(ActionSystem* system) const
{
	ENLIVE_UNUSED(system);
	if (mFunction)
	{
		return mFunction();
	}
	return false;
}

ActionInputEvent::ActionInputEvent(const std::string& name, FuncType eventValidator)
	: ActionInput(name)
	, mEventValidator(eventValidator)
{
}

ActionInputType ActionInputEvent::GetInputType() const
{
	return ActionInputType::Event;
}

bool ActionInputEvent::IsCurrentlyActive(ActionSystem* system) const
{
	if (system != nullptr && mEventValidator)
	{
		const U32 eventCount = system->GetEventCount();
		for (U32 i = 0; i < eventCount; ++i)
		{
			if (mEventValidator(system->GetEvent(i)))
			{
				return true;
			}
		}
	}
	return false;
}

ActionInputKey::ActionInputKey(const std::string& name, sf::Keyboard::Key key, ActionType actionType /*= ActionType::Pressed*/)
	: ActionInput(name)
	, mKey(key)
	, mActionType(actionType)
{
}

ActionInputType ActionInputKey::GetInputType() const
{
	return ActionInputType::Key;
}

bool ActionInputKey::IsCurrentlyActive(ActionSystem* system) const
{
	if (mActionType == ActionType::Hold)
	{
		return sf::Keyboard::isKeyPressed(mKey);
	}
	else if (system != nullptr)
	{
		const U32 eventCount = system->GetEventCount();
		for (U32 i = 0; i < eventCount; ++i)
		{
			const sf::Event& event = system->GetEvent(i);
			if (mActionType == ActionType::Pressed)
			{
				if (event.type == sf::Event::EventType::KeyPressed && event.key.code == mKey)
				{
					return true;
				}
			}
			else if (mActionType == ActionType::Released)
			{
				if (event.type == sf::Event::EventType::KeyReleased && event.key.code == mKey)
				{
					return true;
				}
			}
		}
	}
	return false;
}

sf::Keyboard::Key ActionInputKey::GetKey() const
{
	return mKey;
}

ActionType ActionInputKey::GetType() const
{
	return mActionType;
}

void ActionInputKey::SetKey(sf::Keyboard::Key key)
{
	mKey = key;
}

void ActionInputKey::SetActionType(ActionType actionType)
{
	mActionType = actionType;
}

ActionInputMouse::ActionInputMouse(const std::string& name, sf::Mouse::Button button, ActionType actionType /*= ActionType::Pressed*/)
	: ActionInput(name)
	, mButton(button)
	, mActionType(actionType)
{
}

ActionInputType ActionInputMouse::GetInputType() const
{
	return ActionInputType::Mouse;
}

bool ActionInputMouse::IsCurrentlyActive(ActionSystem* system) const
{
	if (mActionType == ActionType::Hold)
	{
		return sf::Mouse::isButtonPressed(mButton);
	}
	else if (system != nullptr)
	{
		const U32 eventCount = system->GetEventCount();
		for (U32 i = 0; i < eventCount; ++i)
		{
			const sf::Event& event = system->GetEvent(i);
			if (mActionType == ActionType::Pressed)
			{
				if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == mButton)
				{
					return true;
				}
			}
			else if (mActionType == ActionType::Released)
			{
				if (event.type == sf::Event::EventType::MouseButtonReleased && event.mouseButton.button == mButton)
				{
					return true;
				}
			}
		}
	}
	return false;
}

sf::Mouse::Button ActionInputMouse::GetButton() const
{
	return mButton;
}

ActionType ActionInputMouse::GetType() const
{
	return mActionType;
}

void ActionInputMouse::SetButton(sf::Mouse::Button button)
{
	mButton = button;
}

void ActionInputMouse::SetActionType(ActionType actionType)
{
	mActionType = actionType;
}

ActionInputLogical::ActionInputLogical(const std::string& name, ActionInputLogicalOperator logic, U32 inputAID, U32 inputBID /*= U32_Max*/)
	: ActionInput(name)
	, mLogicOperator(logic)
	, mInputAID(inputAID)
	, mInputBID(inputBID)
	, mInputAIndex(0)
	, mInputBIndex(0)
	, mPriorityLevel(0)
{
}

ActionInputType ActionInputLogical::GetInputType() const
{
	switch (GetLogicalOperator())
	{
	case ActionInputLogicalOperator::And: return ActionInputType::And;
	case ActionInputLogicalOperator::Or: return ActionInputType::Or;
	case ActionInputLogicalOperator::Not: return ActionInputType::Not;
	default: assert(false);
	}
	return ActionInputType::Not;
}

bool ActionInputLogical::IsLogicalOperator() const
{
	return true;
}

bool ActionInputLogical::IsCurrentlyActive(ActionSystem* system) const
{
	if (system != nullptr)
	{
		switch (mLogicOperator)
		{
		case ActionInputLogicalOperator::And: return system->GetInputByIndex(mInputAIndex)->IsActive() && system->GetInputByIndex(mInputBIndex)->IsActive();
		case ActionInputLogicalOperator::Or: return system->GetInputByIndex(mInputAIndex)->IsActive() || system->GetInputByIndex(mInputBIndex)->IsActive();
		case ActionInputLogicalOperator::Not: return !system->GetInputByIndex(mInputAIndex)->IsActive();
		default: assert(false);
		}
	}
	return false;
}

U32 ActionInputLogical::GetPriorityLevel() const
{
	return mPriorityLevel;
}

ActionInputLogicalOperator ActionInputLogical::GetLogicalOperator() const
{
	return mLogicOperator;
}

U32 ActionInputLogical::GetInputAID() const
{
	return mInputAID;
}

U32 ActionInputLogical::GetInputBID() const
{
	return mInputBID;
}

void ActionInputLogical::SetInputAID(U32 inputID)
{
	mInputAID = inputID;
}

void ActionInputLogical::SetInputBID(U32 inputID)
{
	mInputBID = inputID;
}

U32 ActionInputLogical::GetInputAIndex() const
{
	return mInputAIndex;
}

U32 ActionInputLogical::GetInputBIndex() const
{
	return mInputBIndex;
}

void ActionInputLogical::SetInputAIndex(U32 inputAIndex)
{
	mInputAIndex = inputAIndex;
}

void ActionInputLogical::SetInputBIndex(U32 inputBIndex)
{
	mInputBIndex = inputBIndex;
}

void ActionInputLogical::SetPriorityLevel(U32 priorityLevel)
{
	mPriorityLevel = priorityLevel;
}

ActionSystem::ActionSystem()
    : mEvents()
    , mInputs()
    , mDirty(false)
{
}

void ActionSystem::Update()
{
	ENLIVE_PROFILE_FUNCTION();

    if (mDirty)
    {
        Update_Internal();
        mDirty = false;
    }

    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (ActionInput* input = mInputs[i])
        {
            input->SetActive(input->IsCurrentlyActive(this));
        }
    }

    ClearEvents();
}

bool ActionSystem::IsInputActive(const std::string& inputName) const
{
    assert(IsInputExisting(inputName));
    return IsInputActive(Hash::CRC32(inputName.c_str()));
}

bool ActionSystem::IsInputActive(U32 inputID) const
{
    assert(IsInputExisting(inputID));
    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (const ActionInput* input = mInputs[i])
        {
            if (input->GetID() == inputID)
            {
                return input->IsActive();
            }
        }
    }
    return false;
}

bool ActionSystem::IsInputExisting(const std::string& inputName) const
{
    return IsInputExisting(Hash::CRC32(inputName.c_str()));
}

bool ActionSystem::IsInputExisting(U32 inputID) const
{
    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (mInputs[i] != nullptr && mInputs[i]->GetID() == inputID)
        {
            return true;
        }
    }
    return false;
}

void ActionSystem::AddInputVariable(const std::string& name, bool* variable)
{
	AddInput_Internal(new ActionInputVariable(name, variable));
}

void ActionSystem::AddInputFunction(const std::string& name, ActionInputFunction::FuncType function)
{
	AddInput_Internal(new ActionInputFunction(name, function));
}

void ActionSystem::AddInputEvent(const std::string& name, ActionInputEvent::FuncType eventValidator)
{
	AddInput_Internal(new ActionInputEvent(name, eventValidator));
}

void ActionSystem::AddInputKey(const std::string& name, sf::Keyboard::Key key, ActionType actionType /*= ActionType::Pressed*/)
{
	AddInput_Internal(new ActionInputKey(name, key, actionType));
}

void ActionSystem::AddInputMouse(const std::string& name, sf::Mouse::Button button, ActionType actionType /*= ActionType::Pressed*/)
{
	AddInput_Internal(new ActionInputMouse(name, button, actionType));
}

void ActionSystem::AddInputAnd(const std::string& name, U32 inputAID, U32 inputBID)
{
	AddInput_Internal(new ActionInputLogical(name, ActionInputLogicalOperator::And, inputAID, inputBID));
}

void ActionSystem::AddInputOr(const std::string& name, U32 inputAID, U32 inputBID)
{
	AddInput_Internal(new ActionInputLogical(name, ActionInputLogicalOperator::Or, inputAID, inputBID));
}

void ActionSystem::AddInputNot(const std::string& name, U32 inputAID)
{
	AddInput_Internal(new ActionInputLogical(name, ActionInputLogicalOperator::Not, inputAID));
}

U32 ActionSystem::GetInputCount() const
{
    return static_cast<U32>(mInputs.size());
}

const ActionInput* ActionSystem::GetInputByIndex(U32 index) const
{
    assert(index < GetInputCount());
    return mInputs[index];
}

const ActionInput* ActionSystem::GetInputByName(const std::string& inputName) const
{
    return GetInputByID(Hash::CRC32(inputName.c_str()));
}

const ActionInput* ActionSystem::GetInputByID(U32 inputID) const
{
    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (mInputs[i] != nullptr && mInputs[i]->GetID() == inputID)
        {
            return mInputs[i];
        }
    }
    return nullptr;
}

void ActionSystem::RemoveInputByIndex(U32 index)
{
	assert(index < GetInputCount());
	delete mInputs[index];
	mInputs[index] = nullptr;
	mInputs.erase(mInputs.begin() + index);
	mDirty = true;
}

U32 ActionSystem::GetInputIndexFromName(const std::string& inputName) const
{
	return GetInputIndexFromID(Hash::CRC32(inputName.c_str()));
}

U32 ActionSystem::GetInputIndexFromID(U32 inputID) const
{
	const U32 inputCount = GetInputCount();
	for (U32 i = 0; i < inputCount; ++i)
	{
		if (mInputs[i]->GetID() == inputID)
		{
			return i;
		}
	}
	return U32_Max;
}

void ActionSystem::ClearInputs()
{
    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (mInputs[i] != nullptr)
        {
            delete mInputs[i];
            mInputs[i] = nullptr;
        }
    }
    mInputs.clear();
}

void ActionSystem::AddEvent(const sf::Event& event)
{
    mEvents.push_back(event);
}

U32 ActionSystem::GetEventCount() const
{
    return static_cast<U32>(mEvents.size());
}

const sf::Event& ActionSystem::GetEvent(U32 index) const
{
    assert(index < GetEventCount());
    return mEvents[index];
}

void ActionSystem::ClearEvents()
{
    mEvents.clear();
}

ActionInput* ActionSystem::GetInputByIndexNonConst(U32 index)
{
	assert(index < GetInputCount());
	return mInputs[index];
}

void ActionSystem::FlagPriorityAsDirty()
{
	mDirty = false;
}

void ActionSystem::AddInput_Internal(ActionInput* input)
{
    if (input != nullptr)
    {
        mInputs.push_back(input);
        mDirty = true;
    }
}

void ActionSystem::Update_Internal()
{
    bool priorityChanged = false;
    const U32 inputCount = GetInputCount();
    for (U32 i = 0; i < inputCount; ++i)
    {
        if (mInputs[i]->IsLogicalOperator())
        {
            if (ActionInputLogical* input = static_cast<ActionInputLogical*>(mInputs[i]))
            {
                const U32 previousPriority = input->GetPriorityLevel();
                const U32 currentPriority = GetMaxPriority_Internal(input->GetInputAID(), input->GetInputBID()) + 1;
                if (currentPriority != previousPriority)
                {
                    input->SetPriorityLevel(currentPriority);
                    priorityChanged = true;
                }
            }
        }
    }
    if (priorityChanged)
    {
        std::sort(mInputs.begin(), mInputs.end(), [](const ActionInput* a, const ActionInput* b)
        {
            assert(a != nullptr); //if (!a) return false;
            assert(b != nullptr); //if (!b) return true;
            const U32 pA = a->GetPriorityLevel();
            const U32 pB = b->GetPriorityLevel();
            if (pA == pB)
            {
                return a->GetID() < b->GetID();
            }
            else
            {
                return pA < pB;
            }
		});

		for (U32 i = 0; i < inputCount; ++i)
		{
			if (mInputs[i]->IsLogicalOperator())
			{
				if (ActionInputLogical* input = static_cast<ActionInputLogical*>(mInputs[i]))
				{
					input->SetInputAIndex(GetInputIndexFromID(input->GetInputAID()));
					input->SetInputBIndex(GetInputIndexFromID(input->GetInputBID()));
				}
			}
		}
    }
}

U32 ActionSystem::GetMaxPriority_Internal(U32 inputAID, U32 inputBID) const
{
    const ActionInput* inputA = GetInputByID(inputAID);
    const U32 priorityA = (inputA != nullptr) ? inputA->GetPriorityLevel() : 0;
    const ActionInput* inputB = GetInputByID(inputBID);
    const U32 priorityB = (inputB != nullptr) ? inputB->GetPriorityLevel() : 0;
    return (priorityA >= priorityB) ? priorityA : priorityB;
}

} // namespace en
