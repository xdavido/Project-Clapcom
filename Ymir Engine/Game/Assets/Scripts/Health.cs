using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

using YmirEngine;

public class Health : YmirComponent
{
    //public GameObject player = null;
    public GameObject healthBar = null;
    public GameObject deathCanvas = null;
    public GameObject winCanvas = null;

    public float currentHealth = 0;
    public float maxHealth = 7;

    public float debugDmg = 1;

    private Player player = null;

    public void Start()
    {
        Debug.Log("HelloWorld");

        GetPlayerScript();

        healthBar = InternalCalls.GetGameObjectByName("Health Bar");
        deathCanvas = InternalCalls.GetGameObjectByName("Death Canvas");
        winCanvas = InternalCalls.GetGameObjectByName("Win Canvas");

        UI.SliderEdit(healthBar, maxHealth);
        currentHealth = maxHealth;
    }

    public void Update()
    {
        if (player.godMode)
        {
            if (Input.GetKey(YmirKeyCode.F3) == KeyState.KEY_DOWN)
            {
                Debug.Log("Instant win");

                WinScreen();
            }

            if (Input.GetKey(YmirKeyCode.F4) == KeyState.KEY_DOWN)
            {
                Debug.Log("Instant lose");

                DeathScreen();
            }
        }

        else
        {
            if (Input.GetKey(YmirKeyCode.F5) == KeyState.KEY_DOWN)
            {
                Debug.Log("Take debug dmg");

                TakeDmg(debugDmg);
            }
        }

        return;
    }

    public bool TakeDmg(float dmg)
    {
        if (!player.godMode)
        {
            currentHealth -= dmg;

            UI.SliderEdit(healthBar, currentHealth);

            if (currentHealth <= 0)
            {
                DeathScreen();
                return true;
            }
        }

        return false;
    }

    public bool DeathScreen()
    {
        deathCanvas.SetActive(true);
        player.gameObject.SetActive(false);
        Audio.StopAllAudios();

        return true;
    }

    public bool WinScreen()
    {
        winCanvas.SetActive(true);
        player.gameObject.SetActive(false);

        return true;
    }

    private void GetPlayerScript()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");

        if (gameObject != null)
        {
            player = gameObject.GetComponent<Player>();
        }
    }

    private void GetAnotherScript()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");

        if (gameObject != null)
        {
            PlayerMovement p = gameObject.GetComponent<PlayerMovement>();

            //Debug.Log("MovmentSpeed= " + movementSpeed);

        }
    }
}