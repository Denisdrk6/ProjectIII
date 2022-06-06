using System;
using RagnarEngine;

public class Voice : RagnarComponent
{
	public GameObject player;
	public GameObject selectedEnemy;
	public GameObject[] enemies;
	public PlayerManager playerManager;
	NavAgent agent;
	bool check = false;

	public void Start()
	{
		player = GameObject.Find("Player");
		enemies = GameObject.FindGameObjectsWithTag("Enemies");
		playerManager = GameObject.Find("PlayerManager").GetComponent<PlayerManager>();
		agent = player.GetComponent<NavAgent>();	
	}
	public void Update()
	{
		//Debug.Log(enemies.Length.ToString());
		selectedEnemy = EnemyFound();
		if (selectedEnemy != null && selectedEnemy.GetComponent<AirEnemy>().enemyType != EnemyType.AIR)
		{
			if(!check)
            {
				Vector3 newForward = selectedEnemy.transform.globalPosition - player.transform.globalPosition;
				double angle = Math.Atan2(newForward.x, newForward.z);
				Quaternion rot = new Quaternion(0, (float)(1 * Math.Sin(angle / 2)), 0, (float)Math.Cos(angle / 2));
				player.GetComponent<Rigidbody>().SetBodyRotation(rot);
				player.GetComponent<Animation>().PlayAnimation("Ability2");
				player.GetComponent<Player>().PlayAudioClip("WPN_VOICE");
				check = true;
            }
			BasicEnemy enemyScript = selectedEnemy.GetComponent<BasicEnemy>();
			ActivateVoice();
			enemyScript.initialPos = selectedEnemy.transform.globalPosition;
			enemyScript.initialRot = selectedEnemy.transform.globalRotation;
		}
        else
        {
			GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].cooldown = 0;
		}
		InternalCalls.Destroy(gameObject);
	}
	public GameObject EnemyFound()
	{
		GameObject enemy = RayCast.HitToTag(agent.rayCastA, agent.rayCastB, "Enemies");
		if (enemy != null && Transform.GetDistanceBetween(player.transform.globalPosition, enemy.transform.globalPosition) < 15)
        {
			switch(enemy.GetComponent<BasicEnemy>().state)
            {
				case EnemyState.DEATH:
					GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].counter = GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].cooldown;
					return null;
				case EnemyState.IS_DYING:
					GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].counter = GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].cooldown;
					return null;
				default:
					return enemy;
			}			
        }
		GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].counter = GameObject.Find("PlayerManager").GetComponent<PlayerManager>().characters[0].abilities[1].cooldown;
		return null;

	}
	public void ActivateVoice()
    {        
		playerManager.players[playerManager.characterSelected].GetComponent<Player>().SetControled(false);
		if (selectedEnemy.GetComponent<BasicEnemy>().ToString() == "BasicEnemy")
		{
			selectedEnemy.GetComponent<BasicEnemy>().SetControled(true);
			GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesControlled++;
		}
		if (selectedEnemy.GetComponent<TankEnemy>().ToString() == "TankEnemy")
		{
			selectedEnemy.GetComponent<TankEnemy>().SetControled(true);
			 GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesControlled++;
		}
		if (selectedEnemy.GetComponent<UndistractableEnemy>().ToString() == "UndistractableEnemy")
		{
			selectedEnemy.GetComponent<UndistractableEnemy>().SetControled(true);
			GameObject.Find("Quest System").GetComponent<QuestSystem>().enemiesControlled++;
		}
	}
}

