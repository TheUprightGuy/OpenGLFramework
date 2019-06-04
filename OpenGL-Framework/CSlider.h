#pragma once
#include "utils.h"
#include "object.h"
#include "CameraManager.h"
#include "input.h"

class CSlider
{
public:
	CSlider(float sliderY, float sliderStartX, float sliderX, float sliderEndX)
	{

		sliderStartPos = { sliderStartX, sliderY, 0.0f };
		sliderPos = { sliderX, sliderY, 0.0f };
		sliderEndPos = { sliderEndX, sliderY, 0.0f };

		float distance = glm::distance(sliderPos, sliderStartPos);

		initinfo barOn;
		barOn.imgFilepath = "Resources/red.jpg";
		barOn.objPosition = { sliderPos.x + (distance / 2), sliderStartPos.y, sliderStartPos.z };
		barOn.objScale = { distance / 2, 0.002f, 0.1f };
		m_barOn = new CObject(DEFAULT, MESH_2D_SPRITE, barOn);

		distance = glm::distance(sliderPos, sliderEndPos);

		initinfo barOff;
		barOff.imgFilepath = "Resources/green.jpg";
		barOff.objPosition = { sliderPos.x - (distance / 2), sliderStartPos.y, sliderStartPos.z };
		barOff.objScale = { distance / 2, 0.002f, 0.1f };
		m_barOff = new CObject(DEFAULT, MESH_2D_SPRITE, barOff);

		initinfo slider;
		slider.objScale = { 0.04f, 0.04f, 0.1f };
		slider.imgFilepath = "Resources/sliderOnOff.png";
		slider.objPosition = sliderPos;
		slider.texStartScale = { 0.0f, 0.0f };
		slider.texEndScale = { 1.0f, 0.5f };
		m_slider = new CObject(DEFAULT, MESH_2D_SPRITE, slider);
	}

	void render()
	{
		m_slider->Render(CCameraManager::GetInstance().GetOrthoCam());
		m_barOn->Render(CCameraManager::GetInstance().GetOrthoCam());
		m_barOff->Render(CCameraManager::GetInstance().GetOrthoCam());
	}

	float process()
	{
		glm::vec3 m_MousePos = CInput::GetInstance().GetMousePos();
		float mouseX = (2.0f * m_MousePos.x) / (float)glutGet(GLUT_WINDOW_WIDTH) - 1.0f;
		float mouseY = 1.0f - (2.0f * m_MousePos.y) / (float)glutGet(GLUT_WINDOW_HEIGHT);

		glm::vec3 _sliderPos = m_slider->GetPos();
		glm::vec3 _sliderScale = m_slider->GetScale();
		if (CInput::GetInstance().GetMouseState(0) == INPUT_HOLD &&
			//mouseX > _sliderPos.x - _sliderScale.x  && mouseX < _sliderPos.x + _sliderScale.x &&
			mouseY > _sliderPos.y - _sliderScale.y  && mouseY < _sliderPos.y + _sliderScale.y &&
			mouseX > sliderStartPos.x + _sliderScale.x   && mouseX < sliderEndPos.x - _sliderScale.x
			)
		{
			sliderPos.x = mouseX;
			m_slider->Translate(sliderPos);
		}

		if (mouseX > _sliderPos.x - _sliderScale.x  && mouseX < _sliderPos.x + _sliderScale.x &&
			mouseY > _sliderPos.y - _sliderScale.y  && mouseY < _sliderPos.y + _sliderScale.y)
		{
			m_slider->ChangeTexture({ 0.0f, 0.5f }, { 1.0f, 0.5f });
		}
		else
		{
			m_slider->ChangeTexture({ 0.0f, 0.0f }, { 1.0f, 0.5f });
		}
		float distance = glm::distance(sliderPos, sliderEndPos);
		m_barOn->Translate({ sliderPos.x + (distance / 2), sliderEndPos.y, sliderEndPos.z });
		m_barOn->Scale({ distance / 2, 0.01f, 0.1f });

		distance = glm::distance(sliderPos, sliderStartPos);
		m_barOff->Translate({ sliderPos.x - (distance / 2), sliderStartPos.y, sliderStartPos.z });
		m_barOff->Scale({ distance / 2, 0.01f, 0.1f });

		return ((sliderPos.x - sliderStartPos.x) / (sliderEndPos.x - sliderStartPos.x));
	}

	~CSlider();

	void Translate( float sliderStartX, float sliderX, float sliderEndX)
	{
		sliderStartPos += glm::vec3( sliderStartX, 0.0f, 0.0f );
		sliderPos += glm::vec3(sliderX, 0.0f, 0.0f );
		m_slider->Translate(sliderPos);
		sliderEndPos += glm::vec3(sliderEndX, 0.0f, 0.0f);
	}

	float GetSliderPosX() { return(m_slider->GetPos().x); }

private:
	CObject* m_slider;
	CObject* m_barOn;
	CObject* m_barOff;

	glm::vec3 sliderStartPos;
	glm::vec3 sliderEndPos;
	glm::vec3 sliderPos;
};

