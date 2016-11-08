﻿#include "BgScene.h"

USING_NS_CC;

Scene* Bg::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Bg::create();

	// add layer as a child to scene
	scene->addChild(layer);

	return scene;
}

bool Bg::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	/*auto sp = Sprite::create("CloseNormal.png");
	sp->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sp,1);*/

	initBG();

	// 添加飞机
	planeLayer = PlaneLayer::create();
	this->addChild(planeLayer, 1);

	this->scheduleUpdate(); // 更新

	// 键盘事件
	auto listenerKeyPad = EventListenerKeyboard::create();
	listenerKeyPad->onKeyPressed = CC_CALLBACK_2(Bg::onKeyPressed, this);
	listenerKeyPad->onKeyReleased = CC_CALLBACK_2(Bg::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerKeyPad, this);

	// 按键是否按下
	isAdown = false;
	isDdown = false;

	return true;
}

void Bg::initBG()
{
	m_bgSprite1 = Sprite::create("bg1.png");
	m_bgSprite1->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(m_bgSprite1, 0);

	m_bgSprite2 = Sprite::create("bg2.png");
	m_bgSprite2->setPosition(Point(visibleSize.width / 2, visibleSize.height + visibleSize.height / 2));
	this->addChild(m_bgSprite2, 0);

}

void Bg::update(float delta)
{
	int posY1 = m_bgSprite1->getPositionY();	//背景地图1的Y坐标
	int posY2 = m_bgSprite2->getPositionY();	//背景地图2的Y坐标

	int iSpeed = 1;	//地图滚动速度

	/* 两张地图向左滚动（两张地图是相邻的，所以要一起滚动，否则会出现空隙） */
	posY1 -= iSpeed;
	posY2 -= iSpeed;

	/* 图大小 */
	Size bgSize = m_bgSprite1->getContentSize();

	/* 当第1个地图完全离开屏幕时，第2个地图刚好完全出现在屏幕上，这时候就让第1个地图紧贴在
	 第2个地图后面 */
	if (posY1 <= -bgSize.height / 2) {
		posY1 = bgSize.height + bgSize.height / 2;
	}
	/* 同理，当第2个地图完全离开屏幕时，第1个地图刚好完全出现在屏幕上，这时候就让第2
	 个地图紧贴在第1个地图后面 */
	if (posY2 <= -bgSize.height / 2) {
		posY2 = bgSize.height + bgSize.height / 2;
	}

	m_bgSprite1->setPositionY(posY1);
	m_bgSprite2->setPositionY(posY2);


	// 飞机移动
	if (isAdown)
		this->planeLayer->moveLeftOrRight('A');
	if (isDdown)
		this->planeLayer->moveLeftOrRight('D');

}

// 按键 按下
void Bg::onKeyPressed(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		//Director::getInstance()->pause();
		//this->planeLayer->moveLeftOrRight('A');
		isAdown = true;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		//this->planeLayer->moveLeftOrRight('D');
		isDdown = true;
		break;
	case EventKeyboard::KeyCode::KEY_W:

		break;
	case EventKeyboard::KeyCode::KEY_S:

		break;

	// 用来发射子弹
	case EventKeyboard::KeyCode::KEY_J:
	{
		this->schedule(SEL_SCHEDULE(&Bg::addBullet), 0.08f); // 开启子弹发射
	}
	break;

	default:
		break;
	}
}

//按键 释放
void Bg::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		isAdown = false;
		break;
	case EventKeyboard::KeyCode::KEY_D:
		isDdown = false;
		break;
	case EventKeyboard::KeyCode::KEY_W:

		break;
	case EventKeyboard::KeyCode::KEY_S:

		break;

	case EventKeyboard::KeyCode::KEY_J:
	{
		this->unschedule(SEL_SCHEDULE(&Bg::addBullet)); // 取消子弹定时器
	}
		break;

	default:
		break;
	}
}

void Bg::addBullet(float tm)
{
	auto bullet = Bullet::create();
	bullet->initBullet("AirplaneResource\\ui\\shoot\\bullet1.png");
	// 设置字段的位置为飞机的出口处 
	bullet->setPosition(Point(planeLayer->plane->getPositionX(),
		planeLayer->plane->getPositionY() + planeLayer->plane->getContentSize().height / 2 + 10));
	// 添加到场景中 
	this->addChild(bullet, 1);

	// 这里还需要将子弹添加到管理器中，一遍后续的碰撞，释放等相关操作 
	// TODO
	
}