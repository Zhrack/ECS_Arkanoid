
#include "CollisionDetector.h"

#include "BoxColliderComponent.h"
#include "CircleColliderComponent.h"
#include "TransformComponent.h"
#include "ComponentList.h"
#include "Message.h"

CollisionDetector::CollisionDetector()
{
    // register functions
    mFunctionMap[0] = CirclevsCircle;
    mFunctionMap[1] = AABBvsCircle;
    mFunctionMap[2] = AABBvsAABB;
}


CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::checkCollisions(const std::vector<BaseComponent*>& actors)
{
    // loop through
    for (size_t i = 0; i < actors.size(); ++i)
    {
        for (size_t j = i + 1; j < actors.size(); ++j)
        {
            BaseComponent* comp1 = actors[i];
            BaseComponent* comp2 = actors[j];

            if (comp1->isZombie() ||
                comp2->isZombie())
                continue;

            // check type of collider to choose the right algorithm
            int comp1Box = 0;
            int comp2Box = 0;

            if (dynamic_cast<BoxColliderComponent*>(comp1))
            {
                comp1Box++;
            }
            if (dynamic_cast<BoxColliderComponent*>(comp2))
            {
                comp2Box++;
            }

            auto func = mFunctionMap[comp1Box + comp2Box];

            sf::Vector2f amount;
            if (func(comp1, comp2, amount))
            {
                // collision!
                std::shared_ptr<CollisionData> data1 = std::make_shared<CollisionData>();
                data1->otherCollider = comp2;
                data1->otherType = comp2Box ? CompType::BOX_COLLIDER : CompType::CIRCLE_COLLIDER;
                data1->amount = amount;
                Message msg1(0, MessageType::MSG_COLLISION, data1);

                comp1->receive(msg1, SendType::IMMEDIATE);

                std::shared_ptr<CollisionData> data2 = std::make_shared<CollisionData>();
                data2->otherCollider = comp1;
                data2->otherType = comp1Box ? CompType::BOX_COLLIDER : CompType::CIRCLE_COLLIDER;
                data2->amount = amount;
                Message msg2(0, MessageType::MSG_COLLISION, data2);

                comp2->receive(msg2, SendType::IMMEDIATE);
            }
        }
    }
}

/// <summary>
/// AABB VS AABB check.
/// </summary>
/// <param name="box1">The BaseComponent 1.</param>
/// <param name="box2">The BaseComponent 2.</param>
/// <returns></returns>
bool AABBvsAABB(BaseComponent * box1, BaseComponent * box2, sf::Vector2f& amount)
{
    // since I'm sure about the type of the components, it's safe to use a static_cast
    BoxColliderComponent* b1 = static_cast<BoxColliderComponent*>(box1);
    BoxColliderComponent* b2 = static_cast<BoxColliderComponent*>(box2);

    sf::FloatRect rect1 = b1->getBounds();
    sf::FloatRect rect2 = b2->getBounds();
    sf::FloatRect intBox;
    if (rect1.intersects(rect2, intBox))
    {
        sf::Vector2f resolveVec;
        // search the direction vector to resolve the collision
        if (intBox.left == rect2.left) // left
        {
            resolveVec.x = -1.f * intBox.width;
        }
        if (intBox.top == rect2.top) // up
        {
            resolveVec.y = -1.f * intBox.height;
        }
        if (intBox.left + intBox.width == rect2.left + rect2.width) // right
        {
            resolveVec.x = 1.f * intBox.width;
        }
        if (intBox.top + intBox.height == rect2.left + rect2.height) // bottom
        {
            resolveVec.y = 1.f * intBox.height;
        }

        amount = resolveVec;
        return true;
    }

    return false;

    //sf::Vector2f pos1 = b1->getTransform()->getPosition();
    //sf::Vector2f pos2 = b2->getTransform()->getPosition();

    //sf::Vector2f size1 = b1->getSize();
    //sf::Vector2f size2 = b2->getSize();

    //if (!(  pos1.x < pos2.x + size2.x &&
    //        pos1.x + size1.x > pos2.x &&
    //        pos1.y < pos2.y + size2.y &&
    //        pos1.y + size1.y > pos2.y))
    //{
    //    return false;
    //}

    //float x1, x2, y1, y2;

    //if (pos1.x + size1.x > pos2.x) {
    //    x1 = pos1.x + size1.x - pos2.x;
    //}

    //if (pos1.x < pos2.x + size2.x) {
    //    x2 = pos2.x + size2.x - pos1.x;
    //}

    //if (pos1.y + size1.y > pos2.y) {
    //    y1 = pos1.y + size1.y - pos2.y;
    //}

    //if (pos1.y < pos2.y + size2.y) {
    //    y2 = pos2.y + size2.y - pos1.y;
    //}

    //amount.x = std::min(x1, x2);
    //amount.y = std::min(y1, y2);

    //return true;
}

bool AABBvsCircle(BaseComponent * box, BaseComponent * c, sf::Vector2f& amount)
{
    // since I'm sure about the type of the components, it's safe to use a static_cast
    BoxColliderComponent* b = static_cast<BoxColliderComponent*>(box);
    CircleColliderComponent* circle = static_cast<CircleColliderComponent*>(c);

    sf::Vector2f posBox = b->getTransform()->getPosition();
    sf::Vector2f posCircle = circle->getTransform()->getPosition();

    sf::Vector2f sizeBox = b->getSize();
    float radius = circle->getRadius();

    // center circle
    posCircle.x += radius;
    posCircle.y += radius;

    sf::Vector2f cPoint;

    cPoint.x = posCircle.x - std::max(posBox.x, std::min(posCircle.x, posBox.x + sizeBox.x));
    cPoint.y = posCircle.y - std::max(posBox.y, std::min(posCircle.y, posBox.y + sizeBox.y));

    if (cPoint.x * cPoint.x + cPoint.y * cPoint.y < radius * radius)
    {
        amount = cPoint;
        return true;
    }
    return false;
}

bool CirclevsCircle(BaseComponent * c1, BaseComponent * c2, sf::Vector2f& amount)
{
    amount;
    // since I'm sure about the type of the components, it's safe to use a static_cast
    CircleColliderComponent* circle1 = static_cast<CircleColliderComponent*>(c1);
    CircleColliderComponent* circle2 = static_cast<CircleColliderComponent*>(c2);

    sf::Vector2f posCircle1 = circle1->getTransform()->getPosition();
    sf::Vector2f posCircle2 = circle2->getTransform()->getPosition();

    float radius1 = circle1->getRadius();
    float radius2 = circle2->getRadius();

    float radiiSq = (radius1 + radius2) * (radius1 + radius2);
    float distCentersSq = (posCircle1.x - posCircle2.x) * (posCircle1.x - posCircle2.x) +
        (posCircle1.y - posCircle2.y) * (posCircle1.y - posCircle2.y);

    //if distance btw center of circles is less than sum of radii
    if (distCentersSq < radiiSq)
    {
        //coll!
        return true;
    }
    return false;
}