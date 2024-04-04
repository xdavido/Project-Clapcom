using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Basic_Movment : YmirComponent
{

    //bool start = true;
    //float movementSpeed = 0.01f;

	public void Start()
	{
			
	}

	public void Update()
	{
        //-------------- Keyboard --------------//
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(0, 0, 10);
            gameObject.SetVelocity(vel);
        }
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(0, 0, -10);
            gameObject.SetVelocity(vel);
        }
        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(-10, 0, 0);
            gameObject.SetVelocity(vel);
        }
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(10, 0, 0);
            gameObject.SetVelocity(vel);
        }

        if(Input.GetKey(YmirKeyCode.W) == KeyState.KEY_IDLE && Input.GetKey(YmirKeyCode.S) == KeyState.KEY_IDLE && Input.GetKey(YmirKeyCode.A) == KeyState.KEY_IDLE && Input.GetKey(YmirKeyCode.D) == KeyState.KEY_IDLE)
        {
            gameObject.SetVelocity(Vector3.zero);
        }

        return;
	}
}