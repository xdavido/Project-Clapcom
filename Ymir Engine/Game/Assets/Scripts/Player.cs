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
    private float deathZone = 0.3f;

    //--------------------- Dash ---------------------\\
    public float dashforce = 1000.0f;
    private float dashTimer = 0.0f;

    //private float timeSinceLastDash = 0.0f;
    public float dashCD = 0.33f;
    public float dashDuration = 0.25f;
    public float dashDistance = 1.0f;
    private float dashSpeed = 0.0f;
    private float dashStartYPos = 0.0f;

    //--------------------- Controller var ---------------------\\
    float x = 0;
    float y = 0;
    Vector3 gamepadInput;
    //bool isMoving = false;

    //--------------------- Shoot var ---------------------\\
    public float fireRate = 0.2f;
    private float shootingTimer = 0.0f;
    public float secondaryRate = 0.2f;

    //--------------------- Fake Start ---------------------\\
    private bool scriptStart = true;

    //Vector3 up = new Vector3(0, 1, 0);

    //Old Things

    //public float movementSpeed = 5f;

    //private double angle = 0.0f;

    //private int Hp;

    public void Start()
    {
        //Hp = 100;

        //--------------------- Dash ---------------------\\
        dashTimer = 0f;
        dashSpeed = dashDistance / dashDuration;


        currentState = STATE.IDLE;
        Debug.Log("START!");
    }

    private void StartFake()
    {
        //Hp = 100;

        //--------------------- Dash ---------------------\\
        dashTimer = 0f;
        dashSpeed = dashDistance / dashDuration;


        currentState = STATE.IDLE;
        Debug.Log("START!");
    }

    public void Update()
    {
        if (scriptStart == true)
        {
            StartFake();
            scriptStart = false;
        }

        // New Things WIP
        UpdateControllerInputs();

        ProcessInternalInput();
        ProcessExternalInput();
        ProcessState();

        UpdateState();






        //Old things
        //UpdateControllerInputs();

        //if (gamepadInput.magnitude > 0)
        //{
        //    isMoving = true;
        //    HandleMovement();
        //}
        //else if (isMoving)
        //{
        //    isMoving = false;
        //    StopPlayer();
        //}

        //if (Input.GetGamepadRightTrigger() > 0)
        //{
        //    StartShooting();
        //    Debug.Log("Shoot");
        //    inputsList.Add(INPUT.I_SHOOTING);
        //}
        //else
        //{
        //    inputsList.Add(INPUT.I_SHOOTING_END);
        //}
        //Debug.Log(gameObject.transform.GetRight());
    }

    #region FSM
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
            StopPlayer();
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
        while (inputsList.Count > 0)
        {
            INPUT input = inputsList[0];

            switch (currentState)
            {
                case STATE.NONE:
                    Debug.Log("ERROR STATE");
                    break;

                case STATE.IDLE:
                    switch (input)
                    {
                        case INPUT.I_MOVE:
                            currentState = STATE.MOVE;
                            StartMove();
                            break;

                        case INPUT.I_DASH:
                            currentState = STATE.DASH;
                            StartDash();
                            break;

                        case INPUT.I_SHOOTING:
                            currentState = STATE.SHOOTING;
                            StartShooting();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;


                case STATE.MOVE:
                    switch (input)
                    {
                        case INPUT.I_IDLE:
                            currentState = STATE.IDLE;
                            //StartIdle(); //Trigger de la animacion //Arreglar esto
                            break;

                        case INPUT.I_DASH:
                            currentState = STATE.DASH;
                            StartDash();
                            break;

                        case INPUT.I_SHOOTING:
                            currentState = STATE.SHOOTING;
                            StartShooting();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;


                case STATE.DASH:
                    switch (input)
                    {
                        case INPUT.I_DASH_END:
                            currentState = STATE.IDLE;
                            EndDash();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;


                case STATE.SHOOTING:
                    switch (input)
                    {
                        case INPUT.I_DASH:
                            currentState = STATE.DASH;
                            StartDash();
                            break;

                        case INPUT.I_SHOOTING_END:
                            currentState = STATE.IDLE;
                            EndShooting();
                            //StartIdle(); //Trigger de la animacion
                            break;

                        case INPUT.I_SHOOT:
                            currentState = STATE.SHOOT;
                            StartShoot();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;

                case STATE.SHOOT:
                    switch (input)
                    {
                        case INPUT.I_SHOOT_END:
                            currentState = STATE.SHOOTING;
                            StartShooting();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;

                default:
                    Debug.Log("No State? :(");
                    break;
            }
            inputsList.RemoveAt(0);
        }
    }
    private void UpdateState()
    {
        switch (currentState)
        {
            case STATE.NONE:
                break;
            case STATE.IDLE:
                break;
            case STATE.MOVE:
                UpdateMove();
                break;
            case STATE.DASH:
                UpdateDash();
                break;
            case STATE.SHOOTING:
                UpdateShooting();
                break;
            case STATE.SHOOT:
                break;
            case STATE.DEAD:
                break;
            default:
                Debug.Log("No State? :(");
                break;
        }
    }
    #endregion

    #region SHOOT

    private void StartShoot()
    {
        // Trigger animacion disparar
        // Futuro autoapuntado
    }
    private void StartShooting()
    {
        // Añadir coidgo de instanciar el prefab de disparo
        // Añadir efecto de sonido

        Debug.Log("Shoot!");

        //Posicion desde la que se crea la bala (la misma que el game object que le dispara)
        Vector3 pos = gameObject.transform.globalPosition + (gameObject.transform.GetForward() * 2);
        Debug.Log("ParentPos: " + gameObject.transform.globalPosition.x + gameObject.transform.globalPosition.y + gameObject.transform.globalPosition.z);
        Debug.Log("Spawn pos: " + pos);

        //Rotacion desde la que se crea la bala (la misma que el game object que le dispara)
        Quaternion rot = gameObject.transform.globalRotation;

        //Tamaño de la bala
        Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);

        //Crea la bala
        Debug.Log("rot: " + gameObject.transform.localRotation.x + gameObject.transform.localRotation.y + gameObject.transform.localRotation.z + gameObject.transform.localRotation.w);
        InternalCalls.CreateBullet(pos, rot, scale);

        //inputsList.Add(INPUT.I_SHOOT_END);
    }
    private void UpdateShooting()
    {
        if (JoystickMoving() == true)
            HandleRotation();
    }

    private void EndShooting()
    {
        // Reset del futuro autoapuntado
    }
    #endregion

    #region DASH
    private void StartDash()
    {
        dashTimer = dashDuration;
        dashStartYPos = gameObject.transform.localPosition.y;
    }
    private void UpdateDash()
    {
        StopPlayer();
        //Debug.Log("Fuersa:" + gameObject.transform.GetForward() * dashforce);
        gameObject.SetImpulse(gameObject.transform.GetForward().normalized * dashforce); //Arreglar esto
    }
    private void EndDash()
    {
        StopPlayer();
        gameObject.transform.localPosition.y = dashStartYPos;
    }
    #endregion

    #region Joystick
    private bool JoystickMoving()
    {
        //Debug.Log("Magnitude:" + gamepadInput.magnitude);
        return gamepadInput.magnitude > deathZone;
    }

    private void UpdateControllerInputs()
    {
        x = Input.GetLeftAxisX();
        y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, -y, 0f);

        //Debug.Log("sdsad" + x);
    }
    #endregion

    #region COLLISION
    public void OnCollisionEnter(GameObject other)
    {
        //Debug.Log("OnCollisionEnter!!!!");
        //gameObject.SetVelocity(up * movementSpeed);
    }
    #endregion

    #region PLAYER

    private void StartMove()
    {
        //Trigger de la animacion
        //Trigger del SFX de caminar
    }
    private void UpdateMove()
    {
        HandleRotation();
        gameObject.SetVelocity(gameObject.transform.GetForward() * movementSpeed);
    }
    private void StopPlayer()
    {
        //Debug.Log("Stoping");
        gameObject.SetVelocity(new Vector3(0, 0, 0));
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
    private void HandleRotation()
    {
        //Calculate player rotation
        Vector3 aX = new Vector3(gamepadInput.x, 0, gamepadInput.y - 1);
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

        //Debug.Log(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).x + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).y + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).z + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).w);

        gameObject.SetRotation(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle));
    }
    #endregion
}
