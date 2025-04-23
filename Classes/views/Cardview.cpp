#include "Cardview.h"
#include "Gameconfig.h"

USING_NS_CC;

CardView* CardView::create(Card* card)
{
    CardView* cardView = new (std::nothrow) CardView();
    if (cardView && cardView->init(card)) {
        cardView->autorelease();
        return cardView;
    }
    CC_SAFE_DELETE(cardView);
    return nullptr;
}

bool CardView::init(Card* card)
{
    if (!Node::init())
    {
        return false;
    }

    _card = card;
    // �������Ʊ��������޸��Լ����ؾ���·��
    _background = Sprite::create("D:\\program_data\\cocos_work\\CardGame\\proj.win32\\card_general.png");
    if (_background)
    {
        _background->setAnchorPoint(Vec2(0.5f, 0.5f));
        this->addChild(_background);

        // ���ýڵ����ݴ�С
        this->setContentSize(_background->getContentSize());
    }
    // ���¿�����ͼ
    updateView();

    return true;
}

CardView::~CardView()
{
    // ������
}

void CardView::updateView()
{
    if (!_card) return;

    cleanupSprites();

    if (_card->isFaceUp()) {
        updateFaceUpCard();
    }
    else {
        updateFaceDownCard();
    }
}

void CardView::cleanupSprites()
{
    if (_numberSprite) {
        _numberSprite->removeFromParent();
        _numberSprite = nullptr;
    }

    if (_suitSprite) {
        _suitSprite->removeFromParent();
        _suitSprite = nullptr;
    }
}

void CardView::updateFaceUpCard()
{
    createNumberSprites();
    createSuitSprite();
    _background->setColor(Color3B::WHITE);
}

void CardView::updateFaceDownCard()
{
    // Set card back color
    _background->setColor(Color3B(200, 200, 200));
}

void CardView::createNumberSprites()
{
    std::string color = _card->isRed() ? "red" : "black";
    std::string numberText = getCardValueText();

    // �������־��飬���޸��Լ����ؾ���·��
    std::string bigNumberFile = "D:\\program_data\\cocos_work\\CardGame\\proj.win32\\big_" + color + "_" + numberText + ".png";
    _numberSprite = Sprite::create(bigNumberFile);
    if (_numberSprite) {
        _numberSprite->setPosition(Vec2(-5, -15));
        this->addChild(_numberSprite);
    }

    // �������־��飬���޸��Լ����ؾ���·��
    std::string smallNumberFile = "D:\\program_data\\cocos_work\\CardGame\\proj.win32\\small_" + color + "_" + numberText + ".png";
    _numberSprite = Sprite::create(smallNumberFile);
    if (_numberSprite) {
        _numberSprite->setPosition(Vec2(-50, 100));
        this->addChild(_numberSprite);
    }
}

std::string CardView::getCardValueText()
{
    switch (_card->getValue()) {
    case 1:  return "A";
    case 11: return "J";
    case 12: return "Q";
    case 13: return "K";
    default: return std::to_string(_card->getValue());
    }
}

void CardView::createSuitSprite()
{
    std::string suitFile = getSuitFilePath();
    _suitSprite = Sprite::create(suitFile);

    if (_suitSprite) {
        _suitSprite->setPosition(Vec2(50, 100));
        this->addChild(_suitSprite);
    }
}

//������ɫ���飬���޸��Լ����ؾ���·��
std::string CardView::getSuitFilePath()
{
    std::string basePath = "D:\\program_data\\cocos_work\\CardGame\\proj.win32\\";

    switch (_card->getSuit()) {
    case CST_CLUBS:    return basePath + "club.png";
    case CST_DIAMONDS: return basePath + "diamond.png";
    case CST_HEARTS:   return basePath + "heart.png";
    case CST_SPADES:   return basePath + "spade.png";
    default:           return basePath + "club.png";
    }
}

void CardView::onEnter()
{
    Node::onEnter();

    // ��Ӵ����¼�������
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);

        if (rect.containsPoint(locationInNode))
        {
            this->setScale(1.1f);  // ����ʱ�Ŵ�
            return true;
        }
        return false;
        };

    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        this->setScale(1.0f);  // �ָ�ԭʼ��С

        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size size = this->getContentSize();
        Rect rect = Rect(0, 0, size.width, size.height);

        if (rect.containsPoint(locationInNode))
        {
            if (_clickCallback)
            {
                _clickCallback(_card);
            }
        }
    };

    listener->onTouchCancelled = [this](Touch* touch, Event* event) {
        this->setScale(1.0f);  // �ָ�ԭʼ��С
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::onExit()
{
    // ��ֹͣ���ж���
    this->stopAllActions();

    // �Ƴ��¼�������
    _eventDispatcher->removeEventListenersForTarget(this);

    // ���ø����onExit
    Node::onExit();
}

Card* CardView::getCard() const
{
    return _card;
}

void CardView::setClickCallback(const std::function<void(Card*)>& callback)
{
    _clickCallback = callback;
}