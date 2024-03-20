using System;
using YmirEngine;

public class BH_Bullet : YmirComponent
{
    public float speed = 10.0f;
    public float maxLifeTime = 2.0f;
    public float currentLifeTime = 0.0f;
    public float yVel = 0.0f;

    private bool destroyed = false;
    public void Start()
    {
        
    }

    public void Update()
    {
        currentLifeTime += Time.deltaTime;

        Vector3 vSpeed = GetDirection() * speed;

        //Para mover la bala sin rigidbody
        //thisReference.transform.localPosition += thisReference.transform.GetForward() * (speed * Time.deltaTime);

        //Velocidad a la que se dispara la bala hacia delante
        //vSpeed *= speed;

        //Se añade la velocidad al game object
        gameObject.SetVelocity(vSpeed);

        ////Caida de bala (no hace falta si la bala tiene rigidbody)
        //yVel -= Time.deltaTime / 15.0f;
        //thisReference.transform.localPosition += (Vector3.up * yVel);

        //Se destruye
        if (currentLifeTime >= maxLifeTime && destroyed == false)
        {
            InternalCalls.Destroy(gameObject);
            destroyed = true;
        }
    }

    private Vector3 GetDirection()
    {
        GameObject gameObject = InternalCalls.GetGameObjectByName("Player");
        if (gameObject != null)
        {
            Player player = gameObject.GetComponent<Player>();
            return player.gameObject.transform.GetForward();
        }
        else return new Vector3(0, 0, 0);
    }
}


