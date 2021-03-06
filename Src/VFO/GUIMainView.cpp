/*
 * GUIMainView.cpp
 *
 *  Created on: Apr 1, 2018
 *      Author: lymes
 */

#include "GUIMainView.h"
#include "VFOController.h"

#include "GUI/Utils.h"

extern VFO::VFOController *_mainController;

#define ENCODER_THRESHOLD 4
#define DIVIDER_FACTOR 11


namespace VFO
{

GUIMainView::GUIMainView()
{
	_window = new GradientBox(0, 0, 160, 128);

	_btn1 = new GradientBox(10, 50, 40, 25);
	_btn1->downLeft =
	{	10, 10, 10};
	_btn1->downRight =
	{	10, 10, 10};
	_btn1->upLeft =
	{	100, 100, 100};
	_btn1->upRight =
	{	100, 100, 100};

	_btn2 = new GradientBox(60, 50, 40, 25);
	_btn2->downLeft =
	{	10, 10, 10};
	_btn2->downRight =
	{	10, 10, 10};
	_btn2->upLeft =
	{	100, 100, 100};
	_btn2->upRight =
	{	100, 100, 100};

	_btn3 = new GradientBox(110, 50, 40, 25);
	_btn3->downLeft =
	{	10, 10, 10};
	_btn3->downRight =
	{	10, 10, 10};
	_btn3->upLeft =
	{	100, 100, 100};
	_btn3->upRight =
	{	100, 100, 100};

	_freqString = new FrequencyString(0, 10, _window);
	_sMeter = new SMeter(10, 90, _window);
}

GUIMainView::~GUIMainView()
{
	delete _btn1;
	delete _btn2;
	delete _btn3;
	delete _window;
}

void GUIMainView::menuKeyPressed()
{
	_mainController->showSetup();
}

void GUIMainView::draw()
{
	this->drawBackground();

	_freqString->setBackground(_window);
	_freqString->draw(_mainController->getConfig()->getFrequency());
}

void GUIMainView::drawBackground()
{
	_window->draw();

	_btn1->draw();
	_btn2->draw();
	_btn3->draw();

	ST7735_Rect(10, 50, 50, 75, COLOR565_WHITE);
	ST7735_Rect(60, 50, 100, 75, COLOR565_WHITE);
	ST7735_Rect(110, 50, 150, 75, COLOR565_WHITE);

	char tt[] = "SSB";
	ST7735_PutStr7x11Ex(18, 57, VFO::utf8to1251(tt), COLOR565_YELLOW, _btn1,
			VFO::backgroundColor);

	_sMeter->draw(255);

	ST7735_defineScrollArea(100, 160);


}

void GUIMainView::pushEncoderIncrement(int16_t increment, uint16_t period)
{
	uint32_t freq = _mainController->getConfig()->getFrequency();
	uint16_t encoderRes = _mainController->getConfig()->getEncoder();

	uint16_t factor = 1;
	if (abs(increment) > (encoderRes / 2))  // half rotation
	{
		factor = 100;
	}
	else if (abs(increment) > (encoderRes / 4)) // quarter rotation
	{
		factor = 10;
	}
	freq += increment * factor / ENCODER_THRESHOLD;
	_mainController->setFrequency(freq);
	_mainController->triggerMemoryWrite();

	_freqString->draw(freq);


//	static int tt = 0;
//	tt += increment;
//
//	if (tt > 160)
//	{
//		tt = 100;
//	}
//	if ( tt < 100 )
//	{
//		tt = 160;
//	}
//	ST7735_scroll(tt);
}

void GUIMainView::showVoltage(uint32_t value)
{
	char buf[6];
	value = value * DIVIDER_FACTOR;

	static uint32_t oldValue = 0;
	value = ( oldValue * 7 + value ) >> 3;  // cumulative moving average filter
	oldValue = value;

	ST7735_PutStr5x7Ex(1, 116, 60, VFO::voltageToStr(value, buf, 6),
				COLOR565_WHITE, _btn3, VFO::backgroundColor);

}



} /* namespace VFO */
