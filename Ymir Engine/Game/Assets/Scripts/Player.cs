using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;

public class Player : YmirComponent
{
    enum STATE : int
    {
        NONE = -1,

        IDLE,
        MOVE,
        DASH,
        SHOOTING,
        SHOOT,
        DEAD,

        All_TYPES
    }
    enum INPUT : int
    {
        I_IDLE,
        I_MOVE,
        I_DASH,
        I_DASH_END,
        I_SHOOTING,
        I_SHOOTING_END,
        I_SHOOT,
        I_SHOOT_END,
        I_DEAD
    }

    //--------------------- State ---------------------\\
    private STATE currentState = STATE.NONE;   //NEVER SET THIS VARIABLE DIRECTLLY, ALLWAYS USE INPUTS
    private List<INPUT> inputsList = new List<INPUT>();

    //--------------------- Movement ---------------------\\
    //public float rotationSpeed = 2.0f;
    public float movementSpeed = 35.0f;
    private double angle = 0.0f;
    private int deathZone = 15000;

    //--------------------- Dash ---------------------\\
    public float dashforce = 1000f;
    private float dashTimer = 0.0f;

    //--------------------- Controller var ---------------------\\
    float x = 0;
    float y = 0;
    Vector3 gamepadInput;

    //--------------------- Shoot var ---------------------\\
    public float fireRate = 0.2f;
    private float shootingTimer = 0.0f;
    public float secondaryRate = 0.2f;


    //Old Things

    //public float movementSpeed = 5f;

    //private double angle = 0.0f;

    //private int Hp;

    public void Start()
    {
        Debug.Log("START!");
        currentState = STATE.IDLE;
        //Hp = 100;
    }

    public void Update()
    {
        // New Things WIP
        //ProcessInternalInput();
        //ProcessExternalInput();
        //ProcessState();

        //UpdateState();






        //Old things
        UpdateControllerInputs();

        //HandleStates();

        if (gamepadInput.magnitude > 0)
        {
            HandleMovement();
        }
        else
        {
            StopPlayer();
        }
        //Debug.Log(gameObject.transform.GetRight());
    }

    private void ProcessInternalInput()
    {
        if (dashTimer > 0)
        {
            dashTimer -= Time.deltaTime;

            if (dashTimer <= 0)
                inputsList.Add(INPUT.I_DASH_END);
        }

        if (shootingTimer > 0)
        {
            shootingTimer -= Time.deltaTime;

            if (shootingTimer <= 0)
            {
                inputsList.Add(INPUT.I_SHOOT);
                Debug.Log("In shoot");
            }
        }
    }
    private void ProcessExternalInput()
    {
        //----------------- Joystic -----------------\\
        if (JoystickMoving() == true)
        {
            inputsList.Add(INPUT.I_MOVE);
        }
        else if (currentState == STATE.MOVE && JoystickMoving() == false)
        {
            inputsList.Add(INPUT.I_IDLE);
        }

        //----------------- Shoot -----------------\\
        if (Input.GetGamepadRightTrigger() > 0)
        {
            inputsList.Add(INPUT.I_SHOOTING);
        }
        else
        {
            inputsList.Add(INPUT.I_SHOOTING_END);
        }

        //----------------- Dash -----------------\\
        if (Input.GetGamepadButton(GamePadButton.B) == KeyState.KEY_DOWN)
        {
            inputsList.Add(INPUT.I_DASH);
        }
    }
    private void ProcessState()
    {
        //while (inputsList.Count > 0)
        //{
        //    INPUT input = inputsList[0];

        //    switch (currentState)
        //    {
        //        case STATE.NONE:
        //            Debug.Log("ERROR STATE");
        //            break;

        //        case STATE.IDLE:
        //            switch (input)
        //            {
        //                case INPUT.I_MOVE:
        //                    currentState = STATE.MOVE;
        //                    HandleMovement();
        //                    break;

        //                case INPUT.I_DASH:
        //                    currentState = STATE.DASH;
        //                    Dash();
        //                    break;

        //                case INPUT.I_SHOOTING:
        //                    currentState = STATE.SHOOTING;
        //                    Shoot();
        //                    break;

        //                case INPUT.I_DEAD:
        //                    break;
        //            }
        //            break;


        //        case STATE.MOVE:
        //            switch (input)
        //            {
        //                case INPUT.I_IDLE:
        //                    currentState = STATE.IDLE;
        //                    StartIdle();
        //                    break;

        //                case INPUT.I_DASH:
        //                    currentState = STATE.DASH;
        //                    Dash();
        //                    break;

        //                case INPUT.I_SHOOTING:
        //                    currentState = STATE.SHOOTING;
        //                    Shoot();
        //                    break;

        //                case INPUT.I_DEAD:
        //                    break;
        //            }
        //            break;


        //        case STATE.DASH:
        //            switch (input)
        //            {
        //                case INPUT.I_DASH_END:
        //                    currentState = STATE.IDLE;
        //                    EndDash();
        //                    break;

        //                case INPUT.I_DEAD:
        //                    break;
        //            }
        //            break;


        //        case STATE.SHOOTING:
        //            switch (input)
        //            {
        //                case INPUT.I_DASH:
        //                    currentState = STATE.DASH;
        //                    StartDash();
        //                    break;

        //                case INPUT.I_SHOOTING_END:
        //                    currentState = STATE.IDLE;
        //                    EndShooting();
        //                    StartIdle();
        //                    break;

        //                case INPUT.I_SHOOT:
        //                    currentState = STATE.SHOOT;
        //                    Shoot();
        //                    break;

        //                case INPUT.I_DEAD:
        //                    break;
        //            }
        //            break;

        //        case STATE.SHOOT:
        //            switch (input)
        //            {
        //                case INPUT.IN_SHOOT_END:
        //                    currentState = STATE.SHOOTING;
        //                    StartShooting();
        //                    break;

        //                case INPUT.IN_DEAD:
        //                    break;
        //            }
        //            break;

        //        case STATE.SECONDARY_SHOOT:
        //            break;

        //        default:
        //            Debug.Log("NEED TO ADD STATE TO CORE SWITCH");
        //            break;
        //    }
        //    inputsList.RemoveAt(0);
        //}
    }
    private void UpdateState()
    {
        //switch (currentState)
        //{
        //    case STATE.NONE:
        //        break;
        //    case STATE.IDLE:
        //        break;
        //    case STATE.MOVE:
        //        HandleMovement();
        //        break;
        //    case STATE.DASH:
        //        UpdateDash();
        //        break;
        //    case STATE.SHOOTING:
        //        UpdateShooting();
        //        break;
        //    case STATE.SHOOT:
        //        break;
        //    case STATE.SECONDARY_SHOOT:
        //        break;
        //    case STATE.DEAD:
        //        break;
        //    default:
        //        Debug.Log("No State? :(");
        //        break;
        //}
    }
    private void Dash()
    {

    }
    private void UpdateControllerInputs()
    {
         x = Input.GetLeftAxisX();
         y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, -y, 0f);

        Debug.Log("sdsad"+x);
    }

    private bool JoystickMoving()
    {
        return gamepadInput.magnitude > 0;
    }

    public void OnCollisionEnter()
    {
        Debug.Log("OnCollisionEnter!!!!");
    }

    void HandleStates()
    {
        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        //switch (actualState)
        //{
        //    case States.IDLE:

        //        //Play Idle animation

        //        if (x > 0 || y > 0)
        //        {
        //            actualState = States.RUNNING;
        //        }

        //        if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        //        {
        //            actualState = States.SHOOT;
        //        }

        //            break;

        //    case States.RUNNING:

        //        //Play Run animation

        //        HandleMovement();

        //        break;
        //    case States.SHOOT:

        //        //Play Shoot animation

        //        Shoot();

        //        break;
        //}
    }

    void HandleMovement()
    {
        //--------------------- KeyBoard Movement ---------------------//
        //if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetForward() * movementSpeed);
        //}

        //if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);
        //}

        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetRight() * movementSpeed);
        //}

        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetRight() * -movementSpeed);
        //}


        ////--------------------- Controller Movement ---------------------//

        //if (gamepadInput.x > 0)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetRight() * -movementSpeed);
        //}
        //if (gamepadInput.x < 0)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetRight() * movementSpeed);
        //}
        //if (gamepadInput.y > 0)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetForward() * movementSpeed);
        //}
        //if (gamepadInput.y < 0)
        //{
        //    gameObject.SetVelocity(gameObject.transform.GetForward() * -movementSpeed);
        //}



        HandleRotation();
        //Debug.Log("Vel:"+gameObject.GetForward() * movementSpeed);
        gameObject.SetVelocity(gameObject.transform.GetForward() * movementSpeed);
    }

    //Susu goes brrrr
    private void StopPlayer()
    {
        //Debug.Log("Stoping");
        gameObject.SetVelocity(new Vector3(0, 0, 0));
    }

    private void HandleRotation()
    {
        //Debug.Log("Hola");
        //Calculate player rotation
        Vector3 aX = new Vector3(gamepadInput.x, 0, -gamepadInput.y);
        Vector3 aY = new Vector3(0, 0, 1);
        //Debug.Log(gamepadInput.x);
        //Debug.Log(gamepadInput.y);
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

        //Debug.Log(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).x + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).y + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).z + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).w);

        gameObject.SetRotation(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle));
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
