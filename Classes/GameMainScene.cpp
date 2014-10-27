#include "GameMainScene.h"
#include "GameManager.h"
#include "SelectMissionScene.h"
#include "Control/PopLayer.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 

#include "platform/android/jni/JniHelper.h"
#include "jni.h"

#endif


enum ButtonType {
	BT_CHEAT_OK = 1000,
	BT_CHEAT_CANCEL,
	BT_RESET_OK,
	BT_RESET_CANCEL,
	BT_NEXT_MISSION_OK,
	BT_NEXT_MISSION_SHARE,
	BT_SELECT_MISSSION_OK,
	BT_SELECT_MISSSION_CANCEL,
	BT_BUY_OK,
	BT_BUY_CANCEL
};

enum Epay{
	FEE_RESULT_SUCCESS = 101,
	FEE_RESULT_CANCELED,
	FEE_RESULT_FAILED
};

GameMainScene::GameMainScene() :
	_winCell(170, 100, 290, 290), _heros(), _layoutInfo(), _currentDirection(),
	_currentPos(), _beginPos(), _strStep(GET_STR("step"))
{

}

Scene * GameMainScene::createScene(int idx, bool isContinue) {
	auto scene = Scene::create();

	auto layer = GameMainScene::create(idx, isContinue);
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool GameMainScene::init() {
	if (!Layer::init())
		return false;
	return true;
}

bool GameMainScene::initWithMissionIdx(int idx, bool isContinue) {
	if (!this->init())
		return false;
	//Director::getInstance()->getOpenGLView()->setDesignResolutionSize(640, 960, ResolutionPolicy::FIXED_WIDTH);

	_userData.index = idx + 1;
	_userData.step = 0;
	_userData.usedTime = 0;
	_userData.isDone = false;
	_userData.isLocked = false;

	Size visiableSize = Director::getInstance()->getVisibleSize();

	GameManager::getInstance()->getHeroLayoutInfoByIdx(idx, _layoutInfo);
	if (isContinue)
		GameManager::getInstance()->loadCurrentMission(_layoutInfo);

	auto heros = GameManager::getInstance()->getHeros();

	//register EventListner
	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *event) {

		auto sprite = static_cast<Sprite *>(event->getCurrentTarget());

		auto rect = sprite->getBoundingBox();
		auto point = touch->getLocation();

		if(rect.containsPoint(point))
		{
			_beginPos = point;
			_currentPos = sprite->getPosition();
			_currentDirection = checkMoveDirection(sprite);
			return true;
		}
		return false;

	};

	listener->onTouchMoved = [&](Touch *touch, Event *event) {

		auto sprite = static_cast<Sprite *>(event->getCurrentTarget());

		float offsetX = touch->getLocation().x - _beginPos.x;
		float offsetY = touch->getLocation().y - _beginPos.y;

		switch (_currentDirection)
		{
		case CanMovedDirection::UNMOVEABLE:
			return;
		case CanMovedDirection::MOVEABLE_UP:
			if(offsetY <= 0) offsetY = 0;
			if(offsetY >= CELL_HEIGHT) offsetY = CELL_HEIGHT;
			sprite->setPosition(_currentPos.x, _currentPos.y + offsetY );
			break;

		case CanMovedDirection::MOVEABLE_RIGHT:
			if(offsetX <= 0) offsetX = 0;
			if(offsetX >= CELL_WIDTH) offsetX = CELL_WIDTH;
			sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			break;

		case CanMovedDirection::MOVEABLE_LEFT:
			if(offsetX >= 0) offsetX = 0;
			if(offsetX <= 0 - CELL_WIDTH) offsetX = 0 - CELL_WIDTH;
			sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			break;

		case CanMovedDirection::MOVEABLE_DOWN:
			if(offsetY >= 0) offsetY = 0;
			if(offsetY <= 0 - CELL_HEIGHT) offsetY = 0 - CELL_HEIGHT;
			sprite->setPosition(_currentPos.x , _currentPos.y + offsetY);
			break;

		case CanMovedDirection::MOVEABLE_LEFT_RIGHT:
			if(offsetX >= CELL_WIDTH ) offsetX = CELL_WIDTH;
			if(offsetX <= 0 - CELL_WIDTH) offsetX = 0 - CELL_WIDTH;
			sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			break;

		case CanMovedDirection::MOVEABLE_UP_DOWN:
			if(offsetY >= CELL_HEIGHT ) offsetX = CELL_HEIGHT;
			if(offsetY <= 0 - CELL_HEIGHT) offsetX = 0 - CELL_HEIGHT;
			sprite->setPosition(_currentPos.x, _currentPos.y + offsetY);
			break;

		case CanMovedDirection::MOVEABLE_LEFT_DOWN:
			if(ABS(offsetX) >= ABS(offsetY)) //ˮƽ����ƫ�ƾ���ֵ ���� ��ֱ����ƫ�ƾ���ֵ
			{
				if(offsetX >= 0) offsetX = 0;
				if(offsetX <= 0 - CELL_WIDTH) offsetX = 0 - CELL_WIDTH;
				sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			}
			else
			{
				if(offsetY >= 0) offsetY = 0;
				if(offsetY <= 0 - CELL_HEIGHT) offsetY = 0 - CELL_HEIGHT;
				sprite->setPosition(_currentPos.x , _currentPos.y + offsetY);
			}
			break;

		case CanMovedDirection::MOVEABLE_LEFT_UP:
			if(ABS(offsetX) >= ABS(offsetY)) //ˮƽ����ƫ�ƾ���ֵ ���� ��ֱ����ƫ�ƾ���ֵ
			{
				if(offsetX >= 0) offsetX = 0;
				if(offsetX <= 0 - CELL_WIDTH) offsetX = 0 - CELL_WIDTH;
				sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			}
			else
			{
				if(offsetY <= 0) offsetY = 0;
				if(offsetY >= CELL_HEIGHT) offsetY = CELL_HEIGHT;
				sprite->setPosition(_currentPos.x, _currentPos.y + offsetY );
			}
			break;

		case CanMovedDirection::MOVEABLE_RIGHT_DOWN:
			if(ABS(offsetX) >= ABS(offsetY)) //ˮƽ����ƫ�ƾ���ֵ ���� ��ֱ����ƫ�ƾ���ֵ
			{
				if(offsetX <= 0) offsetX = 0;
				if(offsetX >= CELL_WIDTH) offsetX = CELL_WIDTH;
				sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			}
			else
			{
				if(offsetY >= 0) offsetY = 0;
				if(offsetY <= 0 - CELL_HEIGHT) offsetY = 0 - CELL_HEIGHT;
				sprite->setPosition(_currentPos.x , _currentPos.y + offsetY);
			}
			break;

		case CanMovedDirection::MOVEABLE_RIGHT_UP:
			if(ABS(offsetX) >= ABS(offsetY)) //ˮƽ����ƫ�ƾ���ֵ ���� ��ֱ����ƫ�ƾ���ֵ
			{
				if(offsetX <= 0) offsetX = 0;
				if(offsetX >= CELL_WIDTH) offsetX = CELL_WIDTH;
				sprite->setPosition(_currentPos.x + offsetX, _currentPos.y);
			}
			else
			{
				if(offsetY <= 0) offsetY = 0;
				if(offsetY >= CELL_HEIGHT) offsetY = CELL_HEIGHT;
				sprite->setPosition(_currentPos.x, _currentPos.y + offsetY );
			}
			break;

		default:
			break;
		}

	};

	listener->onTouchEnded =
		[&](Touch *touch, Event *event)
	{
		auto sprite = static_cast<Sprite *>(event->getCurrentTarget());

		float offsetX = sprite->getPosition().x - _beginPos.x;
		float offsetY = sprite->getPosition().y - _beginPos.y;

		do
		{
			if(ABS(offsetX)>= CELL_WIDTH / 2 && offsetX > 0)
			{
				sprite->setPosition(_currentPos.x + CELL_WIDTH, _currentPos.y);
				_userData.step++;
				break;
			}
			if(ABS(offsetX)>= CELL_WIDTH / 2 && offsetX < 0)
			{
				sprite->setPosition(_currentPos.x - CELL_WIDTH, _currentPos.y);
				_userData.step++;
				break;
			}
			if(ABS(offsetY) >= CELL_HEIGHT / 2 && offsetY > 0)
			{
				sprite->setPosition(_currentPos.x , _currentPos.y + CELL_HEIGHT);
				_userData.step++;
				break;
			}
			if(ABS(offsetY) >= CELL_HEIGHT / 2 && offsetY < 0)
			{
				sprite->setPosition(_currentPos.x , _currentPos.y - CELL_HEIGHT);
				_userData.step++;
				break;
			}
			sprite->setPosition(_currentPos);
		}while (0);

		refreshLayoutInfo();

		if(this->_winCell.containsPoint(GameManager::getInstance()->getHeros().at(HeroType::CAO_CAO)->getPosition()))
		{
			// 			PopLayer *popLayer = PopLayer::create("message.png");
			// 			popLayer->setTitle("��ϲ�㣬 ������");
			// 			popLayer->setContentText("");
			// 			popLayer->addButton("b6.png", "b6.png", "��һ��", ButtonType::BT_NEXT_MISSION_OK);
			// 			popLayer->addButton("b6.png", "b6.png", "���������", ButtonType::BT_NEXT_MISSION_SHARE);
			// 			popLayer->setCallbackFunc(this, (SEL_CallFuncN)&GameMainScene::popLayerCallFunc);
			// 			this->addChild(popLayer, 50);
			// 
			// 			_userData.isDone = true;
			// 			GameManager::getInstance()->saveMissionData(_userData);
			// 
			// 			MissionInfo nextMision;
			// 			nextMision.index = _userData.index + 1;
			// 			nextMision.step = 0;
			// 			nextMision.usedTime = 0;
			// 			nextMision.isDone = false;
			// 			nextMision.isLocked = false;
			// 			GameManager::getInstance()->saveMissionData(nextMision);	

			this->scheduleOnce(SEL_SCHEDULE(&GameMainScene::winFunc), 0.8f);

		}

	};

	listener->setSwallowTouches(true);

	//create sprites
	std::vector<HeroType> createdType;
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMS; ++j) {
			if (_layoutInfo.flags[i][j] == HeroType::NO_HERO) {
				Rect rect(j * CELL_WIDTH + 20, i * CELL_HEIGHT + 110,
					CELL_WIDTH, CELL_HEIGHT);
				_emptyCells.push_back(rect);
				continue;
			}

			if (std::find(createdType.begin(), createdType.end(),
				_layoutInfo.flags[i][j]) == createdType.end()) {
					createdType.push_back(_layoutInfo.flags[i][j]);

					Sprite *sprite = heros.at(_layoutInfo.flags[i][j]);

					sprite->setPosition(
						j * CELL_WIDTH + 20
						+ sprite->getContentSize().width / 2,
						i * CELL_HEIGHT + 110
						+ sprite->getContentSize().height / 2);

					_heros.pushBack(sprite);

					this->_eventDispatcher->addEventListenerWithSceneGraphPriority(
						listener->clone(), sprite);

					this->addChild(sprite, 10);
			}

		};


		LabelTTF *labelTime = LabelTTF::create();
		labelTime->setFontSize(30);
		labelTime->setTag(500);
		labelTime->setPosition(visiableSize.width * 5 / 6,visiableSize.height - 45);
		this->addChild(labelTime, 10);

		LabelTTF *labelStep = LabelTTF::create();
		labelStep->setTag(600);
		labelStep->setFontSize(30);
		labelStep->setPosition(visiableSize.width / 6 ,visiableSize.height - 45);
		this->addChild(labelStep, 10);

		String str;
		str.initWithFormat(GET_STR("mission"), _userData.index);
		LabelTTF *labelMissionNo = LabelTTF::create();
		labelMissionNo->setFontSize(30);
		labelMissionNo->setString(str.getCString());;
		labelMissionNo->setPosition(visiableSize.width / 2 ,visiableSize.height - 45);
		this->addChild(labelMissionNo, 10);

		//Add Menus
		MenuItemImage *selectItem =
			MenuItemImage::create("b1.png", "b1.png",
			[&](Ref *pRef) {
				PopLayer *popLayer = PopLayer::create("message.png");
				popLayer->setTitle("title.png");
				popLayer->setContentText(GET_STR("selectMission"), 20, 300);
				popLayer->addButton("ok.png", "ok.png", NULL, ButtonType::BT_SELECT_MISSSION_OK);
				popLayer->addButton("cancel.png", "cancel.png", NULL, ButtonType::BT_SELECT_MISSSION_CANCEL);
				popLayer->setCallbackFunc(this, (SEL_CallFuncN)&GameMainScene::popLayerCallFunc);
				this->addChild(popLayer, 50);
		});
		MenuItemImage *repeatItem =
			MenuItemImage::create("b2.png", "b2.png",
			[&](Ref *ref) {
				PopLayer *popLayer = PopLayer::create("message.png");
				popLayer->setTitle("title.png");
				popLayer->setContentText(GET_STR("resetGame"), 20, 300);
				popLayer->addButton("ok.png", "ok.png", NULL, ButtonType::BT_RESET_OK);
				popLayer->addButton("cancel.png", "cancel.png", NULL, ButtonType::BT_RESET_CANCEL);
				popLayer->setCallbackFunc(this, (SEL_CallFuncN)&GameMainScene::popLayerCallFunc);
				this->addChild(popLayer, 50);
		});

		MenuItemImage *cheatItem =
			MenuItemImage::create("b3.png", "b3.png",
			[&](Ref *ref) {
				if(GameManager::getInstance()->getCheatCount() <= 0)
				{
					PopLayer *popLayer = PopLayer::create("message.png");
					popLayer->setTitle("title.png");
					popLayer->setContentText(GET_STR("recharge"), 20, 300);
					popLayer->addButton("buy.png", "buy.png", NULL, ButtonType::BT_BUY_OK);
					popLayer->addButton("cancel.png", "cancel.png",NULL, ButtonType::BT_BUY_CANCEL);
					popLayer->setCallbackFunc(this, (SEL_CallFuncN)&GameMainScene::popLayerCallFunc);
					this->addChild(popLayer, 50);
				}
				else
				{
					String str;
					str.initWithFormat(GET_STR("cheatInfo"), GameManager::getInstance()->getCheatCount());
					PopLayer *popLayer = PopLayer::create("message.png");
					popLayer->setTitle("title.png");
					popLayer->setContentText(str.getCString());
					popLayer->addButton("used.png", "used.png", NULL, ButtonType::BT_CHEAT_OK);
					popLayer->addButton("no_used.png", "no_used.png",NULL, ButtonType::BT_CHEAT_CANCEL);
					popLayer->setCallbackFunc(this, (SEL_CallFuncN)&GameMainScene::popLayerCallFunc);
					this->addChild(popLayer, 50);

				}
		});

		MenuItemImage *shareItem =
			MenuItemImage::create("b4.png", "b4.png", [](Ref *pRef) {
				//Director::getInstance()->replaceScene(SelectMissionScene::createScene());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
				JniMethodInfo mInfo;
				jobject jobj;

				bool isHave = JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
				if(!isHave)
				{
					log("jni->%s/getInstance:not Exsit", "org/cocos2dx/cpp/AppActivity");
				}
				else
				{
					log("jni->%s/getInstance: Exsit", "org/cocos2dx/cpp/AppActivity");
					jobj = mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID);
				}

				isHave = JniHelper::getMethodInfo(mInfo, "org/cocos2dx/cpp/AppActivity", "payForCheat", "()V");
				if(!isHave)
				{
					log("jni->%s/payForCheat:not Exsit", "org/cocos2dx/cpp/AppActivity");
				}
				else
				{
					mInfo.env->CallVoidMethod(jobj, mInfo.methodID);
				}

#endif

		});

		Menu *menu = Menu::create(selectItem, repeatItem, cheatItem, shareItem, NULL);
		menu->alignItemsHorizontallyWithPadding(10);
		menu->setPosition(320, 40);
		this->addChild(menu, 10);

		Sprite *bg = Sprite::create("main.png");
		bg->setAnchorPoint(Vec2::ZERO);
		//bg->setPosition(Vec2(0, visiableSize.height - bg->getContentSize().height));
		this->addChild(bg, 0);

		//set Timer
		this->schedule(schedule_selector(GameMainScene::changeTime), 1.0f);

		return true;
}

GameMainScene * GameMainScene::create(int idx, bool isContinue) {
	GameMainScene *pRet = new GameMainScene();
	if (pRet && pRet->initWithMissionIdx(idx, isContinue)) {
		pRet->autorelease();
		return pRet;
	} else {
		delete pRet;
		pRet = NULL;
		return pRet;
	}
}

//�ж��佫�ܳ��ĸ������ƶ�
CanMovedDirection GameMainScene::checkMoveDirection(Sprite *sprite) {
	//CCASSERT(sprite->getParent() == this, "Error: the sprite must be this layer's child");

	CanMovedDirection flag = CanMovedDirection::UNMOVEABLE;
	Rect emptyRect_1 = _emptyCells.at(0);
	Rect emptyRect_2 = _emptyCells.at(1);
	Rect rect = sprite->getBoundingBox();

	if (!rect.intersectsRect(emptyRect_1) && !rect.intersectsRect(emptyRect_1)) //�������û�к��κ�һ���տ�����
		return flag;

	float emptyMidX_1 = emptyRect_1.getMidX();
	float emptyMidX_2 = emptyRect_2.getMidX();
	float emptyMidY_1 = emptyRect_1.getMidY();
	float emptyMidY_2 = emptyRect_2.getMidY();

	Vect emptyMidPoint_1 = Vect(emptyMidX_1, emptyMidY_1); //�����տ���е�
	Vect emptyMidPoint_2 = Vect(emptyMidX_2, emptyMidY_2);

	Rect leftZoom = Rect(rect.getMinX() - CELL_WIDTH, rect.getMinY(),
		rect.size.width, rect.size.height);
	Rect rightZoom = Rect(rect.getMinX() + CELL_WIDTH, rect.getMinY(),
		rect.size.width, rect.size.height);
	Rect topZoom = Rect(rect.getMinX(), rect.getMinY() + CELL_HEIGHT,
		rect.size.width, rect.size.height);
	Rect bottomZoom = Rect(rect.getMinX(), rect.getMinY() - CELL_HEIGHT,
		rect.size.width, rect.size.height);

	int tag = sprite->getTag();

	switch (tag) {
	case HeroType::CAO_CAO:
		if (leftZoom.containsPoint(emptyMidPoint_1)
			&& leftZoom.containsPoint(emptyMidPoint_2))
			flag = CanMovedDirection::MOVEABLE_LEFT;

		else if (rightZoom.containsPoint(emptyMidPoint_1)
			&& rightZoom.containsPoint(emptyMidPoint_2))
			flag = CanMovedDirection::MOVEABLE_RIGHT;

		else if (topZoom.containsPoint(emptyMidPoint_1)
			&& topZoom.containsPoint(emptyMidPoint_2))
			flag = CanMovedDirection::MOVEABLE_UP;

		else if (bottomZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2))
			flag = CanMovedDirection::MOVEABLE_UP;
		break;

	case HeroType::GUAN_YU_H: //���ں����佫
	case HeroType::HUANG_ZHONG_H:
	case HeroType::ZHANG_FEI_H:
	case HeroType::ZHAO_YUN_H:
	case HeroType::MA_CHAO_H:
		if ((leftZoom.containsPoint(emptyMidPoint_1)
			|| leftZoom.containsPoint(emptyMidPoint_2)) //�����߰�������һ���տ����ұ�һ����������
			&& !(rightZoom.containsPoint(emptyMidPoint_1)
			|| rightZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_LEFT;

		else if ((rightZoom.containsPoint(emptyMidPoint_1)
			|| rightZoom.containsPoint(emptyMidPoint_2)) //����ұ߰�������һ���տ������һ����������
			&& !(leftZoom.containsPoint(emptyMidPoint_1)
			|| leftZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_RIGHT;

		else if ((leftZoom.containsPoint(emptyMidPoint_1)
			&& rightZoom.containsPoint(emptyMidPoint_2)) //������Ҹ�����һ���տ�
			|| (leftZoom.containsPoint(emptyMidPoint_2)
			&& rightZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_LEFT_RIGHT;

		else if (topZoom.containsPoint(emptyMidPoint_1)
			&& topZoom.containsPoint(emptyMidPoint_2)) //����Ϸ����������տ�
			flag = CanMovedDirection::MOVEABLE_UP;

		else if (bottomZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2)) //����·������������տ�
			flag = CanMovedDirection::MOVEABLE_DOWN;
		break;

	case HeroType::GUAN_YU_V: //���ں����佫
	case HeroType::HUANG_ZHONG_V:
	case HeroType::ZHANG_FEI_V:
	case HeroType::ZHAO_YUN_V:
	case HeroType::MA_CHAO_V:
		if ((topZoom.containsPoint(emptyMidPoint_1)
			|| topZoom.containsPoint(emptyMidPoint_2)) //����Ϸ���������һ���տ����ұ�һ����������
			&& !(bottomZoom.containsPoint(emptyMidPoint_1)
			|| bottomZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_UP;

		else if ((bottomZoom.containsPoint(emptyMidPoint_1)
			|| bottomZoom.containsPoint(emptyMidPoint_2)) //����·���������һ���տ����ϱ�һ����������
			&& !(topZoom.containsPoint(emptyMidPoint_1)
			|| topZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_DOWN;

		else if ((topZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2)) //������¸�����һ���տ�
			|| (topZoom.containsPoint(emptyMidPoint_2)
			&& bottomZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_UP_DOWN;

		else if (leftZoom.containsPoint(emptyMidPoint_1)
			&& leftZoom.containsPoint(emptyMidPoint_2)) //����󷽰��������տ�
			flag = CanMovedDirection::MOVEABLE_LEFT;

		else if (rightZoom.containsPoint(emptyMidPoint_1)
			&& rightZoom.containsPoint(emptyMidPoint_2)) //����ҷ����������տ�
			flag = CanMovedDirection::MOVEABLE_RIGHT;
		break;

	case HeroType::XIAO_BING_1: //����С��
	case HeroType::XIAO_BING_2:
	case HeroType::XIAO_BING_3:
	case HeroType::XIAO_BING_4:
	case HeroType::XIAO_BING_5:
	case HeroType::XIAO_BING_6:
		if ((leftZoom.containsPoint(emptyMidPoint_1)
			&& rightZoom.containsPoint(emptyMidPoint_2)) || //���Ҹ���һ���տ�
			(leftZoom.containsPoint(emptyMidPoint_2)
			&& rightZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_LEFT_RIGHT;

		else if ((topZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2)) || //���¸���һ���տ�
			(topZoom.containsPoint(emptyMidPoint_2)
			&& bottomZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_UP_DOWN;

		else if ((leftZoom.containsPoint(emptyMidPoint_1)
			&& topZoom.containsPoint(emptyMidPoint_2)) || //���ϸ���һ���տ�
			(leftZoom.containsPoint(emptyMidPoint_2)
			&& topZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_LEFT_UP;

		else if ((leftZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2)) || //���¸���һ���տ�
			(leftZoom.containsPoint(emptyMidPoint_2)
			&& bottomZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_LEFT_DOWN;

		else if ((rightZoom.containsPoint(emptyMidPoint_1)
			&& topZoom.containsPoint(emptyMidPoint_2)) || //���ϸ���һ���տ�
			(rightZoom.containsPoint(emptyMidPoint_2)
			&& topZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_RIGHT_UP;

		else if ((rightZoom.containsPoint(emptyMidPoint_1)
			&& bottomZoom.containsPoint(emptyMidPoint_2)) || //���¸���һ���տ�
			(rightZoom.containsPoint(emptyMidPoint_2)
			&& bottomZoom.containsPoint(emptyMidPoint_1)))
			flag = CanMovedDirection::MOVEABLE_RIGHT_DOWN;

		else if ((topZoom.containsPoint(emptyMidPoint_1)
			|| topZoom.containsPoint(emptyMidPoint_2)) //����Ϸ���������һ���տ����ұ�һ����������
			&& !(bottomZoom.containsPoint(emptyMidPoint_1)
			|| bottomZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_UP;

		else if ((bottomZoom.containsPoint(emptyMidPoint_1)
			|| bottomZoom.containsPoint(emptyMidPoint_2)) //����·���������һ���տ����ϱ�һ����������
			&& !(topZoom.containsPoint(emptyMidPoint_1)
			|| topZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_DOWN;

		else if ((leftZoom.containsPoint(emptyMidPoint_1)
			|| leftZoom.containsPoint(emptyMidPoint_2)) //�����߰�������һ���տ����ұ�һ����������
			&& !(rightZoom.containsPoint(emptyMidPoint_1)
			|| rightZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_LEFT;

		else if ((rightZoom.containsPoint(emptyMidPoint_1)
			|| rightZoom.containsPoint(emptyMidPoint_2)) //����ұ߰�������һ���տ������һ����������
			&& !(leftZoom.containsPoint(emptyMidPoint_1)
			|| leftZoom.containsPoint(emptyMidPoint_2)))
			flag = CanMovedDirection::MOVEABLE_RIGHT;
		break;

	default:
		break;
	}

	return flag;

}

//ˢ��ÿ���佫��λ����Ϣ
void GameMainScene::refreshLayoutInfo() {
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMS; ++j)
			_layoutInfo.flags[i][j] = HeroType::NO_HERO;

	//���»�ȡÿ���佫��λ����Ϣ
	for (auto i = _heros.begin(); i != _heros.end(); ++i) {
		Sprite *sprite = *i;

		HeroType type = static_cast<HeroType>(sprite->getTag());
		Rect rect = sprite->getBoundingBox();
		int x = static_cast<int>((rect.getMinX() - 18) / CELL_WIDTH);
		int y = static_cast<int>((rect.getMinY() - 108) / CELL_HEIGHT);

		_layoutInfo.flags[y][x] = type;

		if (rect.size.width > CELL_WIDTH * 1.5f
			&& rect.size.height > CELL_HEIGHT * 1.5f) {
				_layoutInfo.flags[y + 1][x] = type;
				_layoutInfo.flags[y][x + 1] = type;
				_layoutInfo.flags[y + 1][x + 1] = type;
				continue;
		}
		if (rect.size.width > CELL_WIDTH * 1.5f)
			_layoutInfo.flags[y][x + 1] = type;
		if (rect.size.height > CELL_HEIGHT * 1.5f)
			_layoutInfo.flags[y + 1][x] = type;
	}

	//���¿ո��ӵ�λ��
	_emptyCells.clear();
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMS; ++j) {
			if (_layoutInfo.flags[i][j] == HeroType::NO_HERO) {
				Rect rect(j * CELL_WIDTH + 20, i * CELL_HEIGHT + 110,
					CELL_WIDTH, CELL_HEIGHT);
				_emptyCells.push_back(rect);
			}
		}

}

//����ʱ��Ͳ���
void GameMainScene::changeTime(float dt) {
	_userData.usedTime += 1;
	String str;
	int hour = _userData.usedTime / 3600;
	int min = (_userData.usedTime % 3600) / 60;
	int sec = _userData.usedTime - hour * 3600 - min * 60;

	str.initWithFormat("%2d:%2d:%2d", hour, min, sec);
	static_cast<LabelTTF *>(this->getChildByTag(500))->setString(str.getCString());

	str.initWithFormat(_strStep.c_str(), _userData.step);
	static_cast<LabelTTF *>(this->getChildByTag(600))->setString(str.getCString());

}

//ʹ�ÿ�������
void GameMainScene::cheat(Ref *pRef) {

// 	_currentCheatStep = 0;
// 
// 	GameManager::getInstance()->getHeroLayoutInfoByIdx(_userData.index - 1,
// 		_layoutInfo); //�ָ�ԭʼ����
// 	resetHerosPosition();
// 		
// 	Layer *shadeLayer = Layer::create();
// 	Rect rect = static_cast<Sprite *>(this->getChildByTag(
// 		_layoutInfo.solutions[_currentCheatStep].type))->getBoundingBox();
// 
// 	static_cast<Sprite *>(this->getChildByTag(
// 		_layoutInfo.solutions[_currentCheatStep].type))->runAction(RepeatForever::create(Blink::create(1.0f, 1)));	//��˸
// 	this->addChild(shadeLayer, 11);

#if 0
	ClippingNode *clip = ClippingNode::create(); //���òü��ڵ�
	clip->setInverted(false); //���õװ�ɼ�
	clip->setAlphaThreshold(0); //����͸����AlphaֵΪ0
	clip->setTag(800);
	this->addChild(clip, 10);

	auto shadeLayer = LayerColor::create(Color4B::WHITE);
	clip->addChild(shadeLayer); //�ڲü��ڵ����һ����ɫ��͸����
#endif
	

	//���òü�ģ��

	

#if 0
	Sprite * stencil =
		Sprite::createWithSpriteFrame(
		static_cast<Sprite *>(this->getChildByTag(
		_layoutInfo.solutions[_currentCheatStep].type))->getSpriteFrame());
	stencil->setPosition(
		static_cast<Sprite *>(this->getChildByTag(
		_layoutInfo.solutions[_currentCheatStep].type))->getPosition());
	clip->setStencil(stencil);
#endif

	//����¼�����
// 	EventListenerTouchOneByOne *listener = EventListenerTouchOneByOne::create();
// 	listener->onTouchBegan =
// 		[&](Touch *touch, Event *event) {
// 			//auto stencil = static_cast<Sprite *>( static_cast<ClippingNode *>(this->getChildByTag(800))->getStencil() );
// 			auto rect = static_cast<Sprite *>(this->getChildByTag(
// 				_layoutInfo.solutions[_currentCheatStep].type))->getBoundingBox();
// 			if(rect.containsPoint(touch->getLocation()) && _currentCheatStep < _layoutInfo.solutions.size())
// 			{
// 				auto hero = static_cast<Sprite *>(this->getChildByTag(_layoutInfo.solutions[_currentCheatStep].type));
// 				hero->stopAllActions();
// 				switch (_layoutInfo.solutions[_currentCheatStep].action) //�ж�Ӧ����δ���
// 				{
// 				case CanMovedDirection::MOVEABLE_LEFT: //����һ��
// 					hero->setPosition(hero->getPositionX() - CELL_WIDTH, hero->getPositionY());
// 					break;
// 				case CanMovedDirection::MOVEABLE_RIGHT:
// 					hero->setPosition(hero->getPositionX() + CELL_WIDTH, hero->getPositionY());
// 					break;
// 				case CanMovedDirection::MOVEABLE_UP:
// 					hero->setPosition(hero->getPositionX(), hero->getPositionY() + CELL_HEIGHT);
// 					break;
// 				case CanMovedDirection::MOVEABLE_DOWN:
// 					hero->setPosition(hero->getPositionX(), hero->getPositionY() - CELL_HEIGHT);
// 					break;
// 				default:
// 					break;
// 				}
// 
// 				_currentCheatStep++;
// 				_userData.step++;
// 
// 				//�ж��Ƿ�ʤ��
// 				if(this->_winCell.containsPoint(this->getChildByTag(HeroType::CAO_CAO)->getPosition()) ||
// 					_currentCheatStep == _layoutInfo.minStep)
// 				{
// 					this->getChildByTag(HeroType::CAO_CAO)->stopAllActions();
// 					this->scheduleOnce(SEL_SCHEDULE(&GameMainScene::winFunc), 1.0f);
// 
// 					return true;
// 				}
// 								
// 				static_cast<Sprite *>(this->getChildByTag(
// 					_layoutInfo.solutions[_currentCheatStep].type))->runAction(RepeatForever::create(Blink::create(1.0f, 1)));	//��˸
// 
// #if 0
// //��������ģ��
// 				hero = static_cast<Sprite *>(this->getChildByTag(_layoutInfo.solutions[_currentCheatStep].type));
// 				stencil = Sprite::createWithSpriteFrame(hero->getSpriteFrame());
// 				stencil->setPosition(hero->getPosition());
// 				static_cast<ClippingNode *>(this->getChildByTag(800))->setStencil(stencil);			
// 
// #endif
// 				
// 			}
// 			return true;
// 	};
// 	listener->setSwallowTouches(true);
// 	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,
// 		shadeLayer);
}

//�����佫�Ĳ���
void GameMainScene::resetHerosPosition() {

	auto heros = GameManager::getInstance()->getHeros();
	std::vector<HeroType> createdType;
	GameManager::getInstance()->getHeroLayoutInfoByIdx(_userData.index - 1,
		_layoutInfo);
	for (int i = 0; i < ROWS; ++i)
		for (int j = 0; j < COLUMS; ++j) {
			if (_layoutInfo.flags[i][j] == HeroType::NO_HERO) {
				Rect rect(j * CELL_WIDTH + 20, i * CELL_HEIGHT + 110,
					CELL_WIDTH, CELL_HEIGHT);
				_emptyCells.push_back(rect);
				continue;
			}

			if (std::find(createdType.begin(), createdType.end(),
				_layoutInfo.flags[i][j]) == createdType.end()) {
					createdType.push_back(_layoutInfo.flags[i][j]);

					Sprite *sprite = heros.at(_layoutInfo.flags[i][j]);

					sprite->setPosition(
						j * CELL_WIDTH + 20
						+ sprite->getContentSize().width / 2,
						i * CELL_HEIGHT + 110
						+ sprite->getContentSize().height / 2);

			}

		};
}

//ʤ�����еĶ���
void GameMainScene::winFunc(float dt) {

	//�Ƴ����ֲ�
	auto clip = this->getChildByTag(800);
	if (clip)
		clip->removeFromParent();

	//ֹͣ��ʱ�Ͳ���
	this->unscheduleAllSelectors();

	String str;
	int hour = _userData.usedTime / 3600;
	int min = (_userData.usedTime % 3600) / 60;
	int sec = _userData.usedTime - hour * 3600 - min * 60;

	str.initWithFormat(GET_STR("complate"), _userData.step, hour, min, sec);
	PopLayer *popLayer = PopLayer::create("message.png");
	popLayer->setTitle(GET_STR("prompt"));
	popLayer->setContentText(str.getCString());
	popLayer->addButton("next.png", "next.png",NULL,
		ButtonType::BT_NEXT_MISSION_OK);
	popLayer->addButton("share.png", "share.png", NULL,
		ButtonType::BT_NEXT_MISSION_SHARE);
	popLayer->setCallbackFunc(this,
		(SEL_CallFuncN) &GameMainScene::popLayerCallFunc);
	this->addChild(popLayer, 50);

	//����ؿ���ɶ�
	_userData.isDone = true;
	GameManager::getInstance()->saveMissionData(_userData);

	//�ı���һ�صĽ���״̬
	MissionInfo nextMision;
	nextMision.index = _userData.index + 1;
	nextMision.step = 0;
	nextMision.usedTime = 0;
	nextMision.isDone = false;
	nextMision.isLocked = false;
	GameManager::getInstance()->saveMissionData(nextMision);

}

void GameMainScene::popLayerCallFunc(Node *node) {
	MenuItem *item = dynamic_cast<MenuItem *>(node);
	if (item) {
		int tag = item->getTag();
		switch (tag) {
		case BT_CHEAT_OK: //OK
			cheat(NULL);
			GameManager::getInstance()->changeCheatCount(GameManager::getInstance()->getCheatCount() > 0 ? GameManager::getInstance()->getCheatCount() - 1 : 0);
			log("cheat count : %d", GameManager::getInstance()->getCheatCount());
			break;
		case BT_RESET_OK:
			resetHerosPosition();
			break;
		case BT_NEXT_MISSION_OK:
			{
				for(auto &child: _heros)
					child->removeFromParent();
				Director::getInstance()->pushScene(
					GameMainScene::createScene(_userData.index, false));
				break;
			}
		
		case BT_SELECT_MISSSION_OK:
			GameManager::getInstance()->SaveCurrentMission(_userData.index, _layoutInfo);
			Director::getInstance()->popToSceneStackLevel(2);
			break;
		case BT_BUY_OK:
		{
			log("Buy");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 

			JniMethodInfo mInfo;
			jobject jobj;

			bool isHave = JniHelper::getStaticMethodInfo(mInfo, "org/cocos2dx/cpp/AppActivity", "getInstance", "()Ljava/lang/Object;");
			if(!isHave)
			{
				log("jni->%s/getInstance:not Exsit", "org/cocos2dx/cpp/AppActivity");
			}
			else
			{
				log("jni->%s/getInstance: Exsit", "org/cocos2dx/cpp/AppActivity");
				jobj = mInfo.env->CallStaticObjectMethod(mInfo.classID, mInfo.methodID);
			}

			isHave = JniHelper::getMethodInfo(mInfo, "org/cocos2dx/cpp/AppActivity", "payForCheat", "()V");
			if(!isHave)
			{
				log("jni->%s/payForCheat:not Exsit", "org/cocos2dx/cpp/AppActivity");
			}
			else
			{
				mInfo.env->CallVoidMethod(jobj, mInfo.methodID);
			}



#endif
		}

			break;
		default:
			break;
		}
	}
}


extern "C"{

	void Java_org_cocos2dx_cpp_AppActivity_showPayResult(int resultCode)
	{
		log("enter Java_org_cocos2dx_cpp_AppActivity_showPayResult");
		char str[30];
		sprintf(str, "resultCode: %d", resultCode);
		//MessageBox("str", "test");
		switch (resultCode) {
		case 101:
			MessageBox("Success", "title");
			break;
		case 103:
			MessageBox("cancel", "title");
			break;
		case 102:
			MessageBox("failed", "title");
			break;
		default:
			break;
		}
	}

}

