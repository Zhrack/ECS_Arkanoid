
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
                CollisionData data1;
                data1.otherCollider = comp2;
                data1.otherType = comp2Box ? CompType::BOX_COLLIDER : CompType::CIRCLE_COLLIDER;
                data1.amount = amount;
                Message msg1(0, MessageType::MSG_COLLISION, &data1);

                comp1->receive(msg1, SendType::IMMEDIATE);

                CollisionData data2;
                data2.otherCollider = comp1;
                data2.otherType = comp1Box ? CompType::BOX_COLLIDER : CompType::CIRCLE_COLLIDER;
                data2.amount = amount;
                Message msg2(0, MessageType::MSG_COLLISION, &data2);

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

    sf::Vector2f pos1 = b1->getTransform()->getPosition();
    sf::Vector2f pos2 = b2->getTransform()->getPosition();

    sf::Vector2f size1 = b1->getSize();
    sf::Vector2f size2 = b2->getSize();

    if (!(  pos1.x < pos2.x + size2.x &&
            pos1.x + size1.x > pos2.x &&
            pos1.y < pos2.y + size2.y &&
            pos1.y + size1.y > pos2.y))
    {
        return false;
    }

    float x1, x2, y1, y2;

    if (pos1.x + size1.x > pos2.x) {
        x1 = pos1.x + size1.x - pos2.x;
    }

    if (pos1.x < pos2.x + size2.x) {
        x2 = pos2.x + size2.x - pos1.x;
    }

    if (pos1.y + size1.y > pos2.y) {
        y1 = pos1.y + size1.y - pos2.y;
    }

    if (pos1.y < pos2.y + size2.y) {
        y2 = pos2.y + size2.y - pos1.y;
    }

    amount.x = std::min(x1, x2);
    amount.y = std::min(y1, y2);

    return true;
}

bool AABBvsCircle(BaseComponent * box, BaseComponent * c, sf::Vector2f& amount)
{
    amount;
    // since I'm sure about the type of the components, it's safe to use a static_cast
    BoxColliderComponent* b = static_cast<BoxColliderComponent*>(box);
    CircleColliderComponent* circle = static_cast<CircleColliderComponent*>(c);

    sf::Vector2f posBox = b->getTransform()->getPosition();
    sf::Vector2f posCircle = circle->getTransform()->getPosition();

    sf::Vector2f sizeBox = b->getSize();
    float radius = circle->getRadius();
    float radiusSq = radius * radius;

    sf::Vector2f cPoint;

    if (posCircle.x + radius < posBox.x)//left of B
    {
        cPoint.x = posBox.x - radius;
    }
    else if (posCircle.x - radius > posBox.x + sizeBox.x)//right of B
    {
        cPoint.x = posBox.x + sizeBox.x + radius;
    }
    else//inside B
    {
        cPoint.x = posCircle.x;
    }

    if (posCircle.y + radius < posBox.y)//up of B
    {
        cPoint.y = posBox.y - radius;
    }
    else if (posCircle.y - radius > posBox.y + sizeBox.y)
    {
        cPoint.y = posBox.y + sizeBox.y + radius;
    }
    else
    {
        cPoint.y = posCircle.y;
    }

    float distSq = (cPoint.x - posCircle.x) * (cPoint.x - posCircle.x) + 
                    (cPoint.y - posCircle.y) * (cPoint.y - posCircle.y);



    if(distSq < radiusSq)
    {
        sf::Vector2f penetrationDir = (cPoint - posCircle);
        float penetrationDirLength = (penetrationDir.x * penetrationDir.x +
            penetrationDir.y * penetrationDir.y); 
        penetrationDir /= penetrationDirLength;

        amount = penetrationDir * std::sqrtf(radiusSq - distSq);
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

    //if distance btw center of circles is less than sum of radii
    if ((posCircle1.x - posCircle2.x) * (posCircle1.x - posCircle2.x) + 
        (posCircle1.y - posCircle2.y) * (posCircle1.y - posCircle2.y) < radiiSq)
    {
        //coll!
        return true;
    }
    return false;
}