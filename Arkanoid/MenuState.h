#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "BaseState.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

/// <summary>
/// This class manages the initial menu screen.
/// </summary>
/// <seealso cref="BaseState" />
class MenuState :
    public BaseState
{
public:
    MenuState(World* world, pt::ptree& tree);
    virtual ~MenuState();

    // Inherited via BaseState
    virtual void enter() override;
    virtual void update() override;
    virtual void exit() override;

private:
    sf::Font mFont;

    sf::Text mPressEnterText;
    sf::Text mInstructionText;

    pt::ptree& mTree;

    sf::Texture* mBackgroundTexture;
    sf::VertexArray  mBackground;
    sf::Sprite mLogo;
};


#endif // !MENU_STATE_H
