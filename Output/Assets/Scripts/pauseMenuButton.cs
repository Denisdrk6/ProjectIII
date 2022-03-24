using System;
using RagnarEngine;

public class pauseMenuButton : RagnarComponent
{
	Vector3 pos;
	bool isSowing = false;
	bool firstTime = true;
	bool isFirstS = true;
	GameObject Image;
	GameObject Resume;
	GameObject MainM;
	GameObject Opt;
	GameObject Rect;
	public void Start()
    {

	}
	
	public void Update()
	{
		if (firstTime)
		{
			Image = GameObject.Find("BackgroundImage");
			Rect = GameObject.Find("UIRectangle");
			Resume = GameObject.Find("ButtonResume");
			MainM = GameObject.Find("ButtonMainMenu");
			Opt = GameObject.Find("ButtonOptions");

			pos = new Vector3(0.0f, 0.0f, 0.0f);

			ImageHide();
			RectangleHide();
			ResumeButtonHide();
			MainMenuButtonHide();
			OptionsButtonHide();

			firstTime = false;
		}

		if(isSowing)
        {
			ImageShow();
			RectangleShow();
			ResumeButtonShow();
			MainMenuButtonShow();
			OptionsButtonShow();
			if(isSowing == false)
            {
				ImageHide();
				RectangleHide();
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
			}
		}

		if (Input.GetKey(KeyCode.ESCAPE) == KeyState.KEY_DOWN)
		{
			if (isSowing)
			{
				isSowing = false;

				ImageShow();
				RectangleShow();
				ResumeButtonShow();
				MainMenuButtonShow();
				OptionsButtonShow();

			}
			else
			{
				ImageHide();
				RectangleHide();
				ResumeButtonHide();
				MainMenuButtonHide();
				OptionsButtonHide();
				isSowing = true;
				
			}
		}
	}

	void ImageShow()
    {
		pos.Set(0.0f, 0.0f, 84.0f);
		Image.GetComponent<Transform2D>().position2D = pos;
		Image.GetComponent<Transform2D>().SetSize(InternalCalls.GetRegionGame());
	}

	void ImageHide()
    {
		pos.Set(0.0f, 1000.0f, 84.0f);
		Image.GetComponent<Transform2D>().position2D = pos;
	}

	void ResumeButtonShow()
	{
		pos.Set(-363.0f, 30.0f, 90.0f);
		Resume.GetComponent<Transform2D>().position2D = pos;

		int a = Resume.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				isFirstS = true;
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstS)
				{
					isFirstS = false;

					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				isSowing = false;
				//Quitar menu de pausa
				break;
		}
	}
	void ResumeButtonHide()
	{
		pos.Set(0.0f, 1000.0f, 90.0f);
		Resume.GetComponent<Transform2D>().position2D = pos;
	}

	void MainMenuButtonShow()
	{
		pos.Set(-363.0f, -28.0f, 90.0f);
		MainM.GetComponent<Transform2D>().position2D = pos;

		int a = MainM.GetComponent<UIButton>().GetButtonState();
		switch (a)
		{
			case 0:
				// disabled Mode
				break;
			case 1:
				isFirstS = true;
				// normal Mode
				break;
			case 2:
				// focused mode
				if (isFirstS)
				{
					isFirstS = false;

					//poner sonido
				}

				break;
			case 3:
				// pressed mode
				SceneManager.LoadScene("MainMenu");
				//cambiar de escena
				break;
		}
	}
	void MainMenuButtonHide()
	{
		pos.Set(0.0f, 1000.0f, 90.0f);
		MainM.GetComponent<Transform2D>().position2D = pos;
	}


	void OptionsButtonShow()
	{
		pos.Set(-356.5f, -0.0f, 90.0f);
		Opt.GetComponent<Transform2D>().position2D = pos;

		// Options button logic
	}
	void OptionsButtonHide()
	{
		pos.Set(0.0f, 1000.0f, 90.0f);
		Opt.GetComponent<Transform2D>().position2D = pos;
	}
	void RectangleShow()
	{
		pos.Set(-326.0f, 7.5f, 88.2f);
		Rect.GetComponent<Transform2D>().position2D = pos;
	}
	void RectangleHide()
	{
		pos.Set(0.0f, 1000.0f, 88.2f);
		Rect.GetComponent<Transform2D>().position2D = pos;
	}
}
