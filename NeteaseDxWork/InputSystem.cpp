#include "InputSystem.h"
#include <Windows.h>

InputSystem* InputSystem::GetInstance()
{
    static InputSystem* instance;
    return instance;
}

void InputSystem::AddListener(InputLisenter* listener)
{
    listenerMap.insert(std::make_pair<InputLisenter*, InputLisenter*>
    (
        std::forward<InputLisenter*>(listener), 
        std::forward<InputLisenter*>(listener))
    );
}

void InputSystem::RemoveListener(InputLisenter* listener)
{
    std::map<InputLisenter*, InputLisenter*>::iterator iter = listenerMap.find(listener);

    if (iter != listenerMap.end())
        listenerMap.erase(iter);
}

void InputSystem::Update()
{
    if (GetKeyboardState(keyStates))
    {
        for (unsigned int i = 0; i < 256; i++)
        {
            // key down
            if (keyStates[i] & 0x80)
            {
                std::map<InputLisenter*, InputLisenter*>::iterator iter = listenerMap.begin();

                while (iter != listenerMap.end())
                {
                    iter->second->OnKeyDown(i);
                    ++iter;
                }
            }
            // key up
            else
            {
                if (keyStates[i] != prevKeyStates[i])
                {
                    std::map<InputLisenter*, InputLisenter*>::iterator iter = listenerMap.begin();

                    while (iter != listenerMap.end())
                    {
                        iter->second->OnKeyUp(i);
                        ++iter;
                    }
                }
            }
        }
                
        memcpy(prevKeyStates, keyStates, sizeof(unsigned char) * 256);
    }
}
