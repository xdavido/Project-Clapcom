using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;

enum States
{
    IDLE,
    RUNNING,
    SHOOT,
    HIT,
    DEAD,

    All_TYPES
};

public class Player : YmirComponent
{
    Vector3 gamepadInput;

    public float movementSpeed = 5f;

   // private double angle = 0.0f;

    private States actualState;

    public void Start()
    {
        Debug.Log("START!");
        actualState = States.IDLE;
    }

    public void Update()
    {
        //HandleStates();

        HandleMovement();
    }

    public void OnCollisionEnter()
    {
        Debug.Log("OnCollisionEnter!!!!");
    }

    void HandleStates()
    {
        switch (actualState)
        {
            case States.IDLE:
                //Play Idle animation
                break;
            case States.RUNNING:

                break;
        }
    }

    void HandleMovement()
    {
        //--------------------- KeyBoard Movement ---------------------//
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = gameObject.transform.GetForward() * movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = gameObject.transform.GetForward() * -movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = gameObject.transform.GetRight() * movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = gameObject.transform.GetRight() * -movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }


        //--------------------- Controller Movement ---------------------//

        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, y, 0f);

        if (gamepadInput.x > 0)
        {
            Vector3 vel = gameObject.transform.GetRight() * movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }
        if (gamepadInput.x < 0)
        {
            Vector3 vel = gameObject.transform.GetRight() * -movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }
        if (gamepadInput.y > 0)
        {
            Vector3 vel = gameObject.transform.GetForward() * -movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }
        if (gamepadInput.y < 0)
        {
            Vector3 vel = gameObject.transform.GetForward() * movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }
    }

}
