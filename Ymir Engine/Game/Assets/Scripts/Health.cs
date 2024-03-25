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
        if (/*godmode*/ true)
        {
            if (Input.GetKey(YmirKeyCode.F7) == KeyState.KEY_DOWN)
            {
                Debug.Log("Take debug dmg");

                if (TakeDmg(debugDmg))
                {
                    DeathScreen();
                }
            }

            if (Input.GetKey(YmirKeyCode.F8) == KeyState.KEY_DOWN)
            {
                Debug.Log("Instant win");

                WinScreen();
            }

            if (Input.GetKey(YmirKeyCode.F9) == KeyState.KEY_DOWN)
            {
                Debug.Log("Instant lose");


                DeathScreen();
            }
        }

        return;
    }

    public bool TakeDmg(float dmg)
    {
        if (/*godmode*/ true)
        {
            currentHealth -= dmg;

            UI.SliderEdit(healthBar, currentHealth);

            if (currentHealth <= 0)
            {
                return true;
            }
        }

        return false;
    }

    public bool DeathScreen()
    {
        deathCanvas.SetActive(true);
        player.gameObject.SetActive(false);

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