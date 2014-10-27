#ifndef GAME_MAIN_SCENE_H_
#define GAME_MAIN_SCENE_H_

#include "cocos2d.h"
#include "GameMacros.h"
#include "data/UserData.h"

USING_NS_CC;

class GameMainScene : public Layer
{
public:

	GameMainScene();


	virtual bool init();
	virtual bool initWithMissionIdx(int idx, bool isContinue);

	CREATE_FUNC(GameMainScene);
	static GameMainScene * create(int idx, bool isContinue);

public:
	static Scene * createScene(int idx, bool isContinue);

private:
	CanMovedDirection checkMoveDirection(Sprite *sprite);
	void refreshLayoutInfo();
	void changeTime(float dt);
	void cheat(Ref *pRef);
	void resetHerosPosition();
	void winFunc(float dt);

	void popLayerCallFunc(Node *);

private:

	std::vector<Rect> _emptyCells;			//�ո��ӵ�λ��
	Vector<Sprite *> _heros;				//�佫����
	Rect _winCell;							//�ж�ʤ���ľ�������
			
	Vec2 _beginPos;							//������ʼ��
	Vec2 _currentPos;						//��������佫�ĵ�ǰλ��
		
	CanMovedDirection _currentDirection;	//��������佫��ǰ���ƶ��ķ���
	HeroLayoutInfo _layoutInfo;				//�佫��������
	
	MissionInfo _userData;					//�û����� ��������ʱ��ȣ�	

	int _currentCheatStep;					//�����ؼ�ʱ���е��ڼ���

	const std::string _strStep;
};

#endif // GAME_MAIN_SCENE_H_
