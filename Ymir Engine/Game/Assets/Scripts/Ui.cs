using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Ui : YmirComponent
{
    public GameObject referenceUI = null;
    public bool start = true;
    public void Start()
    { 

    }
    public void Update()
    {


        return;
    }
    public void OnCollisionEnter(GameObject other)
    {
        if(other.Tag == "Pared" && start == true) {
            referenceUI = UI.CreateImageUI(gameObject, "Assets/pato.png", 1, 1);
            start = false;
        }
        else if(other.Tag != "Pared" && start == false)
        {
            InternalCalls.Destroy(referenceUI);
        }
       
    }
    public void OnCollisionExit(GameObject _other)
    {
        Debug.Log("Josemaria");
    }
}