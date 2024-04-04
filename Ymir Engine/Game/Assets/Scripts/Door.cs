using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Door : YmirComponent
{
//bool start = true;

	public void Start()
	{
			Debug.Log("HelloWorld"); 
	}

	public void Update()
	{
		return;
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Name == "Player")
        {
            Debug.Log("[ERROR] Name: " + other.Name + "DOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOR");
			gameObject.parent.SetPosition(gameObject.transform.localPosition + new Vector3(0, 1, 0));
			
        }
    }
}