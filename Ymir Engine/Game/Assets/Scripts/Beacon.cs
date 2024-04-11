using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Beacon : YmirComponent
{
    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player")
        {
       
            InternalCalls.LoadScene("Assets/BASE_FINAL/LVL_BASE_FINAL.yscene");
        }
    }
}