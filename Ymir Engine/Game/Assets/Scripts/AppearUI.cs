using System;
using System.Collections;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class AppearUI : YmirComponent
{
	public bool enter = true;
	public void Start()
	{

	}
	public void Update()
	{

       

        return;
	}

	public void OnCollisionEnter(GameObject other)
	{
		Debug.Log(other.Tag);
    
		if (other.Tag == "Pared" && enter == true)
		{
			UI.CreateImageUI(gameObject, "Assets/pato.png", 1, 1);
            
            enter = false;
		} 
	}
}