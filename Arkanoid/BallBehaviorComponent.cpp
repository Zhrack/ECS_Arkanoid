
#include "BallBehaviorComponent.h"

#include "GameState.h"
#include "CircleColliderComponent.h"
#include "PaddleBehaviorComponent.h"
#include "BoxColliderComponent.h"

#include "TransformComponent.h"

#include "AudioService.h"
#include "ServiceLocator.h"


BallBehaviorComponent::BallBehaviorComponent(EntityID entityID, GameState* game, float velocity, const sf::Vector2f& pos) :
    BaseComponent(entityID, game),
    mVelocity(sf::Vector2f()),
    mMaxVelocity(velocity),
    mLocked(false),
    mState(BallState::BALL_NORMAL)
{
    mWindow = mGame->getWindow();

    mCollider = mGame->getComponent<CircleColliderComponent>(CompType::CIRCLE_COLLIDER, getEntityID());

    std::function<void(const CollisionData& data)> cb = std::bind(&BallBehaviorComponent::onCollisionCb, this, std::placeholders::_1);

    mCollider->setOnCollision(cb);

    mPaddleBehaviorComp = mGame->getPaddleComponent();
    mPaddleCollider = mGame->getComponent<BoxColliderComponent>(CompType::BOX_COLLIDER, mPaddleBehaviorComp->getEntityID());

    mTransform->setPosition(pos);

    sf::SoundBuffer* buffer = ServiceLocator::getAudioService()->getSound(SoundID::BALL_HIT_WALL);
    mHitSound.setBuffer(*buffer);

    buffer = ServiceLocator::getAudioService()->getSound(SoundID::BALL_HIT_BRICK);
    mHitBrickSound.setBuffer(*buffer);
}


BallBehaviorComponent::~BallBehaviorComponent()
{
}

void BallBehaviorComponent::update(float elapsed)
{
    const sf::RectangleShape& walls = mGame->getWalls();

    sf::Vector2f gameAreaSize(walls.getSize());
    sf::Vector2f gameAreaPos(walls.getPosition());

    if (mState == BallState::BALL_NORMAL)
    {
        mTransform->move(mVelocity * elapsed);

        float radius = mCollider->getRadius();

        if (mTransform->getPosition().x < gameAreaPos.x)
        {
            mTransform->setPosition(gameAreaPos.x, mTransform->getPosition().y);
            mVelocity.x = -mVelocity.x;

            if (mHitSound.getStatus() != sf::SoundSource::Status::Playing)
                mHitSound.play();
        }
        else if (mTransform->getPosition().x + radius > gameAreaPos.x + gameAreaSize.x - mCollider->getRadius())
        {
            mTransform->setPosition(gameAreaPos.x + gameAreaSize.x - mCollider->getRadius() - radius, mTransform->getPosition().y);
            mVelocity.x = -mVelocity.x;

            if (mHitSound.getStatus() != sf::SoundSource::Status::Playing)
                mHitSound.play();
        }

        if (mTransform->getPosition().y < gameAreaPos.y)
        {
            mTransform->setPosition(mTransform->getPosition().x, gameAreaPos.y);
            mVelocity.y = -mVelocity.y;

            if(mHitSound.getStatus() != sf::SoundSource::Status::Playing)
                mHitSound.play();
        }
        else if (mTransform->getPosition().y > gameAreaPos.y + gameAreaSize.y)
        {
            //mTransform->setPosition(mTransform->getPosition().x, gameAreaPos.y + gameAreaSize.y - mCollider->getRadius() - radius);
            //mVelocity.y = -mVelocity.y;

            // ball is OUT!
            mGame->destroyEntity(getEntityID());

            Message ballLostMsg(getEntityID(), MessageType::MSG_BALL_LOST);
            mGame->sendMessage(EntityType::TAG_GAME_OVER_WATCHER, CompType::GAME_OVER_WATCHER, ballLostMsg, SendType::DELAYED, sf::seconds(0.1f));
        }

        if (std::fabsf(mVelocity.y) < 20.f)
        {
            if (mVelocity.y < 0.f)
                mVelocity.y = -70.f;
            else
                mVelocity.y = 70.f;
        }
    }
    else if(mState == BallState::BALL_FOLLOW_PADDLE)
    {
        auto paddleTransform = mPaddleBehaviorComp->getTransform();
        sf::Vector2f pos(paddleTransform->getPosition());
        pos.y -= mCollider->getRadius() * 2.f;
        pos.x += (mPaddleCollider->getSize().x * 0.5f) - mCollider->getRadius();
        mTransform->setPosition(pos);
    }
}

void BallBehaviorComponent::onCollisionCb(const CollisionData & data)
{
    EntityType otherType = mGame->getEntityType(data.otherCollider->getEntityID());
    if (otherType == EntityType::TAG_PLAYER)
    {
        // here friction is simulated to give some degree of control to the player,
        // so that is possible to change the direction of the ball with the paddle
        mVelocity.x += (mPaddleBehaviorComp->getFriction() * mPaddleBehaviorComp->getCurrentVelocity().x);
        sf::Vector2f mVelUnit = mVelocity / std::sqrtf((mVelocity.x * mVelocity.x) + (mVelocity.y * mVelocity.y));
        mVelocity = mVelUnit * mMaxVelocity;

        if (!mLocked && mPaddleBehaviorComp->isSticky())
        {
            lockBall();
        }
    }

    mTransform->move(data.amount);
    //std::cout << "AmX: " << data.amount.x << std::endl << "AmY: " << data.amount.y << std::endl;

    if (std::fabs(mVelocity.y) < 60.f)
    {
        mVelocity.y = -60.f;
    }

    if (std::fabs(data.amount.x) > 0.01f)
    {
        // collision on the horizontal direction
        mVelocity.x = -mVelocity.x;
    }

    if (std::fabs(data.amount.y) > 0.01f)
    {
        // collision on the vertical direction
        mVelocity.y = -mVelocity.y;
    }

    if (mHitBrickSound.getStatus() != sf::SoundSource::Status::Playing && 
        (otherType == EntityType::TAG_BRICK ||
        otherType == EntityType::TAG_PLAYER) )
        mHitBrickSound.play();

}

void BallBehaviorComponent::releaseBall()
{
    if (!mLocked) return;

    changeState(BallState::BALL_NORMAL);
    
    float dir = mPaddleBehaviorComp->getCurrentVelocity().x > 0 ? 1.f : -1.f;
    mVelocity = sf::Vector2f(dir * mMaxVelocity, -mMaxVelocity);
    mTransform->move(0.f, -5.f);
    mLocked = false;
}

void BallBehaviorComponent::lockBall()
{
    if (mLocked) return;
    std::cout << "LOCK" << std::endl;
    changeState(BallState::BALL_FOLLOW_PADDLE);
    mVelocity = sf::Vector2f();
    mLocked = true;
}

void BallBehaviorComponent::handleMessage(Message & msg)
{

    switch (msg.mType)
    {
    case MSG_RELEASE_BALL:
        if(mState == BallState::BALL_FOLLOW_PADDLE)
            releaseBall();
        break;
    }
}

void BallBehaviorComponent::changeState(BallState newState)
{
    mState = newState;
}

void BallBehaviorComponent::setVelocity(const sf::Vector2f & vel)
{
    mVelocity = vel;
}
