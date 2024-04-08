using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Button_Exit : YmirComponent
{
    public void Update()
    {
        if (Input.GetKey(YmirKeyCode.ESCAPE) == KeyState.KEY_DOWN)
        {
            InternalCalls.ExitGame();
        }
        return;
    }

    public void OnClickButton()
    {
        InternalCalls.ExitGame();
    }
}