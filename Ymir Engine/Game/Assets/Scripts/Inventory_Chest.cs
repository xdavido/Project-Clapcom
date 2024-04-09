using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Inventory_Chest : YmirComponent
{
    private bool dialogue_ui = false;
    public GameObject name_gameObject;
    public GameObject ui_gameObject;
    public void Start()
	{
			Debug.Log("HelloWorld");

    }

	public void Update()
	{

        if (dialogue_ui)
        {
            UI.TextEdit(name_gameObject, "Lorem ipsum");
            ui_gameObject.SetActive(true);
        }

            return;
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" &&  Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
        {


            //Show the dialogue UI
            dialogue_ui = true;


        }
    }
}
