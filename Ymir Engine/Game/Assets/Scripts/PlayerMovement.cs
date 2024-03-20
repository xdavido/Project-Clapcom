using System;
using System.Collections.Generic;
using YmirEngine;

public class PlayerMovement : YmirComponent
{
    Vector3 gamepadInput;

    public GameObject thisReference = null;

    public float movementSpeed = 5f;
    public Vector3 direccion = new Vector3(0, 1, 0);

    private double angle = 0.0f;
    //private bool script = true;
    public void Start()
    {

        Debug.Log("START!");
        //Vector3 vel = new Vector3(10, 0, 0);
        //gameObject.SetVelocity(vel);


    }
    public void Update()
    {
        thisReference = gameObject;

        //-------------- Keyboard --------------//

        //if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;

        //if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        //    gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * movementSpeed * Time.deltaTime;
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * -movementSpeed * Time.deltaTime;

        if (Input.GetKey(YmirKeyCode.E) == KeyState.KEY_DOWN)
        {
            Debug.Log("Shoot!");

            //Posicion desde la que se crea la bala (la misma que el game object que le dispara)
            //Se le añade un offset en Z para que no colisione el player con la bala
            Vector3 pos = new Vector3(gameObject.transform.globalPosition.x, gameObject.transform.globalPosition.y, gameObject.transform.globalPosition.z + 2);

            //Rotacion desde la que se crea la bala (la misma que el game object que le dispara)
            Vector3 rot = new Vector3(0, 10, 0);

            //Tamaño de la bala
            Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);

            //Crea la bala
            InternalCalls.CreateBullet(pos, rot, scale);
        }

        if (Input.GetKey(YmirKeyCode.SPACE) == KeyState.KEY_DOWN)
        {
            Vector3 vel = new Vector3(0, 0, 10);
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.W) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(0, 0, 10);
            gameObject.SetImpulse(vel);
        }
        if (Input.GetKey(YmirKeyCode.S) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(0, 0, -10);
            gameObject.SetImpulse(vel);
        }

        if (Input.GetKey(YmirKeyCode.A) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(-10, 0, 0);
            gameObject.SetImpulse(vel);
        }
        if (Input.GetKey(YmirKeyCode.D) == KeyState.KEY_REPEAT)
        {

            Vector3 vel = new Vector3(10, 0, 0);
            gameObject.SetImpulse(vel);
        }

        //-------------- Controller --------------//

        float x = Input.GetLeftAxisX();
        float y = Input.GetLeftAxisY();

        gamepadInput = new Vector3(x, y, 0f);

        //if (gamepadInput.x > 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetRight() * -movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.x < 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetRight() * movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.y > 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetForward() * -movementSpeed * Time.deltaTime;
        //}
        //if (gamepadInput.y < 0)
        //{
        //    gameObject.transform.localPosition += gameObject.GetForward() * movementSpeed * Time.deltaTime;
        //}

        if (gamepadInput.x > 0)
        {
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.x < 0)
        {
            gameObject.transform.localPosition += new Vector3(1, 0, 0) * movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y > 0)
        {
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * -movementSpeed * Time.deltaTime;
        }
        if (gamepadInput.y < 0)
        {
            gameObject.transform.localPosition += new Vector3(0, 0, 1) * movementSpeed * Time.deltaTime;
        }

        if (Input.GetGamepadButton(GamePadButton.A) == KeyState.KEY_DOWN)
        {
            Debug.Log("Shoot!");
            Vector3 pos = new Vector3(gameObject.transform.localPosition.x, 0, gameObject.transform.localPosition.z);
            Vector3 rot = new Vector3(0, 1, 0);
            Vector3 scale = new Vector3(0.2f, 0.2f, 0.2f);
            InternalCalls.CreateBullet(pos, rot, scale);
            //Input.GameControllerRumbleCS(3,32,100);
        }

        if (Input.GetGamepadButton(GamePadButton.B) == KeyState.KEY_DOWN)
        {
            Debug.Log("Pause!");
            Audio.PauseAllAudios();
        }
        if (Input.GetGamepadButton(GamePadButton.X) == KeyState.KEY_DOWN)
        {
            Debug.Log("Play!");
            Audio.ResumeAllAudios();
        }


        //Prueba para acceder a otro script funcional
        //if (script)
        //{
        //    GetAnotherScript();
        //    script = false;
        //}



        //En un futuro proximo para hace bien el movimiento
        //RotatePlayer();
    }

    public void OnCollisionEnter()
    {
        Debug.Log("OnCollisionEnter!!!!");
    }

    private void GetAnotherScript()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        if (gameObject != null)
        {
            PlayerMovement player = gameObject.GetComponent<PlayerMovement>();

            player.movementSpeed = 10f;

            Debug.Log("MovmentSpeed= " + movementSpeed);

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
}