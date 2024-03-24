using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class SceneManager : YmirComponent
{
	public void Update()
	{
        if (Input.GetKey(YmirKeyCode.KP_1) == KeyState.KEY_DOWN)
        {
            InternalCalls.LoadScene("Assets/Base_Blockout_Scene.yscene");
            return;
        }

        if (Input.GetKey(YmirKeyCode.KP_2) == KeyState.KEY_DOWN)
        {
            InternalCalls.LoadScene("Assets/MAPA_FINAL.yscene");
            return;
        }

        if (Input.GetKey(YmirKeyCode.KP_3) == KeyState.KEY_DOWN)
        {
            InternalCalls.LoadScene("Assets/Lv1_Blockout_Scene.yscene");
            return;
        }

        if (Input.GetKey(YmirKeyCode.KP_4) == KeyState.KEY_DOWN)
        {
            InternalCalls.LoadScene("Assets/Lvl2_Blockout_Part2_Scene.yscene");
            return;
        }

        return;
	}
}