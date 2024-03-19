using System;
using System.Collections.Generic;
using System.Linq;
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

    private int Hp;

    private States actualState;

    public void Start()
    {
        Debug.Log("START!");
        actualState = States.IDLE;
        Hp = 100;

        //Vector3 vel = new Vector3(10, 0, 0);
        //gameObject.SetVelocity(vel);
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
        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        switch (actualState)
        {
            case States.IDLE:

                //Play Idle animation

                if (x > 0 || y > 0)
                {
                    actualState = States.RUNNING;
                }

                if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
                {
                    actualState = States.SHOOT;
                }

                    break;

            case States.RUNNING:

                //Play Run animation

                HandleMovement();

                break;
            case States.SHOOT:

                //Play Shoot animation

                Shoot();

                break;
        }
    }

    void HandleMovement()
    {
        //--------------------- KeyBoard Movement ---------------------//
        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = new Vector3(0, 0, 1) * movementSpeed;

            //Vector3 vel = new Vector3(0, 0, movementSpeed);
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = new Vector3(0, 0, 1) * -movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = new Vector3(1, 0, 0) * movementSpeed * Time.deltaTime;
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        {
            Vector3 vel = new Vector3(1, 0, 0) * -movementSpeed * Time.deltaTime;
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

    void Shoot()
    {
        Debug.Log("Shoot!");
        Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
        Vector3 rot = new Vector3(0, 1, 0);
        Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);
        InternalCalls.CreateBullet(pos, rot, scale);
        //Input.GameControllerRumbleCS(3,32,100);
    }

}
