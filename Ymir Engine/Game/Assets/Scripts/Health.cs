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

    public float currentHealth = 0;
    public float maxHealth = 3;

    public float debugDmg = 1;

    public void Start()
    {
        Debug.Log("HelloWorld");

        healthBar = InternalCalls.GetGameObjectByName("Health Bar");

        UI.SliderEdit(healthBar, maxHealth);
        currentHealth = maxHealth;
    }

    public void Update()
    {
        if (Input.GetKey(YmirKeyCode.Q) == KeyState.KEY_DOWN)
        {
            Debug.Log("Take debug dmg");

            TakeDmg(debugDmg);
        }

        return;
    }

    public bool TakeDmg(float dmg)
    {
        if (/*godmode*/ true)
        {
            currentHealth -= dmg;

            int paco = (int)currentHealth;

            Debug.Log(paco);

            UI.SliderEdit(healthBar, currentHealth);

            if (currentHealth <= 0)
            {
                return true;
            }
        }

        return false;
    }
    private void GetAnotherScript()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
       
        if (gameObject != null)
        {
            PlayerMovement player = gameObject.GetComponent<PlayerMovement>();

            //Debug.Log("MovmentSpeed= " + movementSpeed);

        }
    }
}