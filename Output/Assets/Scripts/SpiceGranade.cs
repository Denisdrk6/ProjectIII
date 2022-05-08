using System;
using RagnarEngine;

public class SpiceGranade : RagnarComponent
{
	Rigidbody goRB;

	private float force = 1050;
	public float explosionRadius = 6f;
	private float cooldown = 0f;
	private Vector3 relativePos;

	public bool pendingToDelete = false;
	public void Start()
	{
		goRB = gameObject.GetComponent<Rigidbody>();
		AimMethod();
	}
	public void Update()
	{
		goRB.ApplyVelocity(relativePos.normalized * 25);
		if (cooldown > 0 && gameObject != null)
		{
			cooldown -= Time.deltaTime;
			if (cooldown < 0)
			{
				cooldown = 0f;
				pendingToDelete = true;
			}
		}
		if (pendingToDelete) InternalCalls.Destroy(gameObject);
	}
	private void AimMethod()
	{
		GameObject player = GameObject.Find("Player_2");
		Vector3 pos = player.transform.globalPosition;
		pos.y += 1.5f;
		goRB.SetBodyPosition(pos);
		gameObject.transform.globalPosition = pos;

		float radius = GameObject.Find("PlayerManager").GetComponent<PlayerManager>().radius;
		Vector3 hitPoint = GameObject.Find("LevelManager").GetComponent<Level_1>().hitPoint;
		relativePos = hitPoint - pos;
		if (relativePos.magnitude > radius)
		{
			relativePos.y = hitPoint.y;
			pos.y -= 1.5f;
			Vector3 newPos = pos + relativePos.normalized * radius;
			pos.y += 1.5f;
			relativePos = newPos - pos;
		}

		goRB.IgnoreCollision(player, true);
	}
	public void OnCollisionEnter(Rigidbody other)
	{
		goRB.SetAsStatic();
		
		gameObject.GetComponent<ParticleSystem>().Play();

		Rigidbody area = gameObject.CreateComponent<Rigidbody>();
		CreateSphereTrigger(area, explosionRadius, gameObject.transform.globalPosition);
		cooldown = 1;
	}

	private static void CreateSphereTrigger(Rigidbody rb, float radius, Vector3 pos)
	{
		rb.SetCollisionSphere(radius, pos.x, pos.y, pos.z);
		rb.SetAsTrigger();
	}
}