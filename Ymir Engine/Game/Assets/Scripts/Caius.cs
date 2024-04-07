using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Caius : YmirComponent
{

	public GameObject name_gameObject;
	public GameObject line1_gameObject;
	public GameObject line2_gameObject;
	public GameObject line3_gameObject;
	public GameObject ui_gameObject;

	private bool talked = false;
	private bool dialogue_ui = false;


    public void Start()
	{
		
	}

	public void Update()
	{
		//TODO: Show the dialogue UI when the bool is true
		if (dialogue_ui)
		{
			UI.TextEdit(name_gameObject, "Lorem ipsum");
			ui_gameObject.SetActive(true);
			
        }
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player" && talked == false)
        {
            //TODO: Don't let the player move
			

			//Show the dialogue UI
			dialogue_ui = true;

			//Don't let repeat the dialogue
			talked = true;
        }
    }
}