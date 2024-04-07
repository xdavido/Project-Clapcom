using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Electric_trap : YmirComponent
{

    private float time = 0f;
    private bool activate = true;

    public void Start()
    {
        Debug.Log("HelloWorld");
    }

    public void Update()
    {


        if (activate)
        {

            if (time < 3f)
            {
                gameObject.transform.localScale = new Vector3(0.1f, 0.1f, 0.1f);
            }
            else
            {

                activate = false;
                time = 0f;
            }
        }
        else 
        {
           
            if (time < 3f)
            {
                gameObject.transform.localScale = new Vector3(1f, 1f, 1f);
            }
            else
            {
                activate = true;
                time = 0f;
            }
        }

        time += Time.deltaTime;
       
        return;
    }

    public void OnCollisionStay(GameObject other)
    {
        if (other.Tag == "Player")
        {
            
            //danoplayer

        }
    }
}