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
    public float maxHealth = 10;
    public bool isAlive;

    public float debugDmg = 1;

    private Player player = null;

    public void Start()
    {
        Debug.Log("HelloWorld");

        GetPlayerScript();

        healthBar = InternalCalls.GetGameObjectByName("Health Bar");
        deathCanvas = InternalCalls.GetGameObjectByName("Death Canvas");
        winCanvas = InternalCalls.GetGameObjectByName("Win Canvas");

        UI.SliderSetMax(healthBar, maxHealth);
        UI.SliderEdit(healthBar, maxHealth);
        currentHealth = maxHealth;

        isAlive = true;
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

            if (Input.GetKey(YmirKeyCode.F6) == KeyState.KEY_DOWN)
            {
                Debug.Log("Get debug health");

                TakeDmg(-debugDmg);

            }
        }

        return;
    }

    public void TakeDmg(float dmg)
    {
        if (!player.godMode)
        {
            currentHealth -= dmg;

            if (currentHealth > maxHealth)
            {
                currentHealth = maxHealth;
            }

            else if (currentHealth <= 0)
            {
                currentHealth = 0;
                DeathScreen();
            }

            UI.SliderEdit(healthBar, currentHealth);
        }
    }

    public float GetCurrentHealth()
    {
        return currentHealth;
    }

    public bool DeathScreen()
    {
        deathCanvas.SetActive(true);
        player.gameObject.SetActive(false);
        isAlive = false;
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