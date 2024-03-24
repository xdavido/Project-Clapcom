using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class UI_Bullets : YmirComponent
{
    //public GameObject player = null;
    public GameObject bulletsBar = null;

    public int currentBullets = 0;
    public int maxBullets = 7;

    public int debugShoot = 1;


    public void Start()
    {
        Debug.Log("HelloWorld"); 
        
        bulletsBar = InternalCalls.GetGameObjectByName("Bullets Text");

        currentBullets = maxBullets;
        UI.TextEdit(bulletsBar, currentBullets.ToString());
    }

    public void Update()
    {
        if (Input.GetKey(YmirKeyCode.Z) == KeyState.KEY_DOWN)
        {
            Debug.Log("Take debug dmg");
            UseBullets(debugShoot);
        }

        return;
    }

    public bool UseBullets(int bullet)
    {
        if (/*godmode*/ true)
        {
            currentBullets -= bullet;

            UI.TextEdit(bulletsBar, currentBullets.ToString());

            if (currentBullets <= 0)
            {
                return true;
            }
        }

        return false;
    }
}