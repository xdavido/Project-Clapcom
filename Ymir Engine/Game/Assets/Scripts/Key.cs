using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Key : YmirComponent
{
	public GameObject door;
	public void Start()
	{
		
	}

	public void Update()
	{
		
	}

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player")
        {
            if (Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
            {
                //TODO: Set unActive the door
                door.SetActive(false);
            }
        }
    }
}