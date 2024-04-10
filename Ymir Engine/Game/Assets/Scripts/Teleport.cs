using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Teleport : YmirComponent
{
    public string scene = "";

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

            
            
           InternalCalls.LoadScene("Assets/" + scene + ".yscene");
            
        

        }
    }

}