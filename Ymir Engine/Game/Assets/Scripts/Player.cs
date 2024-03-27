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
        JUMP,

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
        I_DEAD,
        I_JUMP,
        I_JUMP_END,
    }

    //--------------------- State ---------------------\\
    private STATE currentState = STATE.NONE;   //NEVER SET THIS VARIABLE DIRECTLLY, ALLWAYS USE INPUTS
    private List<INPUT> inputsList = new List<INPUT>();

    //--------------------- Movement ---------------------\\
    //public float rotationSpeed = 2.0f;
    public float movementSpeed = 35.0f;
    //private double angle = 0.0f;
    private float deathZone = 0.5f;

    //--------------------- Dash ---------------------\\
    //public float dashforce = 1000.0f;
    private float dashTimer = 0.0f;
    private float jumpTimer = 0.0f;

    //private float timeSinceLastDash = 0.0f;
    public float dashCD = 0.1f;
    public float dashDuration = 1.0f;
    public float dashDistance = 100000.0f;
    private float dashSpeed = 0.0f;
    //private float dashStartYPos = 0.0f;

    //--------------------- Controller var ---------------------\\
    float x = 0;
    float y = 0;
    Vector3 gamepadInput;
    //bool isMoving = false;

    //--------------------- Shoot var ---------------------\\
    public float fireRate = 0.2f;
    private float shootingTimer = 0.0f;
    public float secondaryRate = 0.2f;

    private bool isReloading= false;
    private float reloadTimer = 0.0f;
    private float reloadCD = 1.0f;

    public int ammo = 0;
    public int magsize = 5;

    //--------------------- External GameObjects ---------------------\\
    private GameObject cameraObject;

    //--------------------- External Scripts ---------------------\\
    private UI_Bullets csBullets;
    private Health csBHealth;

    //--------------------- GOD mode ---------------------\\
    public bool godMode = false;

    public void Start()
    {
        //--------------------- Dash ---------------------\\
        dashTimer = 0f;
        jumpTimer = 0.0f;
        dashSpeed = dashDistance / dashDuration;

        //--------------------- Shoot ---------------------\\
        ammo = magsize;
        reloadTimer = reloadCD;

        //--------------------- Get Player Scripts ---------------------\\
        GetPlayerScripts();

        //--------------------- Get Camera GameObject ---------------------\\
        cameraObject = InternalCalls.GetGameObjectByName("Main Camera");

        currentState = STATE.IDLE;


        Debug.Log("START!");
    }

    public void Update()
    {
        // New Things WIP
        UpdateControllerInputs();

        ProcessInternalInput();
        ProcessExternalInput();
        ProcessState();

        UpdateState();

        if (Input.GetKey(YmirKeyCode.F1) == KeyState.KEY_DOWN)
        {
            godMode = !godMode;
        }

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
            {  
                inputsList.Add(INPUT.I_DASH_END);
            }
        }

        if (jumpTimer > 0)
        {
            jumpTimer -= Time.deltaTime;

            if (jumpTimer <= 0)
            {
                inputsList.Add(INPUT.I_JUMP_END);
            }
        }

        if (shootingTimer > 0)
        {
            shootingTimer -= Time.deltaTime;

            if (shootingTimer <= 0)
            {
                inputsList.Add(INPUT.I_SHOOT);
                //Debug.Log("In shoot");
            }
        }
        if (isReloading)
        {
            if (reloadTimer > 0)
            {
                reloadTimer -= Time.deltaTime;

                if (reloadTimer <= 0)
                {
                    ammo = magsize; 
                    csBullets.UseBullets();
                    isReloading = false;    
                }
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
        if (Input.GetGamepadRightTrigger() > 0 && !isReloading && ammo > 0)
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
            Input.Rumble_Controller(50);
        }

        //----------------- Reload -----------------\\
        if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        {
            Audio.PlayAudio(gameObject, "W_FirearmReload");
            isReloading = true;
            reloadTimer = reloadCD;
        }

        //----------------- Desbugear -----------------\\
        //if (Input.GetGamepadButton(GamePadButton.Y) == KeyState.KEY_DOWN)
        //{
        //    //Debug.Log("aaaa");
        //    inputsList.Add(INPUT.I_JUMP);
        //    Input.Rumble_Controller(50);
        //}
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
                    //Debug.Log("IDLE");
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

                        case INPUT.I_JUMP:
                            currentState = STATE.JUMP;
                            StartJump();
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
                    //Debug.Log("MOVE");
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

                        case INPUT.I_JUMP:
                            currentState = STATE.JUMP;
                            StartJump();
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
                    //Debug.Log("DASH");
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

                case STATE.JUMP:
                    //Debug.Log("JUMP");
                    switch (input)
                    {
                        case INPUT.I_JUMP_END:
                            currentState = STATE.IDLE;
                            EndJump();
                            break;

                        case INPUT.I_DEAD:
                            break;
                    }
                    break;


                case STATE.SHOOTING:
                    //Debug.Log("SHOOTING");
                    switch (input)
                    {
                        case INPUT.I_DASH:
                            currentState = STATE.DASH;
                            StartDash();
                            break;

                        case INPUT.I_JUMP:
                            currentState = STATE.JUMP;
                            StartJump();
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
                    //Debug.Log("SHOOT");
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
            case STATE.JUMP:
                UpdateJump();
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
        // Añadir efecto de sonido
        Audio.PlayAudio(gameObject,"P_Shoot");
        Input.Rumble_Controller(100);
        //Debug.Log("Shoot!");

        if (!godMode)
        {
            --ammo;
            csBullets.UseBullets();
        }

        //Debug.Log("Ammo:" + ammo);

        StopPlayer();

        Vector3 offset = new Vector3(0, 15, 0);
        //Posicion desde la que se crea la bala (la misma que el game object que le dispara)
        Vector3 pos = gameObject.transform.globalPosition + offset + (gameObject.transform.GetForward() * 2);

        //Debug.Log("ParentPos: " + gameObject.transform.globalPosition.x + gameObject.transform.globalPosition.y + gameObject.transform.globalPosition.z);
        //Debug.Log("Spawn pos: " + pos);

        //Rotacion desde la que se crea la bala (la misma que el game object que le dispara)
        Quaternion rot = gameObject.transform.globalRotation;

        //Tamaño de la bala
        Vector3 scale = new Vector3(2.0f, 2.0f, 4.0f);

        //Crea la bala
        //Debug.Log("rot: " + gameObject.transform.localRotation.x + gameObject.transform.localRotation.y + gameObject.transform.localRotation.z + gameObject.transform.localRotation.w);
        InternalCalls.CreateBullet(pos, rot, scale);

        inputsList.Add(INPUT.I_SHOOT_END);
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
        //Animation.PlayAnimation(gameObject, "Lift2");
        Audio.PlayAudio(gameObject, "P_Dash");
        StopPlayer();
        dashTimer = dashDuration;
        //dashStartYPos = gameObject.transform.localPosition.y;
    }
    private void UpdateDash()
    {
        gameObject.SetImpulse(gameObject.transform.GetForward() * dashSpeed);
    }
    private void EndDash()
    {
        StopPlayer();
        //gameObject.transform.localPosition.y = dashStartYPos;
    }

    private void StartJump()
    {
        //Animation.PlayAnimation(gameObject, "Random");
        //Animation.SetLoop(gameObject, "Random", true);
        jumpTimer = dashDuration;
    }
    private void UpdateJump()
    {
        gameObject.SetImpulse(new Vector3(0,1,0) * dashSpeed);
    }
    private void EndJump()
    {
        StopPlayer();
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

        gamepadInput = new Vector3(x, y, 0f);

        //Debug.Log("sdsad" + x);
    }
    #endregion

    #region COLLISION
    public void OnCollisionEnter(GameObject other)
    {
        //Debug.Log("Peedrito");
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
        Vector3 velocity = gameObject.transform.GetForward() * movementSpeed;
        Vector3 gravity = new Vector3(0.0f, -10.0f, 0.0f);

        velocity += gravity;
        gameObject.SetVelocity(velocity);

        //gameObject.SetVelocity(gameObject.transform.GetForward() * movementSpeed);

        HandleRotation();
    }
    private void StopPlayer()
    {
        Debug.Log("Stopping");
        gameObject.SetVelocity(new Vector3(0, 0, 0));
    }
    
    private void HandleRotation()
    {
        Vector3 aX = new Vector3(gamepadInput.x, 0, gamepadInput.y);
        aX = Vector3.Normalize(aX);
        
        Quaternion targetRotation = Quaternion.identity;

        Vector3 aY = gameObject.transform.GetUp();

        if (aX != Vector3.zero)
        {
            float angle = 0;

            if (aX.x >= 0)
            {
                angle = (float)Math.Acos(Vector3.Dot(aX, aY) - 1);
            }
            else if (aX.x < 0)
            {
                angle = -(float)Math.Acos(Vector3.Dot(aX, aY) - 1);
            }

            // Add 45 degrees to the calculated angle
            angle -= Mathf.PI / 4f; // Rotate 45 degrees to the right

            // Construct quaternion rotation with an inverted axis for correcting left and right inversion
            targetRotation = Quaternion.AngleAxis(angle * Mathf.Rad2Deg, Vector3.up);
        }

        // Apply rotation
        gameObject.SetRotation(targetRotation);

        //Calculate player rotation
        //Vector3 aX = new Vector3(-gamepadInput.x, 0, -gamepadInput.y - 1);
        //Vector3 aY = new Vector3(0, 0, 1);
        //aX = Vector3.Normalize(aX);

        //if (aX.x >= 0)
        //{
        //    angle = Math.Acos(Vector3.Dot(aX, aY) - 1);
        //}
        //else if (aX.x < 0)
        //{
        //    angle = -Math.Acos(Vector3.Dot(aX, aY) - 1);
        //}

        ////Convert angle from world view to orthogonal view
        //angle += 0.785398f; //Rotate 45 degrees to the right

        ////Debug.Log(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).x + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).y + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).z + Quaternion.RotateAroundAxis(Vector3.up, (float)-angle).w);

        //gameObject.SetRotation(Quaternion.RotateAroundAxis(Vector3.up, (float)-angle));
    }

    // TODO: use the generic functions
    private void GetPlayerScripts()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        if (gameObject != null)
        {
            csBullets = gameObject.GetComponent<UI_Bullets>();
            csBHealth = gameObject.GetComponent<Health>();
        }
    }

    #endregion
}
