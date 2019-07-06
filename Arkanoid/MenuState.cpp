#include "MenuState.h"

#include "World.h"
#include "GameState.h"

#include "ServiceLocator.h"
#include "PCAudioService.h"

#include <iostream>

MenuState::MenuState(World* world, pt::ptree& tree) :
    BaseState(world),
    mTree(tree),
    mBackground(sf::PrimitiveType::Quads, 4)
{
}


MenuState::~MenuState()
{
}

void MenuState::enter()
{
    try
    {
        auto audioService = new PCAudioService();

        std::string sfxFolder = mTree.get<std::string>("AUDIO_FOLDER");
        std::string musicFolder = mTree.get<std::string>("MUSIC_FOLDER");
        std::vector<SoundIDFilename> sounds;
        std::vector<MusicIDFilename> musics(
            {
                {MusicID::MUSIC_MENU, mTree.get<std::string>("MENU_MUSIC")}
            });

        if (!audioService->initialize(sfxFolder, musicFolder, sounds, musics))
        {
            throw "Error initializing the audio service";
        }

        ServiceLocator::provide(audioService);

        ServiceLocator::getAudio()->playMusic(MusicID::MUSIC_MENU);

        std::string filename = mTree.get<std::string>("BACKGROUND");

        if (!mBackgroundTexture.loadFromFile(filename))
        {
            throw "Error loading background image " + filename;
        }

        float screenSizeX = mTree.get<float>("SCREEN_WIDTH");
        float screenSizeY = mTree.get<float>("SCREEN_HEIGHT");

        sf::Vector2u texSize = mBackgroundTexture.getSize();

        mBackground[0].position = sf::Vector2f(0.f, 0.f);
        mBackground[1].position = sf::Vector2f(screenSizeX, 0.f);
        mBackground[2].position = sf::Vector2f(screenSizeX, screenSizeY);
        mBackground[3].position = sf::Vector2f(0.f, screenSizeY);

        mBackground[0].texCoords = sf::Vector2f(0.f, 0.f);
        mBackground[1].texCoords = sf::Vector2f((float)texSize.x, 0.f);
        mBackground[2].texCoords = sf::Vector2f((float)texSize.x, (float)texSize.y);
        mBackground[3].texCoords = sf::Vector2f(0.f, (float)texSize.y);


        filename = mTree.get<std::string>("LOGO_IMAGE");
        if (!mLogoTexture.loadFromFile(filename))
        {
            throw "Error loading background image " + filename;
        }

        mLogo.setTexture(mLogoTexture);

        mLogo.setPosition(screenSizeX * 0.5f - mLogoTexture.getSize().x * 0.5f, screenSizeY * 0.5f - mLogoTexture.getSize().y * 0.5f - 100.f);

        if (!mFont.loadFromFile("resources/joystix monospace.ttf"))
        {
            throw "Error loading font";
        }

        mPressEnterText.setFont(mFont);
        mPressEnterText.setString("Press Enter to play");
        mPressEnterText.setPosition(
            screenSizeX * 0.5f - mPressEnterText.getGlobalBounds().width * 0.5f,
            screenSizeY * 0.5f + mPressEnterText.getGlobalBounds().height + 50.f);

        mInstructionText.setFont(mFont);
        mInstructionText.setString("Press Spacebar to fire the ball, A/D or arrows to control the paddle");
        mInstructionText.setCharacterSize(15);
        mInstructionText.setPosition(
            screenSizeX * 0.5f - mInstructionText.getGlobalBounds().width * 0.5f,
            screenSizeY - mPressEnterText.getGlobalBounds().height - mInstructionText.getGlobalBounds().height - 50.f
        );
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

void MenuState::update()
{
    sf::Event event;
    while (mWindow->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            mWindow->close();
            break;
        case sf::Event::KeyPressed:
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                mWorld->changeState(new GameState(mWorld, mTree));
                return;
            }
        }
    }

    mWindow->clear();

    mWindow->draw(mBackground, &mBackgroundTexture);
    mWindow->draw(mLogo);
    mWindow->draw(mPressEnterText);
    mWindow->draw(mInstructionText);

    mWindow->display();
}

void MenuState::exit()
{
    ServiceLocator::getAudio()->stopMusic(MusicID::MUSIC_MENU);
}
