using System;
using RagnarEngine;

public class Cinematic_2 : RagnarComponent
{
    public bool runGame = true;
    public GameObject[] bands;

    public int IdDialogue = 0;

    CinematicManager dialogues;

    public int IdLine = 0;
    enum CinematicState
    {
        FIRST, TRANSITION, ANIMATIONS
    }
    private CinematicState state;

    public void Start()
    {
        //Set UI Bands
        bands = new GameObject[2];
        bands[0] = GameObject.Find("High_Band");
        bands[1] = GameObject.Find("Low_Band");

        bands[0].transform.globalPosition = new Vector3(0f, 449f, -10.4f);
        bands[1].transform.globalPosition = new Vector3(0f, -447f, -10.4f);

        // Deberian ser variables publicas para poder ponerles las animaciones en la funcion: Animations()
        GameObject char_boss = GameObject.Find("char_boss");
        GameObject char_enemy1 = GameObject.Find("char_enemy1");
        GameObject char_enemy2 = GameObject.Find("char_enemy2");
        GameObject char_enemy3 = GameObject.Find("char_enemy3");
        GameObject char_enemy4 = GameObject.Find("char_enemy4");

        char_boss.GetComponent<Animation>().PlayAnimation("Run");
        char_enemy1.GetComponent<Animation>().PlayAnimation("Idle");
        char_enemy2.GetComponent<Animation>().PlayAnimation("Idle");
        char_enemy3.GetComponent<Animation>().PlayAnimation("Idle");
        char_enemy4.GetComponent<Animation>().PlayAnimation("Idle");

        dialogues = GameObject.Find("CinematicDialogue").GetComponent<CinematicManager>();

        //-----------
        state = CinematicState.FIRST;
        //-----------

    }

    public void Update()
    {
        switch (state)
        {
            case CinematicState.FIRST:

                StartCinematic();
                state = CinematicState.TRANSITION;

                break;
            case CinematicState.TRANSITION:
                //Aqui no pasara nada

                break;
            case CinematicState.ANIMATIONS:

                Animations();
                state = CinematicState.TRANSITION;

                break;
            default:
                break;
        }
    }

    // IMPORTATNTE CAMBIAR ESTO SEGUN TU CINEMATICA
    private void StartCinematic()
    {
        //var1: ID del dialogo que se hara en la cinematica(variable arriba)
        //var2: Nombre de la escena a la que se ir� cuando acabe el dialogo
        //var3: Numera de la cinematica (Ej: Cinematica_1 = 1)
        dialogues.SetIDDialogue(IdDialogue, "build", 2);
    }

    private void Animations()
    {
        switch (IdLine)
        {   // EL dialogo puede tener mas o menos lineas
            // Tened en cuenta que por aqui solo pasara...
            // cuando se pase a la siguiente linea de dialogo
            case 0:

                break;
            case 1:

                break;
            case 2:

                break;
            case 3:

                break;
            case 4:

                break;
            case 5:

                break;
            case 6:

                break;

            default:
                break;
        }
    }

    public void SetLine(int line)
    {
        IdLine = line;
        state = CinematicState.ANIMATIONS;
    }
}