#include "AppDelegate.h"
#include "cocos-ext.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLView::create("Hound");
		//glview = GLView::createWithRect("Hound", Rect(0, 0, 1280, 960));
        director->setOpenGLView(glview);
		glview->setDesignResolutionSize(DataCenter::DESIGN_RESOLUTION.width, 
										DataCenter::DESIGN_RESOLUTION.height, 
										ResolutionPolicy::SHOW_ALL);
		director->setContentScaleFactor(
			DataCenter::RESOURCE_RESOLUTION.width / DataCenter::DESIGN_RESOLUTION.width);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	// load all game resources and information into caches
	if (!loadGameResources())
	{
		return false;
	}

	// Data Center load static data
	if (!DataCenter::getInstance()->loadStaticData())
	{
		return false;
	}

	// load player information from database
	if (!DataCenter::getInstance()->loadPlayerInfo(m_playerInfo))
	{
		return false;
	}

	// load specific level data from xml file
	if (!DataCenter::getInstance()->loadLevelInfo(1, m_levelInfo))
	{
		return false;
	}

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = GameScene::create(m_playerInfo, m_levelInfo);
	if (scene == nullptr)
	{
		return false;
	}

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

bool AppDelegate::loadGameResources(void)
{
	// add search pathes
	FileUtils::getInstance()->addSearchPath("backgrounds");
	FileUtils::getInstance()->addSearchPath("bullet");
	FileUtils::getInstance()->addSearchPath("fonts");

	TextureCache *tcache = Director::getInstance()->getTextureCache();
	tcache->addImage("img_bg_1.jpg");
	tcache->addImage("img_bg_2.jpg");
	tcache->addImage("img_bg_3.jpg");
	tcache->addImage("img_bg_4.jpg");
	tcache->addImage("img_bg_5.jpg");

	tcache->addImage("clouds.png");

	tcache->addImage("mplane.png");
	tcache->addImage("frontgun.png");
	tcache->addImage("leftgun.png");
	tcache->addImage("rightgun.png");

	tcache->addImage("n_boss.png");
	tcache->addImage("n1.png");
	tcache->addImage("n2.png");
	tcache->addImage("n3.png");
	tcache->addImage("bullet0.png");
	tcache->addImage("bullet1.png");
	tcache->addImage("bullet2.png");
	tcache->addImage("bullet3.png");
	tcache->addImage("bullet4.png");
	tcache->addImage("bullet5.png");
	tcache->addImage("bullet6.png");
	tcache->addImage("bullet7.png");
	tcache->addImage("bullet8.png");
	tcache->addImage("bullet9.png");
	tcache->addImage("bullet10.png");
	tcache->addImage("bullet11.png");
	tcache->addImage("bullet12.png");
	tcache->addImage("bullet13.png");
	tcache->addImage("bullet14.png");
	tcache->addImage("bullet15.png");
	tcache->addImage("bullet16.png");
	tcache->addImage("bullet17.png");
	tcache->addImage("bullet18.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wsparticle_p01.plist");

	AnimationCache::getInstance()->addAnimationsWithFile("pig.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("nplane.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("bullet.plist");

	return true;
}