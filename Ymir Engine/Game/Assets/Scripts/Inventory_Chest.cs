using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Inventory_Chest : YmirComponent
{
    private bool inventory_ui = false;
    public void Start()
	{
			Debug.Log("HelloWorld");

    }

	public void Update()
	{

        if (inventory_ui) { //open inventory
            InternalCalls.GetGameObjectByName("Inventory").SetActive(true); 
            if (Input.GetGamepadButton(GamePadButton.B) == KeyState.KEY_DOWN || Input.GetKey(YmirKeyCode.ESCAPE) == KeyState.KEY_DOWN) {

                InternalCalls.GetGameObjectByName("Inventory").SetActive(false);
                inventory_ui = false;
            }
        }

       

            return;
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" &&  Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN || other.Tag == "Player" && Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        {
            //Show the inventory UI
            inventory_ui = true;


        }
    }
}
