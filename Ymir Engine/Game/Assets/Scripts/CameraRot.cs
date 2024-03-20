using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;

public class CameraRot : YmirComponent
{
    //--------------------- Controller var ---------------------\\
    float x = 0;
    float y = 0;
    Vector3 gamepadInput;

    private double angle = 0.0f;

    public void Start()
    {
        Debug.Log("START!");
    }

    public void Update()
    {
        UpdateControllerInputs();

        ////HandleStates();

        if (JoystickMoving())
        {
            HandleRotation();
        }
    }

    private bool JoystickMoving()
    {
        return gamepadInput.magnitude > 0;
    }

    private void UpdateControllerInputs()
    {
        x = Input.GetLeftAxisX();
        y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, -y, 0f);

        Debug.Log("sdsad" + x);
    }
    private void HandleRotation()
    {
        //Debug.Log("Hola");
        //Calculate player rotation
        Vector3 aX = new Vector3(gamepadInput.x, 0, gamepadInput.y - 1);
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
}

