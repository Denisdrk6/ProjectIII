using System;
using RagnarEngine;

public class Eagle : RagnarComponent
{
	public GameObject player;
	public PlayerManager playerManager;
    public bool controled = false;
    public float cooldown = 0;
    public float soundRadius = 6f;
    Rigidbody goRB;
    ParticleSystem leftParticles;
    ParticleSystem rightParticles;
    NavAgent agent;
    
    public void Start()
	{
        //gameObject.GetComponent<AudioSource>().PlayClip("WPN_EAGLEORDER");
        agent = gameObject.GetComponent<NavAgent>();
        controled = true;
        player = GameObject.Find("Player");
        goRB = gameObject.GetComponent<Rigidbody>();
        goRB.SetBodyPosition(player.transform.globalPosition);
        goRB.IgnoreCollision(player, true);
        agent.CalculatePath(agent.hitPosition);
        agent.MovePath();
        leftParticles = GameObject.Find("LeftWingParticles").GetComponent<ParticleSystem>();
        rightParticles = GameObject.Find("RightWingParticles").GetComponent<ParticleSystem>();
        leftParticles.Play();
        rightParticles.Play();
    }
	public void Update()
	{
        if(!agent.MovePath() && cooldown == 0)
        {
            Rigidbody area = gameObject.CreateComponent<Rigidbody>();
            CreateSphereTrigger(area, soundRadius, gameObject.transform.globalPosition);
            cooldown = 6f;
        }
        if (cooldown > 0 && gameObject != null)
        {
            cooldown -= Time.deltaTime;
            if (cooldown < 0)
            {
                cooldown = 0f;
                InternalCalls.Destroy(gameObject);
                leftParticles.Pause();
                rightParticles.Pause();
            }
        }
    }

    private static void CreateSphereTrigger(Rigidbody rb, float radius, Vector3 pos)
    {
        rb.SetCollisionSphere(radius, pos.x, pos.y, pos.z);
        rb.SetAsTrigger();
    }

}