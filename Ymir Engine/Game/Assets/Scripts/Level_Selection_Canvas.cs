using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Level_Selection_Canvas : YmirComponent
{
    private bool teleport_ui = false;
    public string toGameObject = "";

    public void Start()
    {
        Debug.Log("HelloWorld");
    }

    public void Update()
    {

        if (teleport_ui)
        { //open teleport_ui
            InternalCalls.GetGameObjectByName(toGameObject).SetActive(true);
            if (Input.GetGamepadButton(GamePadButton.B) == KeyState.KEY_DOWN || Input.GetKey(YmirKeyCode.ESCAPE) == KeyState.KEY_DOWN)
            {

                InternalCalls.GetGameObjectByName(toGameObject).SetActive(false);
                teleport_ui = false;
            }
        }
        return;
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" && Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN || other.Tag == "Player" && Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        {
            //Show the teleport UI
            teleport_ui = true;


        }

    }
}