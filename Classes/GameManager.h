#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include "cocos2d.h"
#include "GameMacros.h"

USING_NS_CC;

class GameManager 
{

public:
	static GameManager * getInstance();
	static void freeInstance();

	Map<int, Sprite *> & getHeros() { return _heros; }
	void getHeroLayoutInfoByIdx(int idx, HeroLayoutInfo &info);
	std::vector<MissionInfo> &getMissionStatus(){return _mision_status;}

	void loadMissionData();
	void loadLayoutData();
	void saveMissionData(MissionInfo info);
	void SaveCurrentMission(int idx, HeroLayoutInfo &layout);
	void loadCurrentMission(HeroLayoutInfo & layout);
	void updateLayoutData(std::string filePath);

	int getCheatCount();
	void changeCheatCount(int);

	bool isFirstPlay();
	void setPlayed();

private:
	GameManager(void);
	~GameManager(void);

	void split(std::string &src, const char *deliem);

	bool init();

private:
	static GameManager *_manager;				//����ʵ��
	Map<int, Sprite *> _heros;					//ÿ���佫�ľ���ʵ��
	std::vector<std::string> _layoutInfoStrs;	//�������ļ���ȡ�� ����/����/�ⷨ �ַ�������
	std::vector<MissionInfo> _mision_status;	//ÿ���ؿ���Ϣ�ļ���

};


#endif // !_GAME_MANAGER_H_
