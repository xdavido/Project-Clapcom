using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Ui_appear : YmirComponent
{
	bool start = true;
	GameObject referenceUI;

    public void Start()
	{

	}
	public void Update()
	{
		

		return;
	}
	public void OnCollisionEnter(GameObject other)
	{
		Debug.Log("JOSEMARIA");
		if (other.Tag == "Pared" && start == true) 
		{
            referenceUI = UI.CreateImageUI(gameObject, "Assets/pato.png", 1, 1);
			start = false;
        }
		else
		{
			//InternalCalls.Destroy(referenceUI);

        }
	}
}