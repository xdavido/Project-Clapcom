using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Button_Navigation : YmirComponent
{
    public string sceneName = "Assets/Scenes/GameUI.yscene";

    public void OnClickButton()
    {
        Debug.Log("Go to scene" + sceneName);
        InternalCalls.LoadScene(sceneName);
    }
}