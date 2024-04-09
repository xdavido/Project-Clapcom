using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using YmirEngine;


    public class PruebaPathFinding : YmirComponent
    {
    

    public GameObject player = null;
    public GameObject shootPoint = null;

    public float wanderSpeed = 3.5f;
    public float runningSpeed = 7.5f;
    public float range = 20.0f;
    public float damage = 20.0f;
    public float bulletSpeed = 10.0f;
    public float healthPoints = 60.0f;
    protected int shotTimes = 0;


    protected float timeBewteenShots = 0.5f;
    protected float timePassed = 0.0f;

    public float idleTime = 5.0f;
    protected Vector3 targetPosition = null;
    protected float stoppingDistance = 1.0f;
    public float wanderRange = 5.0f;
    public float runningRange = 15.0f;

    public float slerpSpeed = 1000.5f;
    //private float timeCount = 0.0f;
    public bool turretMode = false;



    protected PathFinding agent;


      public float movementSpeed = 20f;
        private float actualMovementSpeed;

        public float life = 100f;

        

        public float xSpeed = 0, ySpeed = 0;


        //public float wanderTimer = 5f;

        

        public bool PlayerDetected = false;

        public float DetectionRadius = 15f;

        //private EnemyState state = EnemyState.Idle;

        private WanderState wanderState;

        RandomPointGenerator pointGenerator;
        

        

        private float wanderTimer;
        public float wanderDuration = 5f;

        private float stopedTimer;
        public float stopedDuration = 1f;

        private float cumTimer;
        public float cumDuration = 2f;

        private float cumTimer2;
        public float cumDuration2 = 5f;

        public void Start()
        {
            pointGenerator = new RandomPointGenerator();
            wanderState = WanderState.REACHED;
            wanderDuration = 5f;
            wanderTimer = wanderDuration;
            wanderRange = 20f;
            movementSpeed = 10f;
            stopedDuration = 1f;
            DetectionRadius = 10f;
            
            cumDuration = 2f;
            cumDuration2 = 5f;

            cumTimer = cumDuration2;

            //targetPosition = CalculateNewPosition(wanderRange);
            shotTimes = 0;
       
            player = InternalCalls.GetGameObjectByName("Player");
            agent = gameObject.GetComponent<PathFinding>();
            Debug.Log("Position"+ gameObject.transform.localPosition);

        agent.CalculateRandomPath(gameObject.transform.localPosition, wanderRange);
        Debug.Log("gola2");

        

        }

        public void Update()
        {

        cumTimer2 -= Time.deltaTime;
        if (cumTimer2 <= 0)
        {
            switch (wanderState)
            {
                case WanderState.REACHED:

                    agent.CalculateRandomPath(gameObject.transform.localPosition, wanderRange);
                    wanderTimer = wanderDuration;
                    Debug.Log("[ERROR] Current State: REACHED");
                    wanderState = WanderState.GOING;

                    break;

                case WanderState.GOING:
                    agent.speed = 5f;
                    LookAt(agent.GetDestination());
                    
                    MoveToCalculatedPos(agent.speed);

                    //IsReached(gameObject.transform.globalPosition,agent.GetDestination());
                    break;


                case WanderState.CHASING:

                    //Debug.Log("[ERROR] Current State: CHASING");
                    agent.CalculatePath(gameObject.transform.globalPosition, player.transform.globalPosition);
                    LookAt(agent.GetDestination());
                    MoveToCalculatedPos(agent.speed);
                    Debug.Log("[ERROR] Current State: CHASING");
                    break;

                case WanderState.STOPED:
                    Debug.Log("[ERROR] Current State: STOPED");
                    ProcessStopped();
                    break;

                case WanderState.HIT:

                    Proccescumdown();

                    break;
            }


            if (player.transform.globalPosition.x - gameObject.transform.globalPosition.x < DetectionRadius && player.transform.globalPosition.z - gameObject.transform.globalPosition.z < DetectionRadius)
            {
                if (wanderState != WanderState.HIT)
                {
                    actualMovementSpeed = movementSpeed;
                    wanderState = WanderState.CHASING;
                }

            }

        }



    }


        private void Proccescumdown()
        {
            if (cumTimer > 0)
            {
                cumTimer -= Time.deltaTime;
                if (cumTimer <= 0)
                {
                    Debug.Log("[ERROR] Reached");
                    wanderState = WanderState.REACHED;
                }
            }
        }

        private void ProcessStopped()
        {
            if (stopedTimer > 0)
            {
                stopedTimer -= Time.deltaTime;
                if (stopedTimer <= 0)
                {
                    wanderState = WanderState.REACHED;
                }
            }
        }
        private void ProcessMovement()
        {
            if (wanderTimer > 0)
            {
                wanderTimer -= Time.deltaTime;
                if (wanderTimer <= 0)
                {

                    Debug.Log("[ERROR] AAAA");
                    actualMovementSpeed = 0;
                    stopedTimer = stopedDuration;
                    wanderState = WanderState.STOPED;

                }
            }
        }

        private void HandleRotation()
        {
            Vector3 aX = new Vector3(xSpeed, 0, ySpeed);
            aX = Vector3.Normalize(aX);

            //Debug.Log("[ERROR] Vector: " + aX);

            Quaternion targetRotation = Quaternion.identity;

            Vector3 aY = new Vector3(0, 1, 0);


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

                //Debug.Log("[ERROR] Angle: " + angle);

                targetRotation = Quaternion.AngleAxis(angle * Mathf.Rad2Deg, Vector3.up);

                //Debug.Log("[ERROR] Angle: " + targetRotation);
            }

            gameObject.SetRotation(targetRotation);

        }

        private void RotateEnemy()
        {
            Vector3 direction = player.transform.globalPosition - gameObject.transform.globalPosition;
            direction = direction.normalized;
            float angle = (float)Math.Atan2(direction.x, direction.z);

            //Debug.Log("Desired angle: " + (angle * Mathf.Rad2Deg).ToString());

            if (Math.Abs(angle * Mathf.Rad2Deg) < 1.0f)
                return;

            Quaternion dir = Quaternion.RotateAroundAxis(Vector3.up, angle);

            float rotationSpeed = Time.deltaTime * movementSpeed;
            //Debug.Log("CS: Rotation speed: " + rotationSpeed.ToString());
            //Debug.Log("CS: Time: " + Time.deltaTime);

            Quaternion desiredRotation = Quaternion.Slerp(gameObject.transform.localRotation, dir, rotationSpeed);

            gameObject.transform.localRotation = desiredRotation;

            //Debug.Log("[ERROR] rotation:  " + gameObject.transform.localRotation);
        }

        public void OnCollisionStay(GameObject other)
        {
            if (other.Name == "Player" && wanderState != WanderState.HIT)
            {

                Debug.Log("[ERROR] Name: " + other.Name);
                Debug.Log("[ERROR] HIT!!!");
                gameObject.SetVelocity(gameObject.transform.GetForward() * 0);

                gameObject.SetImpulse(gameObject.transform.GetForward() * -10);
                
                wanderState = WanderState.HIT;
                cumTimer = cumDuration;
            }
        }

    public virtual Vector3 CalculateNewPosition(float maxPos)
    {
        //Vector3 newPosition = new Vector3(0, 0, 0);
        //Random random = new Random();

        //newPosition.x = random.Next((int)maxPos);
        //newPosition.y = gameObject.transform.localPosition.y;
        //newPosition.z = random.Next((int)maxPos);

        //return newPosition;

        return InternalCalls.GetWalkablePointAround(gameObject.transform.globalPosition, maxPos);
    }

    public void LookAt(Vector3 pointToLook)
    {
        Vector3 direction = pointToLook - gameObject.transform.globalPosition;
        direction = direction.normalized;
        float angle = (float)Math.Atan2(direction.x, direction.z);

        //Debug.Log("Desired angle: " + (angle * Mathf.Rad2Deg).ToString());

        if (Math.Abs(angle * Mathf.Rad2Deg) < 1.0f)
            return;

        Quaternion dir = Quaternion.RotateAroundAxis(Vector3.up, angle);

        float rotationSpeed = Time.deltaTime * agent.angularSpeed;
        //Debug.Log("CS: Rotation speed: " + rotationSpeed.ToString());
        //Debug.Log("CS: Time: " + Time.deltaTime);

        Quaternion desiredRotation = Quaternion.Slerp(gameObject.transform.localRotation, dir, rotationSpeed);

        gameObject.transform.localRotation = desiredRotation;

    }


    public void MoveToCalculatedPos(float speed)
    {
        Vector3 pos = gameObject.transform.globalPosition;
        //Debug.Log("[WARNING] Position" + gameObject.transform.globalPosition);
        Vector3 direction = agent.GetDestination() - pos;
        //Debug.Log("[WARNING] Destination" + agent.GetDestination());

        gameObject.transform.localPosition += direction.normalized * speed * Time.deltaTime;
    }


    public void IsReached(Vector3 position, Vector3 destintion)
    {
        Vector3 roundedPosition = new Vector3(Mathf.Round(position.x),
                                      Mathf.Round(position.y),
                                      Mathf.Round(position.z));

        Vector3 roundedDestination = new Vector3(Mathf.Round(destintion.x),
                                                 Mathf.Round(destintion.y),
                                                 Mathf.Round(destintion.z));


        Debug.Log(roundedPosition);
        Debug.Log(roundedDestination);

        if (roundedPosition == roundedDestination)
        {
            wanderState = WanderState.REACHED;
            
        }
    }
}

