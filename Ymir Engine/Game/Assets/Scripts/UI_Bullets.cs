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

    public int debugShoot = 1;

    Player player;

    public void Start()
    {
        Debug.Log("HelloWorld");

        GetAnotherScript();
        bulletsBar = InternalCalls.GetGameObjectByName("Bullets Text");

        UI.TextEdit(bulletsBar, player.magsize.ToString());
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

    public void UseBullets(int bullet)
    {
        UI.TextEdit(bulletsBar, player.ammo.ToString());
    }

    private void GetAnotherScript()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");

        if (gameObject != null)
        {
            player = gameObject.GetComponent<Player>();
        }
    }
}