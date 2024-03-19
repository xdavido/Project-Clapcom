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

    //public float movementSpeed = 5f;

    //private double angle = 0.0f;

    //private int Hp;

    private States actualState;

    private Vector3 aimVector = Vector3.forward;

    //Susu goes Brrr
    // Movement
    public float rotationSpeed = 2.0f;
    public float movementSpeed = 35.0f;
    public float mouseSens = 1.0f;
    private double angle = 0.0f;
    private int deathZone = 15000;


    public void Start()
    {
        Debug.Log("START!");
        actualState = States.IDLE;
        //Hp = 100;

        //Vector3 vel = new Vector3(10, 0, 0);
        //gameObject.SetVelocity(vel);
    }

    public void Update()
    {
        UpdateControllerInputs();

        //HandleStates();
        HandleMovement();
    }

    private void UpdateControllerInputs()
    {
        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, -y, 0f);
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
        ////--------------------- KeyBoard Movement ---------------------//
        //if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        //{
        //    Vector3 vel = gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}

        //if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        //{
        //    Vector3 vel = gameObject.GetForward() * -movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}

        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //{
        //    Vector3 vel = gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}

        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //{
        //    Vector3 vel = gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}


        ////--------------------- Controller Movement ---------------------//

        //float x = Input.GetLeftAxisX();
        //float y = Input.GetLeftAxisY();

        //gamepadInput = new Vector3(x, y, 0f);

        //if (gamepadInput.x > 0)
        //{
        //    Vector3 vel = gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //    //gameObject.SetImpulse(vel);
        //    gameObject.SetVelocity(vel);
        //}
        //if (gamepadInput.x < 0)
        //{
        //    Vector3 vel = gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}
        //if (gamepadInput.y > 0)
        //{
        //    Vector3 vel = gameObject.GetForward() * -movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}
        //if (gamepadInput.y < 0)
        //{
        //    Vector3 vel = gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //    gameObject.SetImpulse(vel);
        //}

        if (gamepadInput.magnitude > deathZone)
        {
            HandleRotation();
            gameObject.SetVelocity(gameObject.GetForward() * movementSpeed * Time.deltaTime);
        }
    }

    //Susu goes brrrr
    private void StopPlayer()
    {
        Debug.Log("Stoping");
        gameObject.SetVelocity(new Vector3(0, 0, 0));
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

    private void HandleRotation()
    {
        //Calculate player rotation
        Vector3 aX = new Vector3(gamepadInput.x, 0, -gamepadInput.y - 1);
        Vector3 aY = new Vector3(0, 0, 1);
        aX = Vector3.Normalize(aX);

        if (aX.x >= 0)
        {
            angle = Math.Acos(Vector3.Dot(aX, aY) - 1);
        }
        else if (aX.x < 0)
        {
            angle = -Math.Acos(Vector3.Dot(aX, aY) - 1);
        }

        //Convert angle from world view to orthogonal view
        angle += 0.785398f; //Rotate 45 degrees to the right

        gameObject.transform.localRotation = Quaternion.RotateAroundAxis(Vector3.up, (float)-angle);
    }

}
