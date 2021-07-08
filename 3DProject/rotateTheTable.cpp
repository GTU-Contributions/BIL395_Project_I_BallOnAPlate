//
// Created by mpolatcan-gyte_cse on 17.11.2016.
//
#include <osgWidget/Canvas>
#include <osg/Object>
#include <osg/Node>
#include <osg/Transform>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/PositionAttitudeTransform>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgWidget/WindowManager>
#include <osgWidget/ViewerEventHandlers>
#include <iostream>

// TODO Keyboard handler for define position of target point
// TODO Integrate serial communication
// TODO Show numeric coordinates of ball (X,Y)

using namespace std;
using namespace osg;
using namespace osgViewer;
using namespace osgGA;
using namespace osgShadow;
using namespace osgDB;

// ------------------------------------ FUNCTIONS AND CALLBACKS -----------------------------
void setTextureSettings(Texture2D *shapeTexture,
                        Image *textureImage,
                        ref_ptr<ShapeDrawable> shape);
bool leftArrowCallback(osgWidget::Event& ev);
bool rightArrowCallback(osgWidget::Event& ev);
bool forwardArrowCallback(osgWidget::Event& ev);
bool backwardArrowCallback(osgWidget::Event& ev);
bool releaseCallback(osgWidget::Event& ev);
bool mouseOverCallback(osgWidget::Event& ev);
bool mouseLeaveCallback(osgWidget::Event& ev);
bool drawCircleCallback(osgWidget::Event& ev);
bool drawSquareCallback(osgWidget::Event& ev);
osgWidget::Label* createButton(const string& name,
                               const string& text,
                               float size,
                               const Vec4& color);
osgWidget::Window* setupArrows();
osgWidget::Window* createUIButton(const string& text,
                                 bool (*callback)(osgWidget::Event&),
                                 int originX,
                                 int originY,
                                 float textSize,
                                 float dimX,
                                 float dimY,
                                 const vector<Vec4*>& buttonStateColors);
void setCallbacks(osgWidget::Label* button,
                  bool (*callback)(osgWidget::Event&),
                  vector<Vec4*> buttonStateColors);
void createComponents();
void placeComponentsToScene();
void applyTexturesToComponents();
void applyShadows();
void placeCamera();
void setupWindowAndUI();
void createUIColors();
//-----------------------------------------------------------------------------------------

const int ReceivesShadowTraversalMask = 0x1;
const int CastsShadowTraversalMask = 0x2;

// ---------------------------- COMPONENTS ------------------------------------
ref_ptr<ShadowedScene> shadowedScene = new ShadowedScene;
ref_ptr<ShapeDrawable> table;
ref_ptr<ShapeDrawable> bottomTable;
ref_ptr<ShapeDrawable> ball;
ref_ptr<ShapeDrawable> stick;
ref_ptr<ShapeDrawable> target;

//mehmed and yunus was here
ref_ptr<ShapeDrawable> backingY;
ref_ptr<ShapeDrawable> backingX;
ref_ptr<ShapeDrawable> motorY;
ref_ptr<ShapeDrawable> motorX;
ref_ptr<ShapeDrawable> motorHeadY;// table'nin uzun kenarinda kalan
ref_ptr<ShapeDrawable> motorHeadX;
ref_ptr<ShapeDrawable> motorPropellerY;
ref_ptr<ShapeDrawable> motorPropellerX;
ref_ptr<ShapeDrawable> motorPropellerIgnoredY;
ref_ptr<ShapeDrawable> motorPropellerIgnoredX;
ref_ptr<ShapeDrawable> pushrodY;
ref_ptr<ShapeDrawable> pushrodX;

//


Group* scene;
Viewer viewer;
MatrixTransform *tableRotationMatrix = new MatrixTransform;
MatrixTransform *ballRotationMatrix = new MatrixTransform;
MatrixTransform *stickRotationMatrix = new MatrixTransform;
MatrixTransform *bottomTableRotationMatrix = new MatrixTransform;
PositionAttitudeTransform *ballPosition = new PositionAttitudeTransform();
PositionAttitudeTransform *targetPosition = new PositionAttitudeTransform();
//
MatrixTransform *pushrodRotationMatrix = new MatrixTransform;
//
vector<Vec4*> uiButtonStateColors;
vector<Vec4*> uiButtonBorderStateColors;
osgWidget::Window *circleBorder;
osgWidget::Window *squareBorder;
//-----------------------------------------------------------------------------


class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:

    KeyboardEventHandler()
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
    {
        switch (ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                switch (ea.getKey())
                {
                    case 'a':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This will be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()+0.35,oldPosition.y(),oldPosition.z()));

                        // Table rotation technique
                        Matrix rotationLeft;
                        // TODO This will be modified with gyroscope data
                        // lift up left side of table
                        rotationLeft.makeRotate(PI/180,0,0,-1);
                        // calc new height of left side of table
                        tableRotationMatrix->setMatrix(rotationLeft*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 's':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This will be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x(),oldPosition.y(),oldPosition.z()-0.35));

                        // Table rotation technique
                        Matrix rotationDown;
                        // TODO This will be modified with gyroscope data
                        // lift up down side of table
                        rotationDown.makeRotate(PI/180,-1,0,0);
                        // calc new height of down side of table
                        tableRotationMatrix->setMatrix(rotationDown*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 'd':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This will be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()-0.35,oldPosition.y(),oldPosition.z()));

                        // Table rotation technique
                        Matrix rotationRight;
                        // TODO This will be modified with gyroscope data
                        // lift up right side of table
                        rotationRight.makeRotate(PI/180,0,0,1);
                        // calc new height of down side of table
                        tableRotationMatrix->setMatrix(rotationRight*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 'w':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This will be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x(),oldPosition.y(),oldPosition.z()+0.35));

                        // Table rotation technique
                        Matrix rotationUp;
                        // TODO This will be modified with gyroscope data
                        // lift up up side of table
                        rotationUp.makeRotate(PI/180,1,0,0);
                        // calc new height of up side of table
                        tableRotationMatrix->setMatrix(rotationUp*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 'q':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This will be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()+0.35,oldPosition.y(),oldPosition.z()+0.35));

                        // Table rotation technique
                        Matrix rotationLeftCrossUp;
                        // TODO This will be modified with gyroscope data
                        // lift up left cross up of table
                        rotationLeftCrossUp.makeRotate(PI/180,1,0,-1);
                        // calc new height of left cross up side of table
                        tableRotationMatrix->setMatrix(rotationLeftCrossUp*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 'e':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This wil be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()-0.35,oldPosition.y(),oldPosition.z()+0.35));

                        // Table rotation technique
                        Matrix rotationRightCrossUp;
                        // TODO This will be modified with gyroscope data
                        // lift up right cross up of table
                        rotationRightCrossUp.makeRotate(PI/180,1,0,1);
                        // calc new height of right cross up side of file
                        tableRotationMatrix->setMatrix(rotationRightCrossUp*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;


                    case 'z':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This wil be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()+0.35,oldPosition.y(),oldPosition.z()-0.35));

                        // Table rotation technique
                        Matrix rotationLeftCrossDown;
                        // TODO This will be modified with gyroscope data
                        // lift up left cross down of table
                        rotationLeftCrossDown.makeRotate(PI/180,-1,0,-1);
                        // calc new height of left cross down side of file
                        tableRotationMatrix->setMatrix(rotationLeftCrossDown*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;

                    case 'c':
                    {
                        // New ball translation technique
                        Vec3d oldPosition = ballPosition->getPosition(); // get old position of ball
                        // TODO This wil be modified with touchscreen data
                        ballPosition->setPosition(Vec3d(oldPosition.x()-0.35,oldPosition.y(),oldPosition.z()-0.35));

                        // Table rotation technique
                        Matrix rotationRightCrossDown;
                        // TODO This will be modified with gyroscope data
                        // lift up right cross down of table
                        rotationRightCrossDown.makeRotate(PI/180,-1,0,1);
                        // calc new height of right cross down side of file
                        tableRotationMatrix->setMatrix(rotationRightCrossDown*tableRotationMatrix->getMatrix());

                        // Rolling a ball
                        Matrix ballRotation;
                        ballRotation.makeRotate(20*PI/180,Vec3d(0,1,0));
                        ballRotationMatrix->setMatrix(ballRotation*ballRotationMatrix->getMatrix());
                    }
                        break;
                }
            }
            default:
                break;

        }
        //return false to allow mouse manipulation
        return false;
    }
};


int main(int argc, char** argv) {
    createComponents();
    applyShadows();
    applyTexturesToComponents();
    placeComponentsToScene();
    placeCamera();
    createUIColors();
    setupWindowAndUI();

    return viewer.run();
}

void setupWindowAndUI() {
    ref_ptr<osgWidget::WindowManager> wm = new osgWidget::WindowManager(&viewer,800.0f,640.0f,0xf0000000);
    Camera* widgetCamera = wm->createParentOrthoCamera();

    wm->addChild(setupArrows());

    circleBorder = createUIButton("",NULL,145,582,12.0f,120,50,uiButtonBorderStateColors);
    wm->addChild(circleBorder);

    squareBorder = createUIButton("",NULL,275,582,12.0f,120,50,uiButtonBorderStateColors);
    wm->addChild(squareBorder);

    wm->addChild(createUIButton("Draw Square",drawSquareCallback,280,587,12.0f,110,40,uiButtonStateColors));
    wm->addChild(createUIButton("Draw Circle",drawCircleCallback,150,587,12.0f,110,40,uiButtonStateColors));

    wm->resizeAllWindows();

    scene->addChild(widgetCamera);

    viewer.setSceneData(scene);
    viewer.setUpViewInWindow(20,20,800,640);
    viewer.addEventHandler(new KeyboardEventHandler());
    viewer.addEventHandler(new osgWidget::MouseHandler(wm.get()));
    viewer.addEventHandler(new osgWidget::KeyboardHandler(wm.get()));
    viewer.addEventHandler(new osgWidget::ResizeHandler(wm.get(),widgetCamera));
    viewer.addEventHandler(new osgWidget::CameraSwitchHandler(wm.get(),widgetCamera));
}

void createUIColors() {
    uiButtonStateColors.push_back(new Vec4(0.7f,0.5f,0.5f,1.0f));
    uiButtonStateColors.push_back(new Vec4(0.5f,0.3f,0.3f,1.0f));
    uiButtonStateColors.push_back(new Vec4(0.6f,0.4f,0.4f,1.0f));
    uiButtonStateColors.push_back(new Vec4(0.5f,0.3f,0.3f,1.0f));

    uiButtonBorderStateColors.push_back(new Vec4(0.0,0.6,0.8,1.0f));
    uiButtonBorderStateColors.push_back(new Vec4(0.0,0.0,0.0,1.0f));
    uiButtonBorderStateColors.push_back(new Vec4(0.3,0.5,0.7,1.0f));
    uiButtonBorderStateColors.push_back(new Vec4(0.0,0.0,0.0,1.0f));
}

void createComponents() {
    table = new ShapeDrawable;
    ball = new ShapeDrawable;
    stick = new ShapeDrawable;
    bottomTable = new ShapeDrawable;
    target = new ShapeDrawable;

    //mehmed and yunus was here
    backingY = new ShapeDrawable;
    backingX = new ShapeDrawable;
    motorY = new ShapeDrawable;
    motorX = new ShapeDrawable;
    motorHeadY = new ShapeDrawable;
    motorHeadX = new ShapeDrawable;
    motorPropellerY = new ShapeDrawable;
    motorPropellerX = new ShapeDrawable;
    motorPropellerIgnoredY = new ShapeDrawable;
    motorPropellerIgnoredX = new ShapeDrawable;
    pushrodX = new ShapeDrawable;
    pushrodY = new ShapeDrawable;
    //


    scene = new Group();

    table->setShape(new Box(Vec3(0.0,0.0,0.0),30.0f,0.5f,20.0f));
    ball->setShape(new Sphere(Vec3(0.0,1.20f,0.0f),1.0f));
    stick->setShape(new Cylinder(Vec3(0.0f,0.0f,8.0f),1.2f,15.0f));
    bottomTable->setShape(new Box(Vec3(0.0,-15.0f,0.0f),40.0f,1.0f,30.0f));
    target->setShape(new Box(Vec3(0.0,0.30f,0.0),2.0f,0.1f,2.0f));
    //mehmed and yunus was here
    backingY->setShape(new Box(Vec3(0.0f,-12.0f,8.0f),3.75f,6.0f,2.5f));
    backingX->setShape(new Box(Vec3(-8.0f,-12.0f,0.0f),3.75f,6.0f,2.5f));
    motorY->setShape(new Box(Vec3(-1.5f,-7.5f,8.0f),2.0f,3.0f,2.5f));
    motorX->setShape(new Box(Vec3(-9.5f,-7.5f,0.0f),2.0f,3.0f,2.5f));
    motorHeadY->setShape(new Box(Vec3(-2.5f,-6.5f,8.0f),0.9f,0.4f,0.4f));
    motorHeadX->setShape(new Box(Vec3(-10.5f,-6.5f,0.0f),0.9f,0.4f,0.4f));
    motorPropellerY->setShape(new Box(Vec3(-3.0f,-6.5f,7.0f),0.2f,0.8f,5.0f));
    motorPropellerX->setShape(new Box(Vec3(-11.0f,-6.5f,-1.0f),0.2f,0.8f,5.0f));
    motorPropellerIgnoredY->setShape(new Box(Vec3(-3.0f,-6.5f,8.0f),0.2f,3.0f,0.8f));
    motorPropellerIgnoredX->setShape(new Box(Vec3(-11.0f,-6.5f,0.0f),0.2f,3.0f,0.8f));

    pushrodY->setShape(new Cylinder(Vec3(-3.0f,5.0f,3.8f),0.2f,8.0f));
    pushrodX->setShape(new Cylinder(Vec3(-11.0f,-3.0f,3.8f),0.2f,8.0f));

    pushrodRotationMatrix->addChild(pushrodY.get());
    pushrodRotationMatrix->addChild(pushrodX.get());
    Matrix rotatePushrod;
    rotatePushrod.makeRotate(90*PI/180,1,0,0);
    pushrodRotationMatrix->setMatrix(rotatePushrod*stickRotationMatrix->getMatrix());

    scene->addChild(pushrodRotationMatrix);

    //
}

void applyShadows() {
    // Shadowing effects on ball
    shadowedScene->setReceivesShadowTraversalMask(ReceivesShadowTraversalMask);
    shadowedScene->setCastsShadowTraversalMask(CastsShadowTraversalMask);
    ref_ptr<ShadowMap> sm = new ShadowMap;
    shadowedScene->setShadowTechnique(sm.get());

    scene->addChild(shadowedScene);

    table->setNodeMask(ReceivesShadowTraversalMask);
    ball->setNodeMask(CastsShadowTraversalMask);
}

void applyTexturesToComponents() {
    Image *ballImage = readImageFile("basketball_texture.bmp");
    Image *tableImage = readImageFile("screen_texture.bmp");
    Image *stickImage = readImageFile("stick_texture.bmp");
    Image *bottomTableImage = readImageFile("wood_texture.bmp");
    Image *targetImage = readImageFile("target_texture.bmp");

    //mehmed and yunus and alper was here
    Image *backingImage = readImageFile("backing.bmp");
    Image *motorImage = readImageFile("motor.bmp");
    Image *motorHeadImage = readImageFile("motorHead.bmp");
    Image *motorPropellerImage = readImageFile("motorPropeller.bmp");

    Texture2D *ballTexture = new Texture2D;
    Texture2D *tableTexture = new Texture2D;
    Texture2D *stickTexture = new Texture2D;
    Texture2D *bottomTableTexture = new Texture2D;
    Texture2D *targetTexture = new Texture2D;

    //mehmed and yunus and alper was here
    Texture2D *backingTexture = new Texture2D;
    Texture2D *motorTexture = new Texture2D;
    Texture2D *motorHeadTexture = new Texture2D;
    Texture2D *motorPropellerTexture = new Texture2D;
    //
    if (ballImage == NULL || tableImage == NULL || stickImage == NULL || bottomTableImage == NULL ||
        targetImage == NULL || backingImage == NULL || motorImage == NULL || motorHeadImage== NULL ||
            motorPropellerImage == NULL)
        cout << "Textures can't be loaded!" << endl;

    setTextureSettings(ballTexture,ballImage,ball);
    setTextureSettings(tableTexture,tableImage,table);
    setTextureSettings(stickTexture,stickImage,stick);
    setTextureSettings(bottomTableTexture,bottomTableImage,bottomTable);
    setTextureSettings(targetTexture,targetImage,target);

    //mehmed and yunus was here
    setTextureSettings(backingTexture,backingImage,backingY);
    setTextureSettings(backingTexture,backingImage,backingX);
    setTextureSettings(motorTexture,motorImage,motorY);
    setTextureSettings(motorTexture,motorImage,motorX);
    setTextureSettings(motorHeadTexture,motorHeadImage,motorHeadY);
    setTextureSettings(motorHeadTexture,motorHeadImage,motorHeadX);
    setTextureSettings(motorPropellerTexture,motorPropellerImage,motorPropellerY);
    setTextureSettings(motorPropellerTexture,motorPropellerImage,motorPropellerX);
    setTextureSettings(motorPropellerTexture,motorPropellerImage,motorPropellerIgnoredY);
    setTextureSettings(motorPropellerTexture,motorPropellerImage,motorPropellerIgnoredX);
    setTextureSettings(motorTexture,motorImage,pushrodY);
    setTextureSettings(motorTexture,motorImage,pushrodX);
    //

}

void placeComponentsToScene() {
    // Bottom table of mechanic system
    bottomTableRotationMatrix->addChild(bottomTable.get());
    Matrix rotateBottomTable;
    rotateBottomTable.makeRotate(180*PI/180,0,1,0);
    bottomTableRotationMatrix->setMatrix(rotateBottomTable*bottomTableRotationMatrix->getMatrix());
    scene->addChild(bottomTableRotationMatrix);
    // -------------------------------------------------

    // Stick of mechanic system
    stickRotationMatrix->addChild(stick.get());
    Matrix rotateStick;
    rotateStick.makeRotate(90*PI/180,1,0,0);
    stickRotationMatrix->setMatrix(rotateStick*stickRotationMatrix->getMatrix());
    scene->addChild(stickRotationMatrix);
    // -------------------------------------------------

    // Ball, table and target point components' placement in there. Ball and table
    // collision detection applied.
    ballRotationMatrix->addChild(ball.get());
    tableRotationMatrix->addChild(table.get());
    tableRotationMatrix->addChild(ballPosition);
    tableRotationMatrix->addChild(targetPosition);
    ballPosition->setPosition(Vec3(0.0,0.0,0.0));
    ballPosition->addChild(ballRotationMatrix);
    targetPosition->setPosition(Vec3(0.0,0.0,0.0));
    targetPosition->addChild(target);
    shadowedScene->addChild(tableRotationMatrix);
    //--------------------------------------------------

    // mehmed and yunus was here.
    scene->addChild(backingY);
    scene->addChild(backingX);
    scene->addChild(motorY);
    scene->addChild(motorX);
    scene->addChild(motorHeadY);
    scene->addChild(motorHeadX);
    scene->addChild(motorPropellerY);
    scene->addChild(motorPropellerX);
    scene->addChild(motorPropellerIgnoredY);
    scene->addChild(motorPropellerIgnoredX);
}

void placeCamera() {
    // --------------------- 3D Camera position settings ------------------------
    Vec3d eye, center, up;

    viewer.addEventHandler(new KeyboardEventHandler());
    viewer.setCameraManipulator(new TrackballManipulator);
    viewer.getCameraManipulator()->getHomePosition(eye,center,up);
    viewer.getCameraManipulator()->setHomePosition(-Vec3d(eye.x(),eye.y()-70,eye.z()-60),
                                                   Vec3d(center.x(),center.y(),center.z()),
                                                   -Vec3d(up.x(),up.y(),up.z()));
    /*--------------------------------------------------------------------------*/
}

// Applies textures to corresponding piece of mechanic system
void setTextureSettings(Texture2D *shapeTexture, Image *textureImage, ref_ptr<ShapeDrawable> shape) {
    Material *material = new Material();
    material->setEmission(Material::FRONT, Vec4(0.8,0.8,0.8,1.0));

    shapeTexture->setDataVariance(Object::DYNAMIC);
    shapeTexture->setFilter(Texture::MIN_FILTER,Texture::LINEAR_MIPMAP_LINEAR);
    shapeTexture->setFilter(Texture::MAG_FILTER,Texture::LINEAR);
    shapeTexture->setWrap(Texture::WRAP_S,Texture::CLAMP);
    shapeTexture->setWrap(Texture::WRAP_T,Texture::CLAMP);
    shapeTexture->setImage(textureImage);

    StateSet *stateSet = shape->getOrCreateStateSet();
    stateSet->ref();
    stateSet->setAttribute(material);
    stateSet->setTextureAttributeAndModes(0,shapeTexture,StateAttribute::ON);
}

osgWidget::Label* createButton(const string& name, const string& text, float size, const Vec4& color) {
    ref_ptr<osgWidget::Label> arrow = new osgWidget::Label(name);
    arrow->setLabel(text);
    arrow->setLayer(osgWidget::Widget::LAYER_MIDDLE);
    arrow->setFont("fonts/arial.ttf");
    arrow->setFontSize(size);
    arrow->setFontColor(1.0f,1.0f,1.0f,1.0f);
    arrow->setColor(color);
    arrow->addSize(3.0f,6.0f);
    arrow->addSize(3.0f,6.0f);
    arrow->setCanFill(true);
    return arrow.release();
}

osgWidget::Window* setupArrows() {
    ref_ptr<osgWidget::Box> arrowsHorizontal = new osgWidget::Box("tabs", osgWidget::Box::HORIZONTAL);
    ref_ptr<osgWidget::Box> main = new osgWidget::Box("main", osgWidget::Box::VERTICAL);
    vector<Vec4*> buttonStateColors;

    buttonStateColors.push_back(new Vec4(0.2f,0.2f,0.2f,1.0f));
    buttonStateColors.push_back(new Vec4(0.0f,0.0f,0.0f,1.0f));
    buttonStateColors.push_back(new Vec4(0.1f,0.1f,0.1f,1.0f));
    buttonStateColors.push_back(new Vec4(0.0f,0.0f,0.0f,1.0f));

    osgWidget::Label* left = createButton("left","<",30.0f,Vec4(0.0f,0.0f,0.0f,1.0f));
    setCallbacks(left,leftArrowCallback,buttonStateColors);
    arrowsHorizontal->addWidget(left);

    osgWidget::Label* right = createButton("right",">",30.0f,Vec4(0.0f,0.0f,0.0f,1.0f));
    setCallbacks(right,rightArrowCallback,buttonStateColors);
    arrowsHorizontal->addWidget(right);

    osgWidget::Label* forward = createButton("forward","/\\",20.0f,Vec4(0.0f,0.0f,0.0f,1.0f));
    setCallbacks(forward,forwardArrowCallback,buttonStateColors);

    osgWidget::Label* backward = createButton("backward","\\/",20.0f,Vec4(0.0f,0.0f,0.0f,1.0f));
    setCallbacks(backward,backwardArrowCallback,buttonStateColors);

    osgWidget::Label* header = createButton("Set position","Target",12.0f,Vec4(0.5f,0.3f,0.3f,1.0f));
    header->setDimensions(0,0,110,40);

    main->setOrigin(20,440);
    main->addWidget(backward);
    main->addWidget(arrowsHorizontal->embed());
    main->addWidget(forward);
    main->addWidget(header);

    return main.release();
}

osgWidget::Window* createUIButton(const string& text,
                                 bool (*callback)(osgWidget::Event&),
                                 int originX,
                                 int originY,
                                 float textSize,
                                 float dimX,
                                 float dimY,
                                 const vector<Vec4*>& buttonStateColors) {
    ref_ptr<osgWidget::Box> box = new osgWidget::Box("UIButtonBox",osgWidget::Box::HORIZONTAL);

    osgWidget::Label* uiButton = createButton(text, text, textSize, *buttonStateColors.at(1));

    setCallbacks(uiButton,callback,buttonStateColors);

    uiButton->setDimensions(0,0,dimX,dimY);

    box->setOrigin(originX,originY);
    box->addWidget(uiButton);

    return box.release();
}

bool leftArrowCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));
    Vec3d oldPosition = targetPosition->getPosition(); // get old position of ball
    targetPosition->setPosition(Vec3d(oldPosition.x()-0.35,oldPosition.y(),oldPosition.z()));

    return true;
}

bool rightArrowCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));
    Vec3d oldPosition = targetPosition->getPosition(); // get old position of ball
    targetPosition->setPosition(Vec3d(oldPosition.x()+0.35,oldPosition.y(),oldPosition.z()));

    return true;
}

bool forwardArrowCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));
    Vec3d oldPosition = targetPosition->getPosition(); // get old position of ball
    targetPosition->setPosition(Vec3d(oldPosition.x(),oldPosition.y(),oldPosition.z()-0.35));

    return true;
}

bool backwardArrowCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));
    Vec3d oldPosition = targetPosition->getPosition(); // get old position of ball
    targetPosition->setPosition(Vec3d(oldPosition.x(),oldPosition.y(),oldPosition.z()+0.35));

    return true;
}

bool releaseCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));
    return true;
}

bool mouseOverCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));

    return true;
}

bool mouseLeaveCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));

    return true;
}

bool drawCircleCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));

    return true;
}

bool drawSquareCallback(osgWidget::Event& ev) {
    osgWidget::Label *label = static_cast<osgWidget::Label*>(ev.getWidget());
    label->setColor(*static_cast<Vec4*>(ev.getData()));

    return true;
}

// Set events and callbacks of buttons
void setCallbacks(osgWidget::Label* button,
                  bool (*moveCallback)(osgWidget::Event&),
                  vector<Vec4*> buttonStateColors) {
    button->setLayer(osgWidget::Widget::LAYER_MIDDLE);
    button->addEventMask(osgWidget::EVENT_MOUSE_PUSH);
    button->addEventMask(osgWidget::EVENT_MOUSE_RELEASE);
    button->addEventMask(osgWidget::EVENT_MOUSE_OVER);
    button->addEventMask(osgWidget::EVENT_MOUSE_LEAVE);

    if (moveCallback != NULL) {
        button->addCallback(new osgWidget::Callback(moveCallback,
                                                    osgWidget::EVENT_MOUSE_PUSH,
                                                    (void *) buttonStateColors.at(0)));

        button->addCallback(new osgWidget::Callback(&releaseCallback,
                                                    osgWidget::EVENT_MOUSE_RELEASE,
                                                    (void *)buttonStateColors.at(1)));
        button->addCallback(new osgWidget::Callback(&mouseOverCallback,
                                                    osgWidget::EVENT_MOUSE_OVER,
                                                    (void *)buttonStateColors.at(2)));
        button->addCallback(new osgWidget::Callback(&mouseLeaveCallback,
                                                    osgWidget::EVENT_MOUSE_LEAVE,
                                                    (void *)buttonStateColors.at(3)));
    }
}