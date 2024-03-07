using System;
using System.Collections.Generic;
using YmirEngine;

public class PlayerMovement : YmirComponent
{
    //public static PlayerMovement instance;

    public GameObject thisReference = null;

    Vector3 gamepadInput;

    public float movementSpeed = 20f;
    private double angle = 0.0f;

    enum STATE : int
    {
        NONE = -1,
        IDLE,
        MOVE,
        DASH,
        SHOOT
    }
    enum INPUT : int
    {
        IN_IDLE,
        IN_MOVE,
        IN_DASH,
        IN_DASH_END,
        IN_SHOOTING,
        IN_SHOOTING_END,
        IN_SHOOT,
        IN_SHOOT_END,
        IN_SEC_SHOOT,
        IN_DEAD
    }

    private STATE currentState = STATE.NONE;
    private List<INPUT> inputList = new List<INPUT>();

    public void Update()
    {
        thisReference = gameObject;

        GameMovement();
        UpdateState();
    }

    private void GameMovement()
    {

        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        //Debug.Log("[WARNING] PosicionX: " + x + "PosicionY: " + y);

        if (x < 0)
        {
            gameObject.transform.localPosition.x += -20f * x * Time.deltaTime;
        }
        else if (x > 0)
        {
            gameObject.transform.localPosition.x += 20f * x * Time.deltaTime;
        }

        if (y < 0)
        {
            gameObject.transform.localPosition.y += -20f * y * Time.deltaTime;
        }
        else if (y > 0)
        {
            gameObject.transform.localPosition.y += 20f * y * Time.deltaTime;
        }

        //gameObject.transform.localPosition.x += 20f * x * Time.deltaTime; 
        //gameObject.transform.localPosition.y += 20f * y * Time.deltaTime;


        gamepadInput = new Vector3(x, y, 0f);

        if (IsMoving())
            inputList.Add(INPUT.IN_MOVE);
        else if (currentState == STATE.MOVE && !IsMoving())
            inputList.Add(INPUT.IN_IDLE);

        if (Input.GetGamepadRightTrigger() > 0)
            inputList.Add(INPUT.IN_SHOOT);

        while (inputList.Count > 0)
        {
            INPUT input = inputList[0];
            switch (currentState)
            {
                case STATE.NONE:

                    break;

                case STATE.IDLE:
                    switch (input)
                    {
                        case INPUT.IN_MOVE:
                            currentState = STATE.MOVE;

                            Move();
                            break;

                        case INPUT.IN_DASH:
                            currentState = STATE.DASH;
                            Dash();
                            break;

                        case INPUT.IN_SHOOTING:
                            currentState = STATE.SHOOT;
                            Shoot();
                            break;

                    }
                    break;
                case STATE.MOVE:
                    switch (input)
                    {
                        case INPUT.IN_IDLE:
                            currentState = STATE.IDLE;
                            break;

                        case INPUT.IN_DASH:
                            currentState = STATE.DASH;
                            Dash();
                            break;

                        case INPUT.IN_SHOOTING:
                            currentState = STATE.SHOOT;
                            Shoot();
                            break;

                        case INPUT.IN_DEAD:
                            break;
                    }
                    break;
                case STATE.SHOOT:
                    switch (input)
                    {
                        case INPUT.IN_SHOOT_END:
                            currentState = STATE.SHOOT;
                            Shoot();
                            break;

                        case INPUT.IN_DEAD:
                            break;
                    }
                    break;

                case STATE.DASH:
                    switch (input)
                    {
                        case INPUT.IN_DASH_END:
                            currentState = STATE.IDLE;
                            break;
                    }
                    break;
            }
            inputList.RemoveAt(0);

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
                RotatePlayer();
                gameObject.transform.localPosition += thisReference.GetForward() * movementSpeed * Time.deltaTime;
                Debug.Log("[ERROR]Mover");
                break;
            case STATE.DASH:

                break;
            case STATE.SHOOT:

                break;
            default:
                Debug.Log("NEED TO ADD STATE TO CORE");
                break;
        }
    }

    private void RotatePlayer()
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

    private void Move()
    {
        // Play Audio and SetAnimation;
    }
    private void Dash()
    {
        // Play Audio and SetAnimation;
    }
    private void Shoot()
    {
        // Play Audio and SetAnimation;
    }

    private bool IsMoving()
    {

        return Math.Abs(gamepadInput.x) > 0 || Math.Abs(gamepadInput.y) > 0;
    }

}