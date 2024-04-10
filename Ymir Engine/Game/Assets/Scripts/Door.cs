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
        if (other.Tag == "Player")
        {
			gameObject.SetPosition(gameObject.transform.globalPosition + new Vector3(0, -5, 0));
			//gameObject.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
			//gameObject.SetActive(false);
			
        }
    }
}