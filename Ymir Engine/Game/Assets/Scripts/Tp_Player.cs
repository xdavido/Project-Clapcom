using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Tp_Player : YmirComponent
{

	public void Start()
	{
			Debug.Log("HelloWorld"); 
	}

	public void OnCollisionEnter(GameObject other)
	{
		if(other.Tag == "Teleport") 
		{
			Debug.Log("jose");
			gameObject.SetPosition(new Vector3(10, 10, 0));
		}
	}
}