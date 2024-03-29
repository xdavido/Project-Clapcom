using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class AcidicSpit : YmirComponent
{
bool start = true;

	public void Update()
	{
		if(start) {

			Debug.Log("HelloWorld"); 

			start = false;
		}

		return;
	}
}